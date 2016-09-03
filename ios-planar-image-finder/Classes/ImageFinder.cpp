#include <cassert>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <errno.h>

#include "accel.h"
#include "nn/nn.h"
#include "ImageFinder.h"
#include "SystemPaths.h"
#include "fast_640x480xBGRA_to_320x240xY.inl"
#include "save-as-grey-pgm.inl"
#include "mat44_multiply.inl"

// All the Haar patterns used in the program
static const float dx_s[]  = { 0,2,3,7,1,0,0,0,3,2,6,7,-2,0,0,0,6,2,9,7,1,0,0,0 };
static const float dy_s[]  = { 2,0,7,3,1,0,0,0,2,3,7,6,-2,0,0,0,2,6,7,9,1,0,0,0 };
static const float dxy_s[] = { 1,1,4,4,1,0,0,0,5,1,8,4,-1,0,0,0,1,5,4,8,-1,0,0,0,5,5,8,8,1,0,0,0 };
static const float ndx_s[] = { 0,0,2,4,-1,0,0,0,2,0,4,4,1,0,0,0 };
static const float ndy_s[] = { 0,0,4,2,1,0,0,0,0,2,4,4,-1,0,0,0 };
// sizes[octave][layer] = (9+6*layer)<<octave
static const int sizes[4][4] = {{9,15,21,27},{18,30,42,54},{36,60,84,108},{72,120,168,216}};

#define DESCRIPTOR_LENGTH 16 // 64
#define PATCH_SZ 12 // 20
#define MAX_REPROJECTION_ERROR 2
#define ORI_WIN 40 // 60
#define ORI_SEARCH_INC 9 // 4
#define ORI_RADIUS 4 // 6
#define ORI_SIGMA 2.5f
#define DESC_SIGMA 3.3f
#define MAX_ORI_SAMPLES ((2*ORI_RADIUS+1)*(2*ORI_RADIUS+1))
#define HESSIAN_THRESHOLD 1000.f

class ImageFinder::PImpl {
	// FIXME: All the Haar stuff uses arrays of structures, making it extremely difficult
	// to vectorize. It should be turned into structures of arrays instead...
	struct haar_t {
		int p[4];
		float w;
		
		static inline float32x2_t calc2(const int* __restrict__ origin,
										const haar_t* __restrict__ fx,
										const haar_t* __restrict__ fy) {
			float32x2_t tmp = {
				fx[0].w*float((origin[fx[0].p[0]] - origin[fx[0].p[2]]) - (origin[fx[0].p[1]] - origin[fx[0].p[3]])) +
				fx[1].w*float((origin[fx[1].p[0]] - origin[fx[1].p[2]]) - (origin[fx[1].p[1]] - origin[fx[1].p[3]])),
				fy[0].w*float((origin[fy[0].p[0]] - origin[fy[0].p[2]]) - (origin[fy[0].p[1]] - origin[fy[0].p[3]])) +
				fy[1].w*float((origin[fy[1].p[0]] - origin[fy[1].p[2]]) - (origin[fy[1].p[1]] - origin[fy[1].p[3]]))
			};
			return tmp;
		}
		
		static inline float calc34(const int* __restrict__ origin,
								   const haar_t* __restrict__ fx,
								   const haar_t* __restrict__ fy,
								   const haar_t* __restrict__ fz) {
			const float tmp(
			fz[0].w*float((origin[fz[0].p[0]] - origin[fz[0].p[2]]) - (origin[fz[0].p[1]] - origin[fz[0].p[3]])) +
			fz[1].w*float((origin[fz[1].p[0]] - origin[fz[1].p[2]]) - (origin[fz[1].p[1]] - origin[fz[1].p[3]])) +
			fz[2].w*float((origin[fz[2].p[0]] - origin[fz[2].p[2]]) - (origin[fz[2].p[1]] - origin[fz[2].p[3]])) +
			fz[3].w*float((origin[fz[3].p[0]] - origin[fz[3].p[2]]) - (origin[fz[3].p[1]] - origin[fz[3].p[3]]))
			);
			return (
			fx[0].w*float((origin[fx[0].p[0]] - origin[fx[0].p[2]]) - (origin[fx[0].p[1]] - origin[fx[0].p[3]])) +
			fx[1].w*float((origin[fx[1].p[0]] - origin[fx[1].p[2]]) - (origin[fx[1].p[1]] - origin[fx[1].p[3]])) +
			fx[2].w*float((origin[fx[2].p[0]] - origin[fx[2].p[2]]) - (origin[fx[2].p[1]] - origin[fx[2].p[3]]))
			) * (
			fy[0].w*float((origin[fy[0].p[0]] - origin[fy[0].p[2]]) - (origin[fy[0].p[1]] - origin[fy[0].p[3]])) +
			fy[1].w*float((origin[fy[1].p[0]] - origin[fy[1].p[2]]) - (origin[fy[1].p[1]] - origin[fy[1].p[3]])) +
			fy[2].w*float((origin[fy[2].p[0]] - origin[fy[2].p[2]]) - (origin[fy[2].p[1]] - origin[fy[2].p[3]]))
			) - .81f*tmp*tmp;
		}

