#import <UIKit/UIKit.h>

@interface ARItemMarker : UIView {
	UILabel *label;
	UIImageView *imageView;
}

- (void)setString:(NSString *)string;
- (void)setImage:(UIImage *)image;

@end
