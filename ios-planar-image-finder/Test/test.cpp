#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <iostream>
#include <vector>
#include <fstream>
#include "ImageFinder.h"

using namespace std;

void flannFindPairs( const CvSeq*, const CvSeq* objectDescriptors,
           const CvSeq*, const CvSeq* imageDescriptors, vector<int>& ptpairs ) {
	int length = (int)(objectDescriptors->elem_size/sizeof(float));

    cv::Mat m_object(objectDescriptors->total, length, CV_32F);
	cv::Mat m_image(imageDescriptors->total, length, CV_32F);


	// copy descriptors
    CvSeqReader obj_reader;
	float* obj_ptr = m_object.ptr<float>(0);
    cvStartReadSeq( objectDescriptors, &obj_reader );
    for(int i = 0; i < objectDescriptors->total; i++ )
    {
        const float* descriptor = (const float*)obj_reader.ptr;
        CV_NEXT_SEQ_ELEM( obj_reader.seq->elem_size, obj_reader );
        memcpy(obj_ptr, descriptor, length*sizeof(float));
        obj_ptr += length;
    }
	
    CvSeqReader img_reader;
	float* img_ptr = m_image.ptr<float>(0);
    cvStartReadSeq( imageDescriptors, &img_reader );
    for(int i = 0; i < imageDescriptors->total; i++ )
    {
        const float* descriptor = (const float*)img_reader.ptr;
        CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );
        memcpy(img_ptr, descriptor, length*sizeof(float));
        img_ptr += length;
    }

    // find nearest neighbors using FLANN
    cv::Mat m_indices(objectDescriptors->total, 2, CV_32S);
    cv::Mat m_dists(objectDescriptors->total, 2, CV_32F);
    cv::flann::Index flann_index(m_image, cv::flann::KDTreeIndexParams(4));  // using 4 randomized kdtrees
    flann_index.knnSearch(m_object, m_indices, m_dists, 2, cv::flann::SearchParams(64) ); // maximum number of leafs checked

	std::ofstream dfs;
	dfs.open("opencv_dists.txt");
	if (dfs) {
		dfs.precision(20);
		int* indices_ptr = m_indices.ptr<int>(0);
		float* dists_ptr = m_dists.ptr<float>(0);
		for (int i=0;i<m_indices.rows;++i) {
			const float& aDist(dists_ptr[2*i]);
			const float& bDist(dists_ptr[2*i+1]);
			dfs << aDist << " " << bDist << "\n";
			if (aDist<0.6*bDist) {
				ptpairs.push_back(i);
				ptpairs.push_back(indices_ptr[2*i]);
			}
		}
		dfs.close();
	}
}


/* a rough implementation for object location */
int
locatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    const CvPoint src_corners[4], CvPoint dst_corners[4] )
{
    double h[9];
    CvMat _h = cvMat(3, 3, CV_64F, h);
    vector<int> ptpairs;
    vector<CvPoint2D32f> pt1, pt2;
    CvMat _pt1, _pt2;
    int i, n;

    flannFindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
	
	{
		std::ofstream ofs;
		ofs.open("testPairs.h");
		if (ofs) {
			ofs.precision(20);
			ofs << "static const int cvPairsCount = " << ptpairs.size()/2 << ";\n";
			ofs << "static const int cvPairs[] = {\n";
			for (int i(0); i<ptpairs.size(); i+=2) ofs << "\t" << ptpairs[i] << ", " << ptpairs[i+1] << ",\n";
			ofs << "\t-1, -1\n};\n";
			ofs.close();
		}
	}

    n = ptpairs.size()/2;
    if( n < 4 )
        return 0;

    pt1.resize(n);
    pt2.resize(n);
    for( i = 0; i < n; i++ )
    {
        pt1[i] = ((CvSURFPoint*)cvGetSeqElem(objectKeypoints,ptpairs[i*2]))->pt;
        pt2[i] = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,ptpairs[i*2+1]))->pt;
    }

    _pt1 = cvMat(1, n, CV_32FC2, &pt1[0] );
    _pt2 = cvMat(1, n, CV_32FC2, &pt2[0] );
    if( !cvFindHomography( &_pt1, &_pt2, &_h, CV_RANSAC, 5 ))
        return 0;

    for( i = 0; i < 4; i++ )
    {
        double x = src_corners[i].x, y = src_corners[i].y;
        double Z = 1./(h[6]*x + h[7]*y + h[8]);
        double X = (h[0]*x + h[1]*y + h[2])*Z;
        double Y = (h[3]*x + h[4]*y + h[5])*Z;
        dst_corners[i] = cvPoint(cvRound(X), cvRound(Y));
    }

    return 1;
}

