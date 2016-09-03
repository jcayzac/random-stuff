//
//  Planar_Image_FinderViewController.h
//  Planar Image Finder
//
//  Created by Cayzac Julien on 9/24/10.
//  Copyright 2010 -. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/CADisplayLink.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import <AVFoundation/AVFoundation.h>
#import <CoreMotion/CoreMotion.h>

#include "VideoQuad.h"
#include "ImageFinder.h"
#include "ContourQuad.h"

@interface Planar_Image_FinderViewController : UIViewController<AVCaptureVideoDataOutputSampleBufferDelegate>
{
	EAGLContext *context;
	CADisplayLink *displayLink;
	AVCaptureSession *captureSession;
	CMMotionManager *motionManager;
	
	ImageFinder* imageFinder;
	ImageFinder::corners_t lastCorners;
	ImageFinder::transformation_matrix_t lastPose;
	float lastSpeed[3];
	float lastTime;

	VideoQuad* videoQuad;
	ContourQuad* contourQuad;
}

@property (nonatomic, strong) AVCaptureSession *captureSession;
@property (nonatomic, strong) EAGLContext *context;
@property (nonatomic, strong) CMMotionManager *motionManager;

- (void)startAnimation;
- (void)stopAnimation;
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection;

@end
