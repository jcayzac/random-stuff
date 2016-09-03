/*
 *  Tracker.cpp
 *  Process
 *
 *  Created by Cayzac Julien on 9/2/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#include "Tracker.h"
#include "SaveImageAsPng.h"
#include "fast.h"

#define MAX_FEATURES 100
#define WINDOW_WIDTH 3

#include <sstream>

#ifdef DEBUG_TRACKING
#include <iostream>
#define DEBUG_OUTPUT(x) { std::cerr << x << "\n"; }
#else
#define DEBUG_OUTPUT(x)
#endif


static inline unsigned int divideBy9(const unsigned int n) {
	unsigned int q(n - (n >> 3));
	q = q + (q >> 6);
	q = q + (q>>12) + (q>>24);
	q = q >> 3;
	unsigned int r(n - q*9);
	return q + ((r + 7) >> 4);
}

static inline unsigned int luminance(unsigned char* line, int srcOff) {
	return (((unsigned int) line[srcOff]) + (((unsigned int) line[srcOff+1])<<1) + ((unsigned int) line[srcOff+2]))>>2;
}

#if 1
#define WIDTH  212
#define HEIGHT 160
static void buildDownscaledLuminanceMap(unsigned char* dst, unsigned char* src, unsigned int dstStep) {
	unsigned int srcIndex=640*4;
	unsigned int dstIndex=0;
	for (unsigned int y=0; y<HEIGHT-1; ++y, dstIndex+=dstStep, srcIndex+=640*4*3) {
		unsigned char* line = &src[srcIndex];
        if (!y) continue;
		int srcOff=0;
		for (unsigned int x=0; x<WIDTH; ++x, srcOff+=3*4) {
			unsigned int sum=0;
			sum += luminance(line, srcOff - (640*4 - 1));
			sum += luminance(line, srcOff - (640*4));
			sum += luminance(line, srcOff - (640*4 + 1));
			sum += luminance(line, srcOff - 1);
			sum += luminance(line, srcOff);
			sum += luminance(line, srcOff + 1);
			sum += luminance(line, srcOff + (640*4 - 1));
			sum += luminance(line, srcOff + (640*4));
			sum += luminance(line, srcOff + (640*4 + 1));
			dst[dstIndex+x] = divideBy9(sum);
		}
	}
}
#else
#define WIDTH  320
#define HEIGHT 240
static void buildDownscaledLuminanceMap(unsigned char* dst, unsigned char* src, unsigned int dstStep) {
	unsigned int srcIndex=640*4;
	unsigned int dstIndex=0;
	for (unsigned int y=0; y<240; ++y, dstIndex+=dstStep) {
		unsigned char* line1 = &src[srcIndex]; srcIndex+=640*4;
		unsigned char* line2 = &src[srcIndex]; srcIndex+=640*4;
		for (unsigned int x=0; x<(320<<3); x+=8) {
			unsigned int sum = 0;
			sum+=luminance(line1, x);
			sum+=luminance(line1, x+4);
			sum+=luminance(line2, x);
			sum+=luminance(line2, x+4);
			dst[dstIndex+(x>>3)] = sum>>2;
		}
	}
}

#endif

Tracker::Tracker()
: mCurrentImage(0)
, mPreviousImage(0)
, mEigenImage(0)
, mTempImage(0)
, mCurrentFeatures(0)
, mPreviousFeatures(0)
, mStorage(0)
, mStatus(0)
, mCurrentFeatureCount(0)
, mPreviousFeatureCount(0)
, mOpticalFlowFlags(0)
, mFirstRun(true)
, mDebugImage(0)
{
	mCurrentImage     = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
	mPreviousImage    = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
	mEigenImage       = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	mTempImage        = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	mCurrentPyramid   = cvCreateImage(cvSize(WIDTH+8, HEIGHT/3), IPL_DEPTH_8U, 1);
	mPreviousPyramid  = cvCreateImage(cvSize(WIDTH+8, HEIGHT/3), IPL_DEPTH_8U, 1);
	mCurrentFeatures  = new CvPoint2D32f[MAX_FEATURES];
	mFoundFeatures    = new CvPoint2D32f[MAX_FEATURES];
	mPreviousFeatures = new CvPoint2D32f[MAX_FEATURES];
	mStatus = new char[MAX_FEATURES];
	mStorage          = cvCreateMemStorage(0);
	mTermCrit = cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 64, 0.01); // FIXME: 64 iterations may be too much
	mDebugImage = cvCreateImage(cvSize(WIDTH, 160), IPL_DEPTH_8U, 1);
}
Tracker::~Tracker() {
	cvReleaseImage(&mCurrentImage);
	cvReleaseImage(&mPreviousImage);
	cvReleaseImage(&mEigenImage);
	cvReleaseImage(&mTempImage);
	cvReleaseImage(&mCurrentPyramid);
	cvReleaseImage(&mPreviousPyramid);
	cvReleaseMemStorage(&mStorage);
	delete[] mCurrentFeatures;
	delete[] mFoundFeatures;
	delete[] mPreviousFeatures;
	delete[] mStatus;
	cvReleaseImage(&mDebugImage);
}
void Tracker::onFrame(void* data) {
	std::swap(mCurrentImage, mPreviousImage);
	std::swap(mCurrentFeatures, mPreviousFeatures);
	std::swap(mCurrentFeatureCount, mPreviousFeatureCount);
	std::swap(mCurrentPyramid, mPreviousPyramid);
	
	// Fill mCurrentImage with a 8-bit 212x160 luminance map.
	// Assumes a 640x480 32-bit bgra original frame.
	buildDownscaledLuminanceMap((unsigned char*) mCurrentImage->imageData, (unsigned char*) data, mCurrentImage->widthStep);
	
#if 0
	static int frame=0; ++frame;
	{
		std::stringstream fname;
		fname << getenv("TMPDIR") << "/" << frame << ".png";
		SaveImageAsPNG saver;
		saver(fname.str(), (unsigned char*) mCurrentImage->imageData, mCurrentImage->width, mCurrentImage->height, 1, mCurrentImage->widthStep, SaveImageAsPNG::COLORSPACE_GREY);
	}
#endif
	
	// Find corners using the FAST detector...
	int num_corners=0;
	xy* corners = fast9_detect_nonmax((unsigned char*) mCurrentImage->imageData, mCurrentImage->width, mCurrentImage->height, mCurrentImage->width, 20, &num_corners);
	std::cerr << "FAST: found " << num_corners << " corners.\n";
	for (int i=0; i<num_corners; ++i) {
		mCurrentFeatures[i].x = corners[i].x;
		mCurrentFeatures[i].y = corners[i].y;		
	}
	mCurrentFeatureCount=num_corners;

/*
	// Find ST corners...
	mCurrentFeatureCount=MAX_FEATURES;
	cvGoodFeaturesToTrack(
		mCurrentImage,			// Image
		mEigenImage,			
		mTempImage,
		&mCurrentFeatures[0],	// Best detected corners
		&mCurrentFeatureCount,
		.1,					// Quality Level – Multiplier for the max/min eigenvalue; specifies the minimal accepted quality of image corners
		5.,						// Min Distance – Limit, specifying the minimum possible distance between the returned corners; Euclidian distance is used
		0
	);
 */
