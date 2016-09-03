#import "PlaceOfInterest.h"

@implementation PlaceOfInterest
+ (PlaceOfInterest *)placeOfInterestWithLabel:(NSString *)label at:(CLLocation *)location {
	PlaceOfInterest *poi = [[PlaceOfInterest alloc] init];
	poi.label = label;
	poi.location = location;
	return poi;
}

@end
