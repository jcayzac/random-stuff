#import <UIKit/UIKit.h>

@interface NSCItemDetailView : UIView

@property (nonatomic) UIImageView *photoImageView;
@property (nonatomic) UILabel *descriptionLabel;

- (void)setItemForPhoto:(NSString *)photoName description:(NSString *)descriptionText;
- (void)openDetail;

@end