//	cvFindCornerSubPix( mCurrentImage, &mCurrentFeatures[0], mCurrentFeatureCount,
//					   cvSize(WINDOW_WIDTH,WINDOW_WIDTH), cvSize(-1,-1),
//					   cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
	
	if (mFirstRun) { mFirstRun=false; return; }
		
	// Compute the optical flow / motion vector field using the Kanade-Lucas-Tomasi algorithm...
	cvCalcOpticalFlowPyrLK(
		mPreviousImage,    mCurrentImage,
		mPreviousPyramid,  mCurrentPyramid,
		mPreviousFeatures, mFoundFeatures,
		mPreviousFeatureCount,
		cvSize(WINDOW_WIDTH,WINDOW_WIDTH),	// Window size (FIXME: too much wide window?)
		3,				// Max pyramid level. 0=no pyramid used.
		mStatus,		// mStatus[feature_idx] will be 1 if flow was found, 0 otherwise
		0,				// We don't need to track the relative error
		mTermCrit,		// 
		mOpticalFlowFlags
	);
	// Next time, we won't need to compute the first pyramid again...
	mOpticalFlowFlags|=CV_LKFLOW_PYR_A_READY;
	/*
	int valid=0;
	for (int i=0; i<mPreviousFeatureCount; ++i) {
		if (mStatus[i]) {
			++valid;
			if (mFoundFeatures[i].x<0.f) {
				std::cerr << ".";
			}
		}
	}
	*/
}
