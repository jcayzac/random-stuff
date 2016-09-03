#import <UIKit/UIKit.h>

@protocol NSCItemListViewDelegate <NSObject>
@optional
- (void)itemListViewRefreshedItsContent;
@end

@interface NSCItemListView : UIView
@property (nonatomic) NSMutableArray *cells;
@property (nonatomic) NSObject <NSCItemListViewDelegate> *delegate;
@end
