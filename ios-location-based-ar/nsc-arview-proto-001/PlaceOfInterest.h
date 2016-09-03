#import <Foundation/Foundation.h>

#import <CoreLocation/CoreLocation.h>

@interface PlaceOfInterest : NSObject
@property (nonatomic) CLLocation *location;
@property (nonatomic) NSString *label;

+ (PlaceOfInterest *)placeOfInterestWithLabel:(NSString *)label at:(CLLocation *)location;

@end
