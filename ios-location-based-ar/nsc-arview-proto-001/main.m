#import <UIKit/UIKit.h>
#import "NSCAppDelegate.h"

int main(int argc, char *argv[]) {
	setenv("CG_NUMERICS_SHOW_BACKTRACE", "1", 1);

	@autoreleasepool {
		return UIApplicationMain(argc, argv, nil, NSStringFromClass([NSCAppDelegate class]));
	}
}
