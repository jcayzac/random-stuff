#import "ARItemMarker.h"

@implementation ARItemMarker

- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self) {
		// Initialization code
		label = [[UILabel alloc] initWithFrame:frame];
		[label setBackgroundColor:[UIColor clearColor]];
		[label setTextColor:[UIColor whiteColor]];
		[label setTextAlignment:NSTextAlignmentCenter];
		imageView = [[UIImageView alloc] initWithFrame:frame];
		[self addSubview:label];
		[self addSubview:imageView];
		[self setImage:[UIImage imageNamed:@"marker.png"]];
	}
	return self;
}

- (void)setString:(NSString *)string {
	[label setText:string];
}

- (void)setImage:(UIImage *)img {
	[imageView setImage:img];
}

@end
