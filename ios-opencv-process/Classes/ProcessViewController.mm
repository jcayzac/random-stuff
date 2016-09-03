#import "ProcessViewController.h"

@implementation ProcessViewController
@synthesize captureManager;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {

	CGPoint touchPoint = [[touches anyObject] locationInView:self.view];
	NSLog(@"Touch at %d, %d", (int) touchPoint.x, (int) touchPoint.y);
}


- (void)viewDidLoad {
	captureManager = [[CaptureSessionManager alloc] init];
	
	CGRect layerRect = self.view.layer.bounds;
	captureManager.previewLayer.bounds = layerRect;
	captureManager.previewLayer.position = CGPointMake(CGRectGetMidX(layerRect), CGRectGetMidY(layerRect));
	[self.view.layer addSublayer:captureManager.previewLayer];
	
	captureManager.overlayLayer.frame = layerRect;
    captureManager.overlayLayer.backgroundColor = [[UIColor clearColor] CGColor];
    [self.view.layer addSublayer:captureManager.overlayLayer];
	
	[captureManager.captureSession startRunning];
	[NSTimer scheduledTimerWithTimeInterval:0.02 target:captureManager.overlayLayer selector:@selector(setNeedsDisplay) userInfo:nil repeats:YES];

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}


- (void)dealloc {
    [super dealloc];
}

@end