		template<int n, int oldSize>
		static inline void resize(const float* __restrict__ src, haar_t* __restrict__ dst, int newSize, int widthStep ) {
			const float r(float(newSize)/float(oldSize));
			for( int k(0); k<n; ++k) {
				int dx1 = int( r*src[k*8  ] );
				int dy1 = int( r*src[k*8+1] );
				int dx2 = int( r*src[k*8+2] );
				int dy2 = int( r*src[k*8+3] );
				dst[k].p[0] = dy1*widthStep + dx1;
				dst[k].p[1] = dy2*widthStep + dx1;
				dst[k].p[2] = dy1*widthStep + dx2;
				dst[k].p[3] = dy2*widthStep + dx2;
				dst[k].w = src[k*8+4]/float((dx2-dx1)*(dy2-dy1));
			}
		}
	};

	// A point and a size
	struct point_and_size_t {
		ImageFinder::point_t pt;
		int size;
		point_and_size_t(): pt(), size(0) { }
		point_and_size_t(float x, float y, int _size): pt(x,y), size(_size) { }
		point_and_size_t(const ImageFinder::point_t& _pt, int _size): pt(_pt), size(_size) { }
		point_and_size_t(const point_and_size_t& o): pt(o.pt), size(o.size) { }
		point_and_size_t& operator=(const point_and_size_t& o) { pt=o.pt; size=o.size; return *this; }
	};
	
	// Integral of the current video frame
	// mSum(x, y) = sum_of(mFrameGreyscale(x', y')) for all x'<x and y'<y
	std::vector<int> mSum;
	
	// Containers for keypoints and descriptors
	std::vector<point_and_size_t> mTempPoints, mObjectPoints, mImagePoints;
	std::vector<float> mTempDescriptors, mObjectDescriptors, mImageDescriptors;
	
	// Pair of containers for our pairs of matching points
	std::vector<ImageFinder::point_t> mPairsObject, mPairsImage;

	// Object corners in marker space, then in frame space
	ImageFinder::corners_t mObjectCorners;
	
	// These are used for building patches & neighborhood
	ImageFinder::point_t mApt[MAX_ORI_SAMPLES];
	float mAptw[MAX_ORI_SAMPLES];
	int mAngle0;
	float mDW[PATCH_SZ][PATCH_SZ];

	// determinants
	std::vector<float> mDets[4];
	
	// Nearest neighbours index
	nn::KDTreeIndex* mpNNIndex;
	
	// Log stream
	std::ofstream mLog;
	
	////////////
	
	pthread_mutex_t mMutex;
	
	std::vector<unsigned char> mMarkerData, mWorkingMarkerData, mSceneData, mWorkingSceneData;
	size_t mMarkerWidth, mMarkerHeight, mMarkerBytesPerRow, mMarkerBytesPerPixel;
	size_t mWorkingMarkerWidth, mWorkingMarkerHeight, mWorkingMarkerBytesPerRow, mWorkingMarkerBytesPerPixel;
	size_t mSceneWidth, mSceneHeight, mSceneBytesPerRow;
	size_t mWorkingSceneWidth, mWorkingSceneHeight, mWorkingSceneBytesPerRow;
	
	ImageFinder::transformation_matrix_t mPose, mWorkingPose, mCornersPose;
	bool mCornersAvailable;
	ImageFinder::corners_t mCorners;
	
