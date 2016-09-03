#import <UIKit/UIKit.h>

@class ProcessViewController;

@interface ProcessAppDelegate : UIResponder <UIApplicationDelegate> {
    UIWindow *window;
    ProcessViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet ProcessViewController *viewController;
@end

