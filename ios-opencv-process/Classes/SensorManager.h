//
//  SensorManager.h
//  Process
//
//  Created by Cayzac Julien on 9/8/10.
//  Copyright 2010 -. All rights reserved.
//

#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>

@protocol ARSensorDelegate
@required
- (void)deviceToWorldRotation:(const float*)rotation inclination:(const float*)inclination;
@end


@interface SensorManager : NSObject {
	id<ARSensorDelegate> delegate;
}

@property (nonatomic, assign) id delegate;
@end
