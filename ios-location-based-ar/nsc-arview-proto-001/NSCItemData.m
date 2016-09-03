#import "NSCItemData.h"

@implementation NSCItemData
- (id)init {
	if ((self = [super init])) {
		// test data
		NSMutableArray *statuses = NSMutableArray.array;

		[statuses addObject:[NSCItemDataStatus.alloc initWithLatitude:90 longitude:0 altitude:0 photo:@"item012.jpeg" description:@"North"]];
		[statuses addObject:[NSCItemDataStatus.alloc initWithLatitude:-90 longitude:0 altitude:0 photo:@"item014.jpeg" description:@"South"]];
		[statuses addObject:[NSCItemDataStatus.alloc initWithLatitude:48.8588377 longitude:2.2775172 altitude:30 photo:@"item007.jpeg" description:@"Paris"]];
		[statuses addObject:[NSCItemDataStatus.alloc initWithLatitude:35.6581003 longitude:139.699553 altitude:50 photo:@"item001.jpeg" description:@"Shibuya Station"]];
		[statuses addObject:[NSCItemDataStatus.alloc initWithLatitude:35.6103005 longitude:139.6269223 altitude:50 photo:@"item002.jpeg" description:@"Futakotamagawa Station"]];
		[statuses addObject:[NSCItemDataStatus.alloc initWithLatitude:35.6104111 longitude:139.6298379 altitude:50 photo:@"item010.jpeg" description:@"9F Cafeteria"]];

		_statuses = statuses;
	}
	return self;
}
@end

@implementation NSCItemDataStatus

- (id)initWithLatitude:(CLLocationDegrees)latitude longitude:(CLLocationDegrees)longitude altitude:(CLLocationDistance)altitude photo:(NSString *)photo description:(NSString *)description {
	self = [super init];
	if (self) {
		// Initialization code
		CLLocationCoordinate2D coords;
		coords.longitude = longitude;
		coords.latitude = latitude;
		_location = [[CLLocation alloc] initWithCoordinate:coords altitude:altitude horizontalAccuracy:1 verticalAccuracy:1 timestamp:NSDate.date];
		_photo = photo;
		_itemDescription = description;
	}
	return self;
}

@end
