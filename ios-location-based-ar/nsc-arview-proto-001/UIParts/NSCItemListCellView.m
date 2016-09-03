#import "NSCItemListCellView.h"

@interface NSCItemListCellView ()
@property (nonatomic) BOOL isAnimating;
@property (nonatomic, copy) NSString *nextPhotoName;
@property (nonatomic, copy) NSString *nextDescriptionText;
@property (nonatomic, copy) NSString *nextMarkerText;
@property (nonatomic) float fadeMoveY;

- (void)initUI;
@end

@implementation NSCItemListCellView
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
	self.photoButton = [UIButton buttonWithType:UIButtonTypeCustom];
	[self.photoButton setFrame:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height)];
	[self.photoButton addTarget:self action:@selector(photoButtonDidPush:) forControlEvents:UIControlEventTouchUpInside];
	[self addSubview:self.photoButton];

	// description text
	self.descriptionLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, self.frame.size.height / 2, self.frame.size.width, self.frame.size.height / 2)];
	self.descriptionLabel.font = [UIFont systemFontOfSize:11];  // 7, 9, 11
	self.descriptionLabel.text = @"";
	self.descriptionLabel.numberOfLines = 0;
	self.descriptionLabel.textAlignment = NSTextAlignmentCenter;
	self.descriptionLabel.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.5];
	self.descriptionLabel.textColor = [UIColor whiteColor];
	[self addSubview:self.descriptionLabel];

	// marker
	self.markerView = [[ARItemMarker alloc] initWithFrame:CGRectMake(-4, -4, 30, 30)];
	[self addSubview:self.markerView];
}

- (void)setItemForPhoto:(NSString *)photoName description:(NSString *)descriptionText marker:(NSString *)markerText {
	[self.photoButton setBackgroundImage:[UIImage imageNamed:photoName] forState:UIControlStateNormal];
	self.descriptionLabel.text = descriptionText;
	[self.markerView setString:markerText];
	self.nextPhotoName = photoName;
	self.nextDescriptionText = descriptionText;
}

- (void)fadeOutItemForPhoto:(NSString *)photoName description:(NSString *)descriptionText marker:(NSString *)markerText moveY:(float)moveY {
	if (self.isAnimating == YES) {
		return;
	}
	self.isAnimating = YES;
	self.nextPhotoName = photoName;
	self.nextDescriptionText = descriptionText;
	self.nextMarkerText = markerText;
	self.fadeMoveY = moveY;

	CGContextRef context = UIGraphicsGetCurrentContext();
	[UIView beginAnimations:nil context:context];
	[UIView setAnimationDuration:0.2];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(fadeOutItemDidStop:finished:context:)];

	CGRect rectView = self.frame;
	rectView.origin.y += moveY;
	self.frame = rectView;
	self.alpha = 0.0f;

	[UIView commitAnimations];
}

- (void)fadeOutItemDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {
	self.isAnimating = NO;
	[self setItemForPhoto:self.nextPhotoName description:self.nextDescriptionText marker:self.nextMarkerText];
	[self fadeInItem:self.fadeMoveY];
}

- (void)fadeInItem:(float)moveY {
	if (self.isAnimating == YES) {
		return;
	}
	self.isAnimating = YES;

	CGRect rectView = self.frame;
	rectView.origin.y -= (moveY * 2);
	self.frame = rectView;

	CGContextRef context = UIGraphicsGetCurrentContext();
	[UIView beginAnimations:nil context:context];
	[UIView setAnimationDuration:0.2];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(fadeInItemDidStop:finished:context:)];

	rectView = self.frame;
	rectView.origin.y += moveY;
	self.frame = rectView;
	self.alpha = 1.0f;

	[UIView commitAnimations];
}

- (void)fadeInItemDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {
	self.isAnimating = NO;
}

- (void)photoButtonDidPush:(id)sender {
	[self.delegate itemListViewShowItemDetail:self.nextPhotoName description:self.nextDescriptionText];
}

@end
