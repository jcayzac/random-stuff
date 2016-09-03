#import "NSCViewController.h"
#import "NSCAppDelegate.h"
#import "PlaceOfInterest.h"
#import <AVFoundation/AVFoundation.h>

@interface NSCViewController ()
@property (strong, nonatomic) AVCaptureSession *session;
@property (strong, nonatomic) AVCaptureVideoPreviewLayer *previewLayer;
- (void)startCamera;
- (void)stopCamera;
@end

@implementation NSCViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
	[[UIApplication sharedApplication]setStatusBarHidden:YES withAnimation:UIStatusBarAnimationSlide];

	// up swipe gestuer
	UISwipeGestureRecognizer *upSwipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(didUpSwipeAnywhere:)];
	upSwipeGesture.direction = UISwipeGestureRecognizerDirectionUp;
	[self.view addGestureRecognizer:upSwipeGesture];
	// down swipe gestuer
	UISwipeGestureRecognizer *downSwipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(didDownSwipeAnywhere:)];
	downSwipeGesture.direction = UISwipeGestureRecognizerDirectionDown;
	[self.view addGestureRecognizer:downSwipeGesture];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[self startCamera];
	self.itemView.delegate = self;
	[self.theARView start];
}

- (void)viewDidDisappear:(BOOL)animated {
	[self stopCamera];
	[self.theARView stop];
	self.itemView.delegate = nil;
	[super viewDidDisappear:animated];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)startCamera {
	if (self.session) {
		[self stopCamera];
	}

	self.session = [[AVCaptureSession alloc] init];
	self.previewLayer = [AVCaptureVideoPreviewLayer layerWithSession:self.session];
	AVCaptureDeviceInput *input = [[AVCaptureDeviceInput alloc] initWithDevice:[AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo] error:nil];
	if (input) [self.session addInput:input];
	[self.session setSessionPreset:AVCaptureSessionPresetMedium];

	CGRect bounds = self.view.layer.bounds;
	self.previewLayer.videoGravity = AVLayerVideoGravityResizeAspectFill;
	self.previewLayer.bounds = bounds;
	self.previewLayer.position = CGPointMake(CGRectGetMidX(bounds), CGRectGetMidY(bounds));

	[self.view.layer insertSublayer:self.previewLayer atIndex:0];
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
	    [self.session startRunning];
	});
}

- (void)stopCamera {
	[self.session stopRunning];
	[self.previewLayer removeFromSuperlayer];
	self.session = nil;
}

- (void)itemListViewRefreshedItsContent {
	// update AR view
	//NSArray* labels = [NSArray arrayWithObjects:@"N", @"S", @"W", @"E", @"D", @"U", nil];
	NSArray *labels = @[@"A", @"B", @"C", @"D", @"E", @"F"];
	NSCItemData *data = [[NSCAppDelegate applicationDelegate] itemData];
	NSMutableArray *pois = [[NSMutableArray alloc] init];
	for (int i = 0; i < 6; ++i) {
		NSCItemDataStatus *poi = data.statuses[i];
		if (!poi) break;
		[pois addObject:[PlaceOfInterest placeOfInterestWithLabel:labels[i] at:poi.location]];
	}
	self.theARView.placesOfInterest = pois;
}

- (void)itemListViewShowItemDetail:(NSString *)photoName description:(NSString *)descriptionText {
	// show item detail view
	[self.itemDetailView setItemForPhoto:photoName description:descriptionText];
	[self.itemDetailView openDetail];
}

@end