	pthread_t mThread;
	volatile bool mThreadRunning;
    volatile bool mIncomingPicture;
	
public:
	PImpl()
	: mSum(321*241)
	, mTempPoints()
	, mObjectPoints()
	, mImagePoints()
	, mTempDescriptors()
	, mObjectDescriptors()
	, mImageDescriptors()
	, mPairsObject()
	, mPairsImage()
	, mAngle0(0)
	, mpNNIndex(0)
	, mLog()
	, mMarkerData()
	, mMarkerWidth(0)
	, mMarkerHeight(0)
	, mMarkerBytesPerRow(0)
	, mMarkerBytesPerPixel(0)
	, mWorkingMarkerData()
	, mWorkingMarkerWidth(0)
	, mWorkingMarkerHeight(0)
	, mWorkingMarkerBytesPerRow(0)
	, mWorkingMarkerBytesPerPixel(0)
	, mSceneData(640*480*4)
	, mSceneWidth(0)
	, mSceneHeight(0)
	, mSceneBytesPerRow(0)
	, mWorkingSceneData(320*240)
	, mWorkingSceneWidth(0)
	, mWorkingSceneHeight(0)
	, mWorkingSceneBytesPerRow(0)
	, mCornersAvailable(false)
	, mThreadRunning(true)
    , mIncomingPicture(false)
	{
		std::stringstream logfile;
		logfile << SystemPaths::get(SystemPaths::TEMP) << "/log.txt";
		mLog.open(logfile.str().c_str());
		mLog.precision(5);
		
		cv::Mat matG = cv::getGaussianKernel( 2*ORI_RADIUS+1, ORI_SIGMA, CV_32F );
		const float* G = (const float*)matG.data;
		for(int i(-ORI_RADIUS); i<=ORI_RADIUS; ++i) {
			for(int j(-ORI_RADIUS); j<=ORI_RADIUS; ++j) {
				if(i*i + j*j <= ORI_RADIUS*ORI_RADIUS ) {
					mApt[mAngle0]  = ImageFinder::point_t(j,i);
					mAptw[mAngle0] = G[i+ORI_RADIUS]*G[j+ORI_RADIUS];
					++mAngle0;
				}
			}
		}

		const float c2(.5f/(DESC_SIGMA*DESC_SIGMA));
		const float disp(float(PATCH_SZ-1)*.5f);
		float gs(0.f);
		for(int i(0); i<PATCH_SZ; ++i) {
			for(int j(0); j<PATCH_SZ; ++j) {
				const float x(float(j) - disp);
				const float y(float(i) - disp);
				const float val(expf(-(x*x+y*y)*c2));
				mDW[i][j] = val;
				gs += val;
			}
		}
		gs=1.f/gs;
		for(int i(0); i<PATCH_SZ; ++i) {
			for(int j(0); j<PATCH_SZ; ++j) {
				mDW[i][j] *= gs;
			}
		}
		
		if (pthread_mutex_init(&mMutex, 0))  throw std::runtime_error("Cannot create mutex!");
		if (pthread_create(&mThread, 0, static_main, this)) throw std::runtime_error("Cannot start ImageFinder's thread!");
	}
	~PImpl() {
		mThreadRunning=false;
		pthread_join(mThread, 0);
		delete mpNNIndex;
		pthread_mutex_destroy(&mMutex);
		mLog.close();
	}
	
