/*
 *  Tracker.h
 *  Process
 *
 *  Created by Cayzac Julien on 9/2/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#pragma once

#define CV_NO_BACKWARD_COMPATIBILITY
#include <opencv2/opencv.hpp>
#include <vector>

class Tracker {
public:
	struct IMotionVectorVisitor {
		virtual void operator()(const CvPoint2D32f& origin, const CvPoint2D32f& end)=0;
	};
public:
	Tracker();
	~Tracker();
	void onFrame(void* data);
	void visitMotionVectors(IMotionVectorVisitor& visitor) const {
		for (int i=0; i<mPreviousFeatureCount; ++i) {
			if (mStatus[i]!=0 && mFoundFeatures[i].x>0 && mFoundFeatures[i].y>0)
				visitor(mPreviousFeatures[i], mFoundFeatures[i]);
		}
	}
	const IplImage* grey() const { return mCurrentImage; }
	const IplImage* debug() const { return mDebugImage; }
private:
	// Tracker is non-copyable
	Tracker(const Tracker&);
	Tracker& operator=(const Tracker&);
private:
	IplImage* mDebugImage;
	IplImage* mCurrentImage;
	IplImage* mPreviousImage;
	IplImage* mEigenImage;
	IplImage* mTempImage;
	IplImage* mCurrentPyramid;
	IplImage* mPreviousPyramid;
	CvPoint2D32f* mCurrentFeatures;
	CvPoint2D32f* mPreviousFeatures;
	CvPoint2D32f* mFoundFeatures;
	char* mStatus;
	CvMemStorage* mStorage;
	CvTermCriteria mTermCrit;
	int mCurrentFeatureCount;
	int mPreviousFeatureCount;
	int mOpticalFlowFlags;
	bool mFirstRun;
};
