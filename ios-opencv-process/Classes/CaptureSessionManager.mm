#include <algorithm>
#include <iostream>
#include <sstream>
#import "CaptureSessionManager.h"
#import "SaveImageAsPng.h"


static int runs=0;

struct MotionVectorFiller: public Tracker::IMotionVectorVisitor {
	std::vector<CGPoint> points;
	int size;
	MotionVectorFiller(): points(1000), size(0) { }
	void reset() { size=0; }
	void operator()(const CvPoint2D32f& origin, const CvPoint2D32f& end) {
		const CGSize screenSize = UIScreen.mainScreen.bounds.size;
		const float width(screenSize.width), height(screenSize.height);

		points[size].x = width  * (1. - (origin.y / 160.));
		points[size].y = height * (origin.x / 212.);
		++size;

		points[size].x = width  * (1. - (end.y / 160.));
		points[size].y = height * (end.x / 212.);
		++size;
	}
};

MotionVectorFiller filler;
unsigned char* downscaled = 0;

@implementation CaptureSessionManager
@synthesize sensorManager;
@synthesize captureSession;
@synthesize previewLayer;
@synthesize overlayLayer;
@synthesize tracker;
@synthesize planeExtractor;

- (void)drawLayer:(CALayer *)layer inContext:(CGContextRef)context {
	CGContextSetLineWidth(context, 3.0);
	CGContextSetRGBStrokeColor(context, 1.0, 0.5, 0.0, 0.5);
	CGContextStrokeLineSegments(context, &filler.points[0], filler.size);
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
#if 0 // Save input frame to disk...
	{
	CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    CVPixelBufferLockBaseAddress(imageBuffer,0);
		uint8_t *baseAddress = (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer);
		size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
		size_t width = CVPixelBufferGetWidth(imageBuffer);
		size_t height = CVPixelBufferGetHeight(imageBuffer);
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		CGContextRef newContext = CGBitmapContextCreate(baseAddress, width, height, 8, bytesPerRow, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
		CGImageRef newImage = CGBitmapContextCreateImage(newContext);
		CGContextRelease(newContext);
		CGColorSpaceRelease(colorSpace);
	CVPixelBufferUnlockBaseAddress(imageBuffer,0);
	UIImage *image= [UIImage imageWithCGImage:newImage scale:1.0 orientation:UIImageOrientationRight];
	CGImageRelease(newImage);
	NSData* png = UIImagePNGRepresentation(image);
	[image release];
	static int frame=0;
	std::stringstream fname;
	fname << getenv("TMPDIR") << "/image_" << (++frame) << ".png";
	NSString* s = [[NSString alloc] initWithUTF8String:fname.str().c_str()];
	[png writeToFile:s atomically:YES];
	[s release];
	[png release];
	}
#endif
	
	// buffer is 640x480, 32-bit BGRA.
	CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer( sampleBuffer );
	CVPixelBufferLockBaseAddress( pixelBuffer, 0 );
	tracker->onFrame(CVPixelBufferGetBaseAddress(pixelBuffer));
	++runs;
	CVPixelBufferUnlockBaseAddress( pixelBuffer, 0 );
	
	// ..and then save the results
	filler.reset();
	tracker->visitMotionVectors(filler);
}

- (void)deviceToWorldRotation:(const float*)rotation inclination:(const float*)inclination {
#if 0
	std::cerr << "--------------\n";
	std::cerr << "rotation:\n";
	int offset=-1;
	for (int y=0; y<4; ++y) {
		for (int x=0; x<4; ++x) {
			std::cerr << rotation[++offset] << "\t";
		}
		std::cerr << "\n";
	}
	std::cerr << "inclination:\n";
	offset=-1;
	for (int y=0; y<4; ++y) {
		for (int x=0; x<4; ++x) {
			std::cerr << inclination[++offset] << "\t";
		}
		std::cerr << "\n";
	}
#endif
#if 0	
	static int fiou=0;
	if (++fiou % 100 == 0) {
		fiou=0;
		std::cerr << 
		"azimuth=" << atan2f(rotation[1], rotation[5]) << ", " << 
		"pitch=" << asinf(-rotation[9]) << ", " << 
		"roll=" << atan2f(-rotation[8], rotation[10]) << ", " <<
		"inclination=" << atan2f(inclination[6], inclination[5]) << 
		"\n";
	}
#endif
}

- (id) init {
	if (self = [super init]) {
		// Create capture session...
		self.captureSession = [[AVCaptureSession alloc] init];
		self.captureSession.sessionPreset = AVCaptureSessionPreset640x480;
		
		// Create video device
		AVCaptureDevice *videoDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];	
		if ( videoDevice ) {
			NSError *error;
			// Lock autofocus
			if ([videoDevice lockForConfiguration:&error]) {
				[videoDevice setFocusMode:AVCaptureFocusModeLocked];
				[videoDevice unlockForConfiguration];
				// Get video input
				AVCaptureDeviceInput *videoIn = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error:&error];
				if ( !error ) {
					[self.captureSession addInput:videoIn];
				}
				else
					NSLog(@"Couldn't create video input");
			}
			else 
				NSLog(@"Couldn't disable autofocus");
		}
		else
			NSLog(@"Couldn't create video capture device");
		
		// Create output
		AVCaptureVideoDataOutput *videoOut = [[AVCaptureVideoDataOutput alloc] init];
		[videoOut setAlwaysDiscardsLateVideoFrames:YES];
		[videoOut setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]]; // BGRA is necessary for manual preview
		dispatch_queue_t my_queue = dispatch_queue_create("com.tonchidot.sekaicamera.opencvtest", NULL);
		[videoOut setSampleBufferDelegate:self queue:my_queue];
		[self.captureSession addOutput:videoOut];
		[videoOut release];
		
		// Create sensors
		self.sensorManager = [[SensorManager alloc] init];
		[self.sensorManager setDelegate:self];
		// Create tracker
		self.tracker = new Tracker();
		self.planeExtractor = new PlaneExtractor();
		
		downscaled = new unsigned char[320*240];
		
		// Setup layers
		self.previewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:self.captureSession];
		self.previewLayer.videoGravity = AVLayerVideoGravityResizeAspectFill;
		self.overlayLayer = [CALayer layer];
		self.overlayLayer.delegate = self;
	}
	return self;
}

- (void)dealloc {
	std::cerr << runs << " runs\n";
	delete[] downscaled;
	[self.captureSession stopRunning];
	[self.sensorManager release];
	delete self.tracker;
	delete self.planeExtractor;
	[self.previewLayer release];
	[self.overlayLayer release];
	[self.captureSession release];
	[super dealloc];
}
@end