	// The hessian detector detect scale invariant key points in the picture
	void hessianDetector(const CvMat& img) {
		mTempPoints.clear();
		
		// Resize layers if needed
		for (int layer(0); layer<4; ++layer) {
			if (mDets[layer].size()<size_t(img.cols*img.rows)) {
				mDets[layer].resize(img.cols*img.rows);
			}
		}
		
		// Resize sum vector if needed
		if (mSum.size()<(size_t)(img.cols+1)*(img.rows+1)) {
			mSum.resize((img.cols+1)*(img.rows+1));
		}
		
		// compute sum (equivalent to cvIntegral(&img, &mSum[0]))
		size_t sumOffset(img.cols+2);
		size_t srcOffset(0);
		accel::vClear(&mSum[0], img.cols+1);
		for (size_t y(0); y<(size_t)img.rows; ++y, sumOffset+=(img.cols+1), srcOffset+=img.step) {
			
			// set 1st pixel of the row to 0
			mSum[sumOffset-1] = 0;
			
			// sum[x,y] = sum[x, y-1] + s
			size_t s(0);
			for (size_t x(0); x<(size_t)img.cols; ++x) {
				s+=img.data.ptr[srcOffset+x];
				mSum[sumOffset+x]=mSum[sumOffset - (img.cols+1) + x] + s;
			}
		}
		
		int rows(img.rows);
		int cols(img.cols);

		// Cache the haar patterns between calls if the picture's width doesn't change
		static struct haar_for_hessian_detector_t {
			haar_t Dx[4][4][3];
			haar_t Dy[4][4][3];
			haar_t Dxy[4][4][4];
			size_t size;
			haar_for_hessian_detector_t(): size(0) { }
			void resize(size_t s) {
				if (size==s) return;
				size=s;
				for(int octave(0); octave<4; ++octave) {
					for(int layer(0); layer<4; ++layer) {
						haar_t::resize<3,9>( dx_s, Dx[octave][layer], sizes[octave][layer], s);
						haar_t::resize<3,9>( dy_s, Dy[octave][layer], sizes[octave][layer], s);
						haar_t::resize<4,9>( dxy_s, Dxy[octave][layer], sizes[octave][layer], s);
					}
				}
			}
		} haars;
		haars.resize(img.cols+1);
		
		for(int octave(0); octave<4; ++octave, rows>>=1, cols>>=1 ) {
			// compute determinant of the hessian
			for(int layer(0); layer<4; ++layer) {
				int size = sizes[octave][layer];
				int margin = size>>(octave+1);
				for(int sum_i(0), i(margin); sum_i<=img.rows-size; sum_i+=(1<<octave), ++i) {
					for(size_t sum_j(0), j(margin), off(i*img.cols + margin); sum_j<=(size_t)img.cols-size; sum_j+=(1<<octave), ++j, ++off) {
						mDets[layer][off] = haar_t::calc34(&mSum[sum_i*(img.cols+1) + sum_j],
														   haars.Dx[octave][layer],
														   haars.Dy[octave][layer],
														   haars.Dxy[octave][layer]);
					}
				}
			}
			
			// find maximum value for determinant of the hessian
			for(int layer(1); layer<3; ++layer) {
				int size = sizes[octave][layer];
				int margin = (sizes[octave][layer+1]>>(octave+1))+1;
				for(int i(margin); i < rows-margin; ++i ) {
					for(int j(margin); j < cols-margin; ++j ) {
						const float& val0(mDets[layer][i*img.cols + j]);
						bool fail(val0<HESSIAN_THRESHOLD);
						if (fail) continue;
						const int start(i*img.cols + j - 2);
						float N9[27]; // 3x3x3 matrix
						
						int ii(0), ll(layer-2), o;
#define STEP { N9[ii]=mDets[ll][++o]; fail=fail||(val0<=N9[ii]); ++ii; } 
						++ll;
						o = start-img.cols;	STEP; STEP; STEP;
						o = start;			STEP; STEP; STEP;
						o = start+img.cols;	STEP; STEP; STEP;
						if (fail) continue;
						++ll;
						o = start-img.cols;	STEP; STEP; STEP;
						o = start;			STEP; N9[ii]=mDets[ll][++o]; ++ii; STEP;
						o = start+img.cols;	STEP; STEP; STEP;
						if (fail) continue;
						++ll;
						o = start-img.cols;	STEP; STEP; STEP;
						o = start;			STEP; STEP; STEP;
						o = start+img.cols;	STEP; STEP; STEP;
						if (fail) continue;
#undef STEP
						
						// Interpolate point
						float b[3];
						b[0] = -(N9[14]-N9[12])*.5f;				// Negative 1st deriv with respect to x
						b[1] = -(N9[16]-N9[10])*.5f;				// Negative 1st deriv with respect to y
						b[2] = -(N9[22]-N9[ 4])*.5f;				// Negative 1st deriv with respect to s
						float A[9];
						A[0] = N9[12]-2.f*N9[13]+N9[14];			// 2nd deriv x, x
						A[4] = N9[10]-2.f*N9[13]+N9[16];			// 2nd deriv y, y
						A[8] = N9[ 4]-2.f*N9[13]+N9[22];			// 2nd deriv s, s
						A[1] = (N9[17]-N9[15]-N9[11]+N9[9])*.25f;	// 2nd deriv x, y
						A[2] = (N9[23]-N9[21]-N9[ 5]+N9[3])*.25f;	// 2nd deriv x, s
						A[5] = (N9[25]-N9[19]-N9[ 7]+N9[1])*.25f;	// 2nd deriv y, s
						A[3] = A[1];								// 2nd deriv y, x
						A[6] = A[2];								// 2nd deriv s, x
						A[7] = A[5];								// 2nd deriv s, y
						__CLPK_integer n = 3;
						__CLPK_integer nrhs = 1;
						__CLPK_integer ipiv[3];
						__CLPK_integer info;
						sgesv_(&n, &nrhs, A, &n, ipiv, b, &n, &info);
						const int point_size(sizes[octave][layer] + int(b[2]*float(sizes[octave][layer]-sizes[octave][layer-1])));
						if (info || (point_size < 1)) continue;
						
						mTempPoints.push_back(
							point_and_size_t(
								ImageFinder::point_t(
									b[0]*(1<<octave) + float((j-(size>>(octave+1)))<<octave) + float(size-1)*.5f,
									b[1]*(1<<octave) + float((i-(size>>(octave+1)))<<octave) + float(size-1)*.5f
								),
								point_size
							)
						);
					}
				}
			}
		}
	}	
#ifdef DOESNT_WORK_YET
	bool findHomography(float* H) {
		const size_t num_points(mPairsObject.size());
		unsigned int indices[4];
		for (size_t i(0); i<1; ++i) {
			for (size_t j(0); j<4; ++j) indices[j] = j; // accel::rand(num_points);
			
			float candidate[9];
			if (!accel::find_homography_4_pairs(candidate,
								 (const float*) &mPairsObject[0],
								 (const float*) &mPairsImage[0],
								 indices)) continue;

			// Reproject points
			float error_max(0);
			for (size_t p(0); p<num_points && error_max<=MAX_REPROJECTION_ERROR*MAX_REPROJECTION_ERROR; ++p) {
				const ImageFinder::point_t& f(mPairsObject[p]);
				const ImageFinder::point_t& t(mPairsImage[p]);
				const float w(     1.f/(candidate[6]*f.x + candidate[7]*f.y + candidate[8]));
				const float dx(t.x - w*(candidate[0]*f.x + candidate[1]*f.y + candidate[2]));
				const float dy(t.y - w*(candidate[3]*f.x + candidate[4]*f.y + candidate[5]));
				const float square_error(dx*dx + dy*dy);
				error_max = std::max(square_error, error_max);
			}
			if (error_max>MAX_REPROJECTION_ERROR*MAX_REPROJECTION_ERROR) continue;
			for (size_t h(0); h<9; ++h) H[h]=candidate[h];
			return true;
		}
		return false;
	}
#endif
	bool getHomography(float* H) {
		if (!mpNNIndex || mImagePoints.size()<4) return false;
		
		const size_t num_dataset(mImageDescriptors.size()/DESCRIPTOR_LENGTH);
		
		mPairsObject.clear();
		mPairsImage.clear();
		
		// Descriptors are 64-(or 16-)dimensional points.
		// We build pairs of features by matching those high-dimensional points.
		int indices[2];
		float dists[2];
		for (size_t i(0), j(0);i<num_dataset && j<50; ++i) {
			mpNNIndex->findNeighbors(&mImageDescriptors[i*DESCRIPTOR_LENGTH], indices, dists, 64);
			// This check ensures no other candidate is too close to our reference point
			// i.e. it keeps only well differentiated candidates
			if (dists[0] < .6f*dists[1]) {
				mPairsObject.push_back(mObjectPoints[indices[0]].pt);
				mPairsImage.push_back(mImagePoints[i].pt);
				++j; // break out of the loop if we have N pairs already
			}
		}
		
		const size_t n(mPairsObject.size());
		
		// We need at least 4 points to compute an homography
		if (n<4) return false;

		CvMat _h   = cvMat(3, 3, CV_32F, H);
		CvMat _pt1 = cvMat(1, n, CV_32FC2, &mPairsObject[0]);
		CvMat _pt2 = cvMat(1, n, CV_32FC2, &mPairsImage[0]);
		return cvFindHomography(&_pt1, &_pt2, &_h, CV_RANSAC /* CV_LMEDS */, MAX_REPROJECTION_ERROR);
	}
	
