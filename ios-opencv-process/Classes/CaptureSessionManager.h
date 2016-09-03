#include "Tracker.h"
#include "PlaneExtractor.h"
#import "SensorManager.h"
#import <CoreMedia/CoreMedia.h>
#import <AVFoundation/AVFoundation.h>

@interface CaptureSessionManager : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate, ARSensorDelegate, CALayerDelegate> {
	Tracker* tracker;
	PlaneExtractor* planeExtractor;
	SensorManager* sensorManager;
	AVCaptureSession *captureSession;
	AVCaptureVideoPreviewLayer *previewLayer;
	CALayer *overlayLayer;
	AVCaptureConnection *videoConnection;
}

@property (retain) AVCaptureVideoPreviewLayer *previewLayer;
@property (retain) CALayer *overlayLayer;
@property (retain) AVCaptureSession *captureSession;
@property (retain) SensorManager *sensorManager;
@property (readwrite) Tracker *tracker;
@property (readwrite) PlaneExtractor *planeExtractor;
@end
