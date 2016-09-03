/*
 *  PlaneExtractor.cpp
 *  Process
 *
 *  Created by Cayzac Julien on 9/7/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#define CV_NO_BACKWARD_COMPATIBILITY
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>

#include "PlaneExtractor.h"
#include "SaveImageAsPng.h"

#define MAX_FEATURES 400

struct PlaneExtractor::PImp {
	IplImage* grey;
	IplImage* eig;
	IplImage* tmp;
	CvPoint2D32f features[MAX_FEATURES];
	int featureCount;

	PImp():featureCount(0) {
		grey = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 1);
		eig  = cvCreateImage(cvSize(320, 240), IPL_DEPTH_32F, 1);
		tmp  = cvCreateImage(cvSize(320, 240), IPL_DEPTH_32F, 1);
	}
	~PImp() {
		cvReleaseImage(&grey);
		cvReleaseImage(&eig);
		cvReleaseImage(&tmp);
	}
};

static inline unsigned int bgra2grey(const unsigned char* bgra) {
	return (((unsigned int) bgra[0]) + (((unsigned int) bgra[1])<<1) + ((unsigned int) bgra[2]))>>2;
}

static inline unsigned int divideBy9(const unsigned int n) {
	unsigned int q(n - (n >> 3));
	q = q + (q >> 6);
	q = q + (q>>12) + (q>>24);
	q = q >> 3;
	unsigned int r(n - q*9);
	return q + ((r + 7) >> 4);
}

PlaneExtractor::PlaneExtractor(): mPimp(0) { mPimp = new PImp; }
PlaneExtractor::~PlaneExtractor() { delete mPimp; }

void PlaneExtractor::addImage(unsigned int width, unsigned int height, unsigned int bytesPerRow, unsigned int bytesPerPixel, unsigned char* data) {
	assert(width==640);
	assert(height==480);
	assert(bytesPerPixel==4);
	
	// Build L0 from source image, performing the following filters:
	// 1- Convert to greyscale
	// 2- Rescale from 640x320 to 320x240 linearly
	IplImage* img = mPimp->grey;
	unsigned char* imgData = (unsigned char*) img->imageData;
	unsigned int offset=0;
	unsigned int dst=0;
	for (unsigned int y=0; y<240; ++y, dst+=img->widthStep) {
		unsigned char* line1 = &data[offset]; offset+=bytesPerRow;
		unsigned char* line2 = &data[offset]; offset+=bytesPerRow;
		for (unsigned int x=0; x<(320<<3); x+=8) {
			unsigned int sum = 0;
			sum+=(((unsigned int) line1[x]) + (((unsigned int) line1[x+1])<<1) + ((unsigned int) line1[x+2]))>>2;
			sum+=(((unsigned int) line2[x]) + (((unsigned int) line2[x+1])<<1) + ((unsigned int) line2[x+2]))>>2;
			sum+=(((unsigned int) line1[x+4]) + (((unsigned int) line1[x+5])<<1) + ((unsigned int) line1[x+6]))>>2;
			sum+=(((unsigned int) line2[x+4]) + (((unsigned int) line2[x+5])<<1) + ((unsigned int) line2[x+6]))>>2;
			imgData[dst+(x>>3)] = sum>>2;
		}
	}
	
	mPimp->featureCount=MAX_FEATURES;
	cvGoodFeaturesToTrack(mPimp->grey, mPimp->eig, mPimp->tmp, mPimp->features, &mPimp->featureCount, .01, 10, 0);
	
#if 1
	static int frame=0; ++frame;
	{
		std::stringstream fname;
		fname << getenv("TMPDIR") << "/" << frame << ".png";
		IplImage* img = mPimp->grey;
		SaveImageAsPNG saver;
		saver(fname.str(), (unsigned char*) img->imageData, img->width, img->height, 1, img->widthStep, SaveImageAsPNG::COLORSPACE_GREY);
	}
#endif
	
	
	
	
}
