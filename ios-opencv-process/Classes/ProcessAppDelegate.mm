#import <QuartzCore/QuartzCore.h>
#import "ProcessAppDelegate.h"
#import "ProcessViewController.h"

@implementation ProcessAppDelegate
@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	window.rootViewController = viewController;
	[window makeKeyAndVisible];
	return YES;
}

- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}
@end
