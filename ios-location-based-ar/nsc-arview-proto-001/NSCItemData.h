#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

@class NSCItemDataStatus;

@interface NSCItemData : NSObject
@property (nonatomic) NSArray<NSCItemDataStatus *> *statuses;
@end


@interface NSCItemDataStatus : NSObject

@property (nonatomic, copy) CLLocation *location;
@property (nonatomic, copy) NSString *photo;
@property (nonatomic, copy) NSString *itemDescription;

- (id)initWithLatitude:(CLLocationDegrees)latitude longitude:(CLLocationDegrees)longitude altitude:(CLLocationDistance)altitude photo:(NSString *)photo description:(NSString *)description;

@end
