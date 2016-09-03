#import "NSCItemListView.h"
#import "NSCItemListCellView.h"
#import "NSCAppDelegate.h"

@interface NSCItemListView ()
@property (nonatomic) NSArray *markerList;
- (void)initUI;
@end

@implementation NSCItemListView
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
	// marker list
	self.markerList = @[@"A", @"B", @"C", @"D", @"E", @"F"];

	self.cells = [NSMutableArray arrayWithCapacity:6];
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 3; i++) {
			NSCItemListCellView *cell = [[NSCItemListCellView alloc] initWithFrame:CGRectMake((96 + 8) * i, (96 + 8) * j, 96, 96)];
			[self addSubview:cell];
			[self.cells addObject:cell];
		}
	}
	int i = 0;
	for (NSCItemListCellView *cell in[self.cells objectEnumerator]) {
		NSString *markerText = self.markerList[i];
		NSCItemDataStatus *data = kItemData.statuses[i++];
		[cell setItemForPhoto:data.photo description:data.itemDescription marker:markerText];
	}
	[self.delegate itemListViewRefreshedItsContent];
}

- (void)setDelegate:(NSObject <NSCItemListViewDelegate> *)delegate {
	_delegate = delegate;
	[self.delegate itemListViewRefreshedItsContent];

	for (NSCItemListCellView *cell in[self.cells objectEnumerator]) {
		cell.delegate = (NSObject <NSCItemListCellViewDelegate> *)delegate;
	}
}

@end