	// Extract rotation & scale invariant features
	void extractSURF(
		CvMat& img,
		std::vector<point_and_size_t>& keyPoints,
		std::vector<float>& descriptors
	) {
		static const float WTF_FACTOR(1.2f/9.f); // yeah, WTF...
		
		keyPoints.clear();
		descriptors.clear();

		// Get image keypoints
		hessianDetector(img);
		
		const size_t keypoint_count(mTempPoints.size());
		mTempDescriptors.resize(DESCRIPTOR_LENGTH*keypoint_count);
		
		haar_t dx_t[2], dy_t[2];
		
		float32x2_t XY[MAX_ORI_SAMPLES];
		float angle[MAX_ORI_SAMPLES];
		unsigned char PATCH[PATCH_SZ+1][PATCH_SZ+1];
		float DX[PATCH_SZ][PATCH_SZ], DY[PATCH_SZ][PATCH_SZ];
		
		int maxSize(0);
		for(int k(0); k<(int)keypoint_count; ++k)
			maxSize = std::max(maxSize, mTempPoints[k].size);
		maxSize = cvCeil((PATCH_SZ+1)*maxSize*WTF_FACTOR);

		std::vector<unsigned char> winbuf(maxSize*maxSize);
		
		for(int k(0); k<(int)keypoint_count; ++k) {
			point_and_size_t& kp = mTempPoints[k];
			const ImageFinder::point_t& center(kp.pt);
			const float s((float)kp.size*WTF_FACTOR);
			
			int grad_wav_size = int(2.f*s)<<1;
			haar_t::resize<2,4>( ndx_s, dx_t, grad_wav_size, img.cols+1 );
			haar_t::resize<2,4>( ndy_s, dy_t, grad_wav_size, img.cols+1 );
			size_t nangle(0);
			const float gwsf(float(grad_wav_size-1)*.5f);
			for(int kk(0); kk<mAngle0; ++kk) {
				const int x(center.x + mApt[kk].x*s - gwsf);
				const int y(center.y + mApt[kk].y*s - gwsf);
				if((y>=0) && (y<(img.rows+1-grad_wav_size)) &&
					(x>=0) && (x<(img.cols+1-grad_wav_size))) {
					const int* ptr(&mSum[x + y*(img.cols+1)]);
					XY[nangle] = vmul_f32(haar_t::calc2(ptr, dx_t, dy_t), vdup_n_f32(mAptw[kk]));
					++nangle;
				}
			}
			
			for (size_t i(0); i<nangle; ++i) {
				const float* ptr((const float*) &XY[i]);
				angle[i]=accel::fastAtan2Deg(ptr[1], ptr[0]);
			}
			
			// Find the principal direction
			float32x2_t best = { 0.f, 0.f};
			float descriptor_mod = 0;
			for(int i(0); i<360; i+=ORI_SEARCH_INC) {
				float32x2_t sum = { 0.f, 0.f};
				for(size_t j(0); j<nangle; ++j) {
					const int d(std::abs(int(angle[j]) - i));
					if( d < (ORI_WIN>>1) || d > 360-(ORI_WIN>>1) ) {
						sum += XY[j];
					}
				}
				float temp_mod = vget_lane_f32(vpadd_f32(sum*sum, sum*sum), 0);
				if( temp_mod > descriptor_mod ) {
					descriptor_mod = temp_mod;
					best = sum;
				}
			}
			
			int win_size = (int)((PATCH_SZ+1)*s);
			const float win_offset(-float(win_size-1)*.5f);
			const float32x2_t ones = { 1.f, -1.f };
			const int32x2_t zeroes = { 0, 0 };
			const int32x2_t maxes  = { img.cols-1, img.rows-1 };
			
			float32x2_t dir = vmul_f32(best, best);
			dir=vmul_f32(best, vrsqrte_f32(vpadd_f32(dir, dir)));
			float32x2_t zdir = vmul_f32(dir, ones);
			float32x2_t start = *(float32x2_t*)&center;
			start = vmla_f32(start, vdup_n_f32(win_offset), vadd_f32(zdir, vrev64_f32(dir)));
			
			for(int i(0); i<win_size; ++i, start = vadd_f32(start, vrev64_f32(dir))) {
				float32x2_t pixel=start;
				for(int j(0); j<win_size; ++j, pixel = vadd_f32(pixel, zdir)) {
					const int32x2_t pi = vmin_s32(vmax_s32(vcvt_s32_f32(pixel), zeroes), maxes);
					winbuf[i*win_size + j] = img.data.ptr[vget_lane_s32(pi,1)*img.step + vget_lane_s32(pi,0)];
				}
			}
			
			// FIXME: this method sucks.
			CvMat _patch = cvMat(PATCH_SZ+1, PATCH_SZ+1, CV_8U, PATCH);
			CvMat win = cvMat(win_size, win_size, CV_8U, &winbuf[0]);
			cvResize( &win, &_patch, CV_INTER_AREA );
			
			// Construct descriptor
			size_t vec_offset(DESCRIPTOR_LENGTH*k);
			accel::vClear((size_t*) &mTempDescriptors[vec_offset], DESCRIPTOR_LENGTH);
			
			for(int i(0); i<PATCH_SZ; ++i) {
				for(int j(0); j<PATCH_SZ; ++j) {
					DX[i][j] = (- PATCH[i][j] + PATCH[i][j+1] - PATCH[i+1][j] + PATCH[i+1][j+1])*mDW[i][j];
					DY[i][j] = (- PATCH[i][j] - PATCH[i][j+1] + PATCH[i+1][j] + PATCH[i+1][j+1])*mDW[i][j];
				}
			}
			
#if DESCRIPTOR_LENGTH==64
			for(int i(0); i<4; ++i) {
				for(int j(0); j<4; ++j, vec_offset+=4) {
					for(int y(i*(PATCH_SZ/4)); y<i*(PATCH_SZ/4)+(PATCH_SZ/4); ++y) {
						for(int x(j*(PATCH_SZ/4)); x<j*(PATCH_SZ/4)+(PATCH_SZ/4); ++x) {
							const float tx(DX[y][x]), ty(DY[y][x]);
							mTempDescriptors[vec_offset  ] += tx;
							mTempDescriptors[vec_offset+1] += ty;
							mTempDescriptors[vec_offset+2] += std::abs(tx);
							mTempDescriptors[vec_offset+3] += std::abs(ty);
						}
					}
				}
			}
#elif DESCRIPTOR_LENGTH==16
			for(int i(0); i<2; ++i) {
				for(int j(0); j<2; ++j, vec_offset+=4) {
					for(int y(i*(PATCH_SZ/2)); y<i*(PATCH_SZ/2)+(PATCH_SZ/2); ++y) {
						for(int x(j*(PATCH_SZ/2)); x<j*(PATCH_SZ/2)+(PATCH_SZ/2); ++x) {
							const float tx(DX[y][x]), ty(DY[y][x]);
							mTempDescriptors[vec_offset  ] += tx;
							mTempDescriptors[vec_offset+1] += ty;
							mTempDescriptors[vec_offset+2] += std::abs(tx);
							mTempDescriptors[vec_offset+3] += std::abs(ty);
						}
					}
				}
			}
#else
#error "Not implemented"
#endif
			// Normalize the descriptor
			accel::vNormalize(&mTempDescriptors[DESCRIPTOR_LENGTH*k], DESCRIPTOR_LENGTH);
		}

		mTempPoints.swap(keyPoints);
		mTempDescriptors.swap(descriptors);
	}
	
