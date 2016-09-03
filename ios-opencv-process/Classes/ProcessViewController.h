#import <UIKit/UIKit.h>
#import "CaptureSessionManager.h"

@interface ProcessViewController : UIViewController {
	CaptureSessionManager *captureManager;
}

@property (retain) CaptureSessionManager *captureManager;
@end