void writeSURFHeader(const char* filename, CvSeq* keypoints, CvSeq* descriptors) {
	std::ofstream ks, ds;
	{ std::stringstream x; x << filename << "_keypoints.h"; ks.open(x.str().c_str()); };
	{ std::stringstream x; x << filename << "_descriptors.h"; ds.open(x.str().c_str()); };
	if (ks) {
		ks.precision(20);
		ks << "static const int " << filename << "KeypointCount = " << keypoints->total << ";\n";
		ks << "static const CvSURFPoint " << filename << "Keypoints[] = {\n";
		CvSeqReader reader;
		cvStartReadSeq( keypoints, &reader );
		const int count(keypoints->total);
		for(int i(0); i<count; ++i) {
			const CvSURFPoint& point = *((const CvSURFPoint*)reader.ptr);
			CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
			ks << "\t{{" << point.pt.x << "f," << point.pt.y << "f},"
			<< point.laplacian << "," << point.size << "," << point.dir << "f," << point.hessian << "f}";
			if (i+1<count) ks << ",";
			ks << "\n";
		}
		ks << "};\n";
		ks.close();
	}
	if (ds) {
		ds.precision(20);
		ds << "static const int " << filename << "DescriptorCount = " << descriptors->total << ";\n";
		ds << "static const float " << filename << "Descriptors[] = {\n";
		CvSeqReader reader;
		cvStartReadSeq( descriptors, &reader );
		const int count(descriptors->total);
		for(int i(0); i<count; ++i) {
			const float* vec = (const float*)reader.ptr;
			CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
			for (int j(0); j<64; ++j) {
				ds << vec[j] << (((j+1)%4)?"f,":"f,\n");
			}
		}
		ds << "0.f};\n";
		ds.close();
	}
}

void saveImage(const IplImage* img, const char* name, const char* orig) {
	std::stringstream filename;
	filename << name << "Data.h";
	std::ofstream os;
	os.open(filename.str().c_str());
	if (os) {
		unsigned int checksum(0);
		const unsigned char* data = (const unsigned char*) img->imageData;
		os << "static const char " << name << "Filename[] = \"" << orig << "\";\n";
		os << "static const int " << name << "Width = " << img->width << ";\n";
		os << "static const int " << name << "BytesPerRow = " << img->widthStep << ";\n";
		os << "static const int " << name << "Height = " << img->height << ";\n";
		os << "static const unsigned char " << name << "Data[] = {\n";
		for (unsigned int y(0); y<img->height; ++y) {
			for (unsigned int x(0); x<img->widthStep; ++x) {
				unsigned int pixel = data[y*img->widthStep + x];
				if (x<img->width) checksum+=pixel;
				os << pixel << ",";
				if (x%32==0) os << "\n";
			}
			os << "\n";
		}
		os << "};\n";
		os << "static const unsigned int " << name << "Checksum = " << checksum << ";\n";
		os.close();
	}
}

int main(int argc, const char* argv[]) {
	if (argc!=3) {
		std::cerr << "Usage: " << argv[0] << " <pattern> <scene>\n";
		return 1;
	}

	IplImage* object = cvLoadImage( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	IplImage* image  = cvLoadImage( argv[2], CV_LOAD_IMAGE_GRAYSCALE );
	if( !object || !image ) {
		std::cerr << "Failed to open images.\n";
		return 2;
	}
	saveImage(object, "testObject", argv[1]);
	saveImage(image,  "testImage",  argv[2]);

	CvMemStorage* storage = cvCreateMemStorage(0);

	CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
	cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, cvSURFParams(2500, 0) );
	std::cerr << "cvExtractSURF(object): " << objectKeypoints->total << " keypoints, " << objectDescriptors->total << " descriptors.\n";
	writeSURFHeader("testObject", objectKeypoints, objectDescriptors);

	CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
	cvExtractSURF( image,  0, &imageKeypoints,  &imageDescriptors,  storage, cvSURFParams(2500, 0) );
	std::cerr << "cvExtractSURF(image): " << imageKeypoints->total << " keypoints, " << imageDescriptors->total << " descriptors.\n";
	writeSURFHeader("testImage", imageKeypoints, imageDescriptors);

	CvPoint src_corners[4] = {{0,0}, {object->width,0}, {object->width, object->height}, {0, object->height}};
	CvPoint dst_corners[4];

	locatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, src_corners, dst_corners );

	std::cerr << "Found with OpenCV: [" 
	<< dst_corners[0].x << ", " << dst_corners[0].y << "]   ["
	<< dst_corners[1].x << ", " << dst_corners[1].y << "]   ["
	<< dst_corners[2].x << ", " << dst_corners[2].y << "]   ["
	<< dst_corners[3].x << ", " << dst_corners[3].y << "]\n";
	
	float ax(0), ay(0);
	for (int i(0); i<4; ++i) {
		ax+=dst_corners[i].x;
		ay+=dst_corners[i].y;
	}
	ax/=4.f; ay/=4.f;
	std::cerr << "…average position = [" << ax << ", " << ay << "]\n";

	ImageFinder finder;
	finder.setImageToFind(object->imageData, object->width, object->height, object->widthStep);
	finder.findInFrame(image->imageData);
	float fx[4], fy[4];
	for (int i(0); i<4; ++i) finder.getCorner(i, fx[i], fy[i]);

	std::cerr << "Found with ImageFinder: ["
	<< (int) fx[0] << ", " << (int) fy[0] << "]   ["
	<< (int) fx[1] << ", " << (int) fy[1] << "]   ["
	<< (int) fx[2] << ", " << (int) fy[2] << "]   ["
	<< (int) fx[3] << ", " << (int) fy[3] << "]\n";
}

