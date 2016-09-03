//
//  Planar_Image_FinderViewController.m
//  Planar Image Finder
//
//  Created by Cayzac Julien on 9/24/10.
//  Copyright 2010 -. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "Planar_Image_FinderViewController.h"
#import "EAGLView.h"
#include <iostream>
#include <vector>
#include <cmath>
#define DEG2RAD(x) (x*(M_PI/180.f))
#define CONTENT_OF(base, ext) [[NSString stringWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@base ofType:@ext] encoding:NSUTF8StringEncoding error:nil] UTF8String]
@implementation Planar_Image_FinderViewController
@synthesize context, captureSession, motionManager;


- (void)awakeFromNib {
	NSLog(@"awakeFromNib");
	EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	if (!aContext)
		NSLog(@"Failed to create ES context");
	else if (![EAGLContext setCurrentContext:aContext])
		NSLog(@"Failed to set ES context current");
	
	self.context = aContext;
	
	EAGLView* glView = (EAGLView *)self.view;
	
	[glView setContext:context];
	[glView setFramebuffer];
	
	videoQuad = new VideoQuad();
	imageFinder = new ImageFinder();
	contourQuad = new ContourQuad();
	
	CGImageRef imageToFind = [[UIImage imageNamed:@"ManRayPoster.png"] CGImage];
	CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(imageToFind));
	NSAssert(8==CGImageGetBitsPerComponent(imageToFind), @"Wrong BPC");
	NSAssert(8==CGImageGetBitsPerPixel(imageToFind), @"Wrong BPP");
	imageFinder->setMarker((void*) CFDataGetBytePtr(dataRef),
						   (size_t) CGImageGetWidth(imageToFind),
						   (size_t) CGImageGetHeight(imageToFind),
						   (size_t) CGImageGetBytesPerRow(imageToFind),
						   1);
	CFRelease(dataRef);

	displayLink = nil;

	// Create video session
	captureSession = [[AVCaptureSession alloc] init];
	captureSession.sessionPreset = AVCaptureSessionPreset640x480;

	AVCaptureDevice* camera = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];

	NSError* error;
	AVCaptureDeviceInput* videoIn = [AVCaptureDeviceInput deviceInputWithDevice:camera error:&error];
	[captureSession addInput:videoIn];
	
	AVCaptureVideoDataOutput* videoOut = [[AVCaptureVideoDataOutput alloc] init];
	[videoOut setAlwaysDiscardsLateVideoFrames:YES];
	[videoOut setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
	[videoOut setSampleBufferDelegate:self queue:dispatch_queue_create("com.tonchidot.pif", 0)];
	[captureSession addOutput:videoOut];
	
	motionManager = [[CMMotionManager alloc] init];

	[super awakeFromNib];
}