	void setMarker(void* data, size_t width, size_t height, size_t bytesPerRow, size_t bytesPerPixel) {
		const int error(pthread_mutex_lock(&mMutex));
		if (!error) {
			mMarkerData.resize(height*bytesPerRow);
			accel::MemCopy(&mMarkerData[0], data, height*bytesPerRow);
			mMarkerWidth=width;
			mMarkerHeight=height;
			mMarkerBytesPerRow=bytesPerRow;
			mMarkerBytesPerPixel=bytesPerPixel;
			pthread_mutex_unlock(&mMutex);
		}
		switch (error) {
			case EDEADLK: throw std::runtime_error("Locking mutex would result in deadlock"); break;
			case EINVAL:  throw std::runtime_error("Invalid mutex specified"); break;
		}
	}
	void addVideoFrame(void* data, size_t width, size_t height, size_t bytesPerRow, const ImageFinder::transformation_matrix_t& pose) {
		const int error(pthread_mutex_lock(&mMutex));
		if (!error) {
			mSceneData.resize(height*bytesPerRow);
			accel::MemCopy(&mSceneData[0], data, height*bytesPerRow);
			mSceneWidth=width;
			mSceneHeight=height;
			mSceneBytesPerRow=bytesPerRow;
			accel::MemCopy(mPose, pose, sizeof(ImageFinder::transformation_matrix_t));
            mIncomingPicture=true;
			pthread_mutex_unlock(&mMutex);
		}
		switch (error) {
			case EDEADLK: throw std::runtime_error("Locking mutex would result in deadlock"); break;
			case EINVAL:  throw std::runtime_error("Invalid mutex specified"); break;
		}
	}
	bool pollCorners(ImageFinder::corners_t& corners, ImageFinder::transformation_matrix_t& pose) {
		bool cornersAvailable=false;
		const int error(pthread_mutex_lock(&mMutex));
		if (!error) {
			if (mCornersAvailable) {
				accel::MemCopy(corners, mCorners, sizeof(ImageFinder::corners_t));
				accel::MemCopy(pose, mCornersPose, sizeof(ImageFinder::transformation_matrix_t));
				cornersAvailable=true;
				mCornersAvailable=false;
			}
			pthread_mutex_unlock(&mMutex);
			return cornersAvailable;
		}
		switch (error) {
			case EDEADLK: throw std::runtime_error("Locking mutex would result in deadlock"); break;
			case EINVAL:  throw std::runtime_error("Invalid mutex specified"); break;
		}
		return false;
	}
	
