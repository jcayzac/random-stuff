#import <UIKit/UIKit.h>
#import "ARItemMarker.h"

@protocol NSCItemListCellViewDelegate <NSObject>
@optional
- (void)itemListViewShowItemDetail:(NSString *)photoName description:(NSString *)descriptionText;
@end

@interface NSCItemListCellView : UIView
@property (nonatomic) UIButton *photoButton;
@property (nonatomic) UILabel *descriptionLabel;
@property (nonatomic) ARItemMarker *markerView;
@property (nonatomic) NSObject <NSCItemListCellViewDelegate> *delegate;

- (void)setItemForPhoto:(NSString *)photoName description:(NSString *)descriptionText marker:(NSString *)markerText;
- (void)fadeOutItemForPhoto:(NSString *)photoName description:(NSString *)descriptionText marker:(NSString *)markerText moveY:(float)moveY;
- (void)fadeInItem:(float)moveY;

@end