- (void)dealloc {
	[captureSession stopRunning];
	
	[motionManager stopDeviceMotionUpdates];
	
	delete videoQuad;		videoQuad=(VideoQuad*)0xdeaddead;
	delete imageFinder;		imageFinder=(ImageFinder*)0xdeaddead;
	delete contourQuad;		contourQuad=(ContourQuad*)0xdeaddead;
	
	// Tear down context.
	if ([EAGLContext currentContext] == context)
		[EAGLContext setCurrentContext:nil];
	
	
}
- (void)viewWillAppear:(BOOL)animated {
	[self startAnimation];
	[super viewWillAppear:animated];
}
- (void)viewWillDisappear:(BOOL)animated {
	[self stopAnimation];
	[super viewWillDisappear:animated];
}
- (void)startAnimation {
	if (!displayLink) {
		displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawFrame)];
		[displayLink setFrameInterval:1];
		[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		motionManager.deviceMotionUpdateInterval = 1.f/60.f;
		[motionManager startDeviceMotionUpdates];
		memset(lastSpeed, 0, 3*sizeof(float));
		lastTime=-1.f;
		[captureSession startRunning];
	}
}
- (void)stopAnimation {
	if (displayLink) {
		[captureSession stopRunning];
		[motionManager stopDeviceMotionUpdates];
		[displayLink invalidate];
		displayLink = nil;
	}
}
- (void)drawFrame {
	EAGLView* glView = (EAGLView*)self.view;
	[glView setFramebuffer];
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	@synchronized(self) {
		videoQuad->draw();
	}
	
	contourQuad->draw();
	
	glFlush();
	[glView presentFramebuffer];
}
- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning];
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
	(void)captureOutput;
	(void)connection;
	
	// Get corners and reference pose
	
	ImageFinder::corners_t newCorners;
	ImageFinder::transformation_matrix_t cornersPose;
	bool found = imageFinder->pollCorners(newCorners, cornersPose);
	if (found) {
		accel::MemCopy(lastCorners, newCorners, sizeof(ImageFinder::corners_t));
		accel::MemCopy(lastPose, cornersPose, sizeof(ImageFinder::transformation_matrix_t));
		// TODO: reset position vector
	}
	
	// Get current pose
	CMDeviceMotion* motion = motionManager.deviceMotion;
	CMAcceleration accel = motion.userAcceleration;
	
	struct timeval tv;
	gettimeofday(&tv, 0);
	const float now(float(tv.tv_sec) + float(tv.tv_usec>>10)*1.024f);
	const float elapsed(lastTime<0.f?0.f:now-lastTime);
	lastTime = now;
	const float eg(elapsed*.10193679918450560652f);
	const float axt(float(accel.x)*eg);
	const float ayt(float(accel.y)*eg);
	const float azt(float(accel.z)*eg);
	const float px(lastPose[ 3] + elapsed*(lastSpeed[0] + .5f*axt));
	const float py(lastPose[ 7] + elapsed*(lastSpeed[1] + .5f*ayt));
	const float pz(lastPose[11] + elapsed*(lastSpeed[2] + .5f*azt));
	lastSpeed[0] += axt;
	lastSpeed[1] += ayt;
	lastSpeed[2] += azt;
	
	CMAttitude* attitude = motion.attitude;
	CMRotationMatrix rot=attitude.rotationMatrix;
	ImageFinder::transformation_matrix_t currentPose = { 
		(float) rot.m11, (float) rot.m21, (float) rot.m31, px,
		(float) rot.m12, (float) rot.m22, (float) rot.m32, py,
		(float) rot.m13, (float) rot.m23, (float) rot.m33, pz,
		0.f,             0.f,             0.f,             1.f
	};
	
//	NSLog(@"[ax, ay, az] = [%f, %f, %f]", float(accel.x), float(accel.y), float(accel.z));
//	NSLog(@"[px, py, pz] = [%f, %f, %f]", px, py, pz);

	// Lock video buffer
	CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer( sampleBuffer );
	CVPixelBufferLockBaseAddress( pixelBuffer, 0 );
	const unsigned int* base = (const unsigned int*) CVPixelBufferGetBaseAddress(pixelBuffer);

	// Upload frame and current pose to ImageFinder (non-blocking)
	imageFinder->addVideoFrame((void*) base,640,480,640*4, currentPose);

	// Upload frame to GL
	EAGLView* glView = (EAGLView*)self.view;
	[EAGLContext setCurrentContext:[glView context]];
	@synchronized(self) {
		videoQuad->updateTexture((const unsigned char*) base);
	}
	
	// Unlock video buffer
	CVPixelBufferUnlockBaseAddress( pixelBuffer, 0);

	// Compute pose difference...
	// ImageFinder::transformation_matrix_t delta, invRef;
	// invRef = inv(lastPose)
	// mat44_multiply(delta, currentPose, invRef)
	
	
	
	// Update corners
	contourQuad->updateCorners(lastCorners[0].x, lastCorners[0].y,
							   lastCorners[1].x, lastCorners[1].y, 
							   lastCorners[2].x, lastCorners[2].y,
							   lastCorners[3].x, lastCorners[3].y);
	
}

@end
