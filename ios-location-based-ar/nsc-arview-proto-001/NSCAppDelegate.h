#import <UIKit/UIKit.h>
#import "NSCItemData.h"

#define kItemData       [[NSCAppDelegate applicationDelegate] itemData]

@interface NSCAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) NSCItemData *itemData;

+ (id)applicationDelegate;

@end