	void main() {
		while(mThreadRunning) {
			// for some reason, sched_yield() crashes the app…
			usleep(5000);
			if (!mIncomingPicture) continue;

			// See if there is a new marker or a new scene...
			if (!pthread_mutex_lock(&mMutex)) {
				// copy marker
				mMarkerData.swap(mWorkingMarkerData);
				std::swap(mMarkerWidth, mWorkingMarkerWidth);
				std::swap(mMarkerHeight, mWorkingMarkerHeight);
				std::swap(mMarkerBytesPerRow, mWorkingMarkerBytesPerRow);
				std::swap(mMarkerBytesPerPixel, mWorkingMarkerBytesPerPixel);
				// copy scene
				std::swap(mSceneWidth, mWorkingSceneWidth);
				std::swap(mSceneHeight, mWorkingSceneHeight);
				std::swap(mSceneBytesPerRow, mWorkingSceneBytesPerRow);

				assert(mWorkingSceneWidth == 640 && mWorkingSceneHeight == 480);
				assert(mWorkingSceneBytesPerRow == 640*4);
				fast_640x480xBGRA_to_320x240xY(&mWorkingSceneData[0], &mSceneData[0]);
				mSceneData.clear();
				
				accel::MemCopy(mWorkingPose, mPose, sizeof(ImageFinder::transformation_matrix_t));
                mIncomingPicture = false;
				pthread_mutex_unlock(&mMutex);
			}
			
			if (!mWorkingMarkerData.empty()) {
				if (mWorkingMarkerBytesPerPixel != 1) {
					assert(mWorkingMarkerBytesPerPixel == 4);
					// Convert colorspace in-place
					size_t bytesPerRow = mWorkingMarkerWidth;
					if (mWorkingMarkerWidth&3) { bytesPerRow+=4; }
					for (size_t y(0); y<mWorkingMarkerHeight; ++y) {
						const unsigned int* src = ((const unsigned int*) &mWorkingMarkerData[0]) + y*(mWorkingMarkerBytesPerRow>>2);
						unsigned char* dst      = ((unsigned char*) &mWorkingMarkerData[0]) + y*bytesPerRow;
						for (size_t x(0); x<mWorkingMarkerWidth; ++x) {
							dst[x] = (src[x]&0xff)+(((src[x]>>8)&0xff)<<1)+((src[x]>>16)&0xff);
						}
					}
					mWorkingMarkerBytesPerPixel = 1;
					mWorkingMarkerBytesPerRow=bytesPerRow;
				}
					
				CvMat mat;
				cvInitMatHeader(&mat,
								mWorkingMarkerHeight,
								mWorkingMarkerWidth,
								CV_8U,
								&mWorkingMarkerData[0],
								mWorkingMarkerBytesPerRow); 
				
				mObjectCorners[0] = ImageFinder::point_t(0.f, 0.f);
				mObjectCorners[1] = ImageFinder::point_t(mWorkingMarkerWidth, 0);
				mObjectCorners[2] = ImageFinder::point_t(mWorkingMarkerWidth, mWorkingMarkerHeight);
				mObjectCorners[3] = ImageFinder::point_t(0.f, mWorkingMarkerHeight);

				extractSURF(mat, mObjectPoints, mObjectDescriptors);
				
				// Free temporary memory
				std::vector<point_and_size_t>().swap(mTempPoints);
				std::vector<float>().swap(mTempDescriptors);
				mWorkingMarkerData.clear();
				
				// Create nearest neighbors index
				if (mpNNIndex) { delete mpNNIndex; mpNNIndex=0; }
				mpNNIndex = new nn::KDTreeIndex(&mObjectDescriptors[0], mObjectDescriptors.size()/DESCRIPTOR_LENGTH, DESCRIPTOR_LENGTH);
				mpNNIndex->buildIndex();
			}
			
			if (!mWorkingSceneData.empty()) {
				CvMat mat;
				cvInitMatHeader(&mat, 240, 320, CV_8U, &mWorkingSceneData[0], 320); 
				extractSURF(mat, mImagePoints, mImageDescriptors);
				
				float H[9];
				if (getHomography(H)) {
					ImageFinder::corners_t corners;
					bool ok=true;
					for(int i(0); i<4; ++i) {
						const float x(mObjectCorners[i].x), y(mObjectCorners[i].y);
						const float Z(1.f/(H[6]*x + H[7]*y + H[8]));
						corners[i].x = (H[0]*x + H[1]*y + H[2])*Z;
						corners[i].y = (H[3]*x + H[4]*y + H[5])*Z;
						// All points are inside the frame:
						ok = ok &&
							(corners[i].x>=0.f) &&
							(corners[i].y>=0.f) && 
							(corners[i].x<mWorkingSceneWidth) && 
							(corners[i].y<mWorkingSceneHeight);
					}
					if (ok) {
						// 4-gon is convex:
						const ImageFinder::point_t v1(corners[1].x - corners[0].x, corners[1].y - corners[0].y);
						const ImageFinder::point_t v2(corners[3].x - corners[0].x, corners[3].y - corners[0].y);
						const float detv1v2rcpt(1.f/(v1.x*v2.y - v1.y*v2.x));
						const float a(detv1v2rcpt*((corners[2].x*v2.y - corners[2].y*v2.x) - (corners[0].x*v2.y - corners[0].y*v2.x)));
						const float b(detv1v2rcpt*((corners[0].x*v1.y - corners[0].y*v1.x) - (corners[2].x*v1.y - corners[2].y*v1.x)));
						if ((a>=0.f) && (b>=0.f) && (a+b>=1.f)) {
							if (!pthread_mutex_lock(&mMutex)) {
								accel::MemCopy(mCorners, corners, sizeof(ImageFinder::corners_t));
								accel::MemCopy(mCornersPose, mWorkingPose, sizeof(ImageFinder::transformation_matrix_t));
								mCornersAvailable=true;
								pthread_mutex_unlock(&mMutex);
							}
						}
					}
				}
			}
		}
	}
	
	static void* static_main(void* ptr) {
		PImpl* self = (PImpl*) ptr;
		self->main();
		return 0;
	}
};

// Exposed class

ImageFinder::ImageFinder(): mImp(new PImpl()) { }

ImageFinder::~ImageFinder() {
	delete mImp; mImp = (PImpl*)0xdeaddead;
}

bool ImageFinder::pollCorners(ImageFinder::corners_t& corners, ImageFinder::transformation_matrix_t& pose) {
	return mImp->pollCorners(corners, pose);
}

void ImageFinder::setMarker(void* data, size_t width, size_t height, size_t bytesPerRow, size_t bytesPerPixel) {
	mImp->setMarker(data, width, height, bytesPerRow, bytesPerPixel);
}

void ImageFinder::addVideoFrame(void* data, size_t width, size_t height, size_t bytesPerRow, const ImageFinder::transformation_matrix_t& pose) {
	mImp->addVideoFrame(data, width, height, bytesPerRow, pose);
}

