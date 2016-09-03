#import "NSCAppDelegate.h"

@implementation NSCAppDelegate
+ (id)applicationDelegate {
	return (NSCAppDelegate *)[[UIApplication sharedApplication] delegate];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	// Override point for customization after application launch.
	self.itemData = [[NSCItemData alloc] init];
	return YES;
}

@end
