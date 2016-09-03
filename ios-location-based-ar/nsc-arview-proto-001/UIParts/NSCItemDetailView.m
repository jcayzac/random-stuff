#import "NSCItemDetailView.h"

@interface NSCItemDetailView ()
@property (nonatomic) BOOL isAnimating;

- (void)initUI;
@end

@implementation NSCItemDetailView
- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self) {
		// Initialization code
		[self initUI];
	}
	return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	if (self) {
		// Initialization code
		[self initUI];
	}
	return self;
}

- (void)initUI {
	// detail button, photo image
	float x = 10;
	float y = 10;
	float w = (float)(self.bounds.size.width - 20.f);
	float h = w;
	self.photoImageView = [[UIImageView alloc] initWithFrame:CGRectMake(x, y, w, h)];
	[self.photoImageView setImage:[UIImage imageNamed:@"item000.jpeg"]];
	[self addSubview:self.photoImageView];

	// description text
	y = x + h + 10;
	h = (float)(self.bounds.size.height - 10 - w);
	self.descriptionLabel = [[UILabel alloc] initWithFrame:CGRectMake(x, y, w, h)];
	self.descriptionLabel.font = [UIFont systemFontOfSize:20];
	self.descriptionLabel.text = @"Test message test message test message test message";
	self.descriptionLabel.numberOfLines = 0;
	self.descriptionLabel.textAlignment = NSTextAlignmentCenter;
	self.descriptionLabel.backgroundColor = [UIColor colorWithRed:0.1f green:0.1f blue:0.1f alpha:1.0f];
	self.descriptionLabel.textColor = [UIColor whiteColor];
	[self addSubview:self.descriptionLabel];

	// background color
	self.backgroundColor = [UIColor brownColor];

	// up swipe gestuer
	UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(didTapAnywhere:)];
	[self addGestureRecognizer:tapGesture];
}

- (void)setItemForPhoto:(NSString *)photoName description:(NSString *)descriptionText {
	[self.photoImageView setImage:[UIImage imageNamed:photoName]];
	self.descriptionLabel.text = descriptionText;
}

- (void)openDetail {
	if (self.isAnimating == YES) {
		return;
	}
	self.isAnimating = YES;

	CGRect rectView = self.frame;
	rectView.origin.x = 8;
	rectView.origin.y = 500;
	self.frame = rectView;
	self.alpha = 0.0f;

	CGContextRef context = UIGraphicsGetCurrentContext();
	[UIView beginAnimations:nil context:context];
	[UIView setAnimationDuration:0.2];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(openDetailDidStop:finished:context:)];

	rectView = self.frame;
	rectView.origin.x =  8;
	rectView.origin.y =  8;
	self.frame = rectView;
	self.alpha = 0.9f;

	[UIView commitAnimations];
}

- (void)openDetailDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {
	self.isAnimating = NO;
}

- (void)closeDetail {
	if (self.isAnimating == YES) {
		return;
	}
	self.isAnimating = YES;

	CGContextRef context = UIGraphicsGetCurrentContext();
	[UIView beginAnimations:nil context:context];
	[UIView setAnimationDuration:0.2];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(closeDetailDidStop:finished:context:)];

	CGRect rectView = self.frame;
	rectView.origin.x = 8;
	rectView.origin.y = 500;
	self.frame = rectView;
	self.alpha = 0.0f;

	[UIView commitAnimations];
}

- (void)closeDetailDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {
	self.isAnimating = NO;
}

- (void)didTapAnywhere:(UITapGestureRecognizer *)recognizer {
	[self closeDetail];
}

@end
