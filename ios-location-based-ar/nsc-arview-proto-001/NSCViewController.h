#import <UIKit/UIKit.h>
#import "ARView.h"
#import "NSCItemListView.h"
#import "NSCItemDetailView.h"

@interface NSCViewController : UIViewController <NSCItemListViewDelegate>

@property (strong, nonatomic) IBOutlet ARView *theARView;
@property (strong, nonatomic) IBOutlet NSCItemListView *itemView;
@property (strong, nonatomic) IBOutlet NSCItemDetailView *itemDetailView;

- (void)itemListViewRefreshedItsContent;
@end
