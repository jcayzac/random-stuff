#import <AVFoundation/AVFoundation.h>
#import "ARView.h"
#import "PlaceOfInterest.h"
#include <vector>
#include "armath.h"

using namespace armath;

#pragma mark -
#pragma mark ARView extension

struct PoiCoordinates {
	Vector4 localCoordinates;
	float distanceFromUser;
};

@interface ARView () {
	Matrix4 ECEFToLocal;
	Matrix4 projectionTransform;
	Quat cameraRotation;
	Matrix4 projectionCameraTransform;
	std::vector <PoiCoordinates> placesOfInterestCoordinates;
}
@property (nonatomic) CADisplayLink *displayLink;
@property (nonatomic) CMMotionManager *motionManager;
@property (nonatomic) CLLocationManager *locationManager;
@property (nonatomic) CLLocation *location;
@property (nonatomic) UIFont *fontForItemID;
@property (nonatomic) UIFont *fontForItemDistance;

- (void)initialize;

- (void)startLocation;
- (void)stopLocation;

- (void)startDeviceMotion;
- (void)stopDeviceMotion;

- (void)startDisplayLink;
- (void)stopDisplayLink;

- (void)updatePlacesOfInterestCoordinates;

- (void)onDisplayLink:(id)sender;
- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation;

@end


#pragma mark -
#pragma mark ARView implementation

@implementation ARView

- (void)dealloc {
	[self stop];
}

- (void)start {
	[self startLocation];
	[self startDeviceMotion];
	[self startDisplayLink];
}

- (void)stop {
	[self stopLocation];
	[self stopDeviceMotion];
	[self stopDisplayLink];
}

- (void)setPlacesOfInterest:(NSArray *)pois {
	_placesOfInterest = pois;
	placesOfInterestCoordinates.clear();
	placesOfInterestCoordinates.resize(self.placesOfInterest.count);
	if (self.location != nil) {
		[self updatePlacesOfInterestCoordinates];
	}
}

- (void)initialize {
	self.fontForItemID = [UIFont systemFontOfSize:16.0];
	self.fontForItemDistance = [UIFont systemFontOfSize:16.0];

	// Initialize projection matrix
	projectionTransform = armath::GetInfiniteProjectionMatrix(0.25, 40 * M_PI / 180, self.bounds.size.width / self.bounds.size.height);
	//projectionTransform = armath::GetPerspectiveProjectionMatrix(0.25, 1000.0, 40.0*M_PI/180.0, self.bounds.size.width / self.bounds.size.height);
	cameraRotation = Quat::identity();
}

- (void)startLocation {
	self.locationManager = [[CLLocationManager alloc] init];
	self.locationManager.delegate = self;
	self.locationManager.distanceFilter = 10.0;
	self.locationManager.desiredAccuracy = kCLLocationAccuracyBest;
	[self.locationManager requestWhenInUseAuthorization];
	[self.locationManager startUpdatingLocation];
}

- (void)stopLocation {
	[self.locationManager stopUpdatingLocation];
	self.locationManager = nil;
}

- (void)startDeviceMotion {
	self.motionManager = [[CMMotionManager alloc] init];

	// Tell CoreMotion to show the compass calibration HUD when required to provide true north-referenced attitude
	self.motionManager.showsDeviceMovementDisplay = YES;

	self.motionManager.deviceMotionUpdateInterval = 1.0 / 60.0;

	// New in iOS 5.0: Attitude that is referenced to true north
	[self.motionManager startDeviceMotionUpdatesUsingReferenceFrame:
	 //CMAttitudeReferenceFrameXMagneticNorthZVertical];
	 CMAttitudeReferenceFrameXTrueNorthZVertical];
}

- (void)stopDeviceMotion {
	[self.motionManager stopDeviceMotionUpdates];
	self.motionManager = nil;
}

- (void)startDisplayLink {
	self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(onDisplayLink:)];
	[self.displayLink setFrameInterval:1];
	[self.displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)stopDisplayLink {
	[self.displayLink invalidate];
	self.displayLink = nil;
}

- (void)updatePlacesOfInterestCoordinates {
	int i = 0;
	for (PlaceOfInterest *poi in self.placesOfInterest) {
		double poiX, poiY, poiZ;
		armath::GetECEFFromGeodeticCoordinates(poi.location.coordinate.latitude, poi.location.coordinate.longitude, poi.location.altitude, poiX, poiY, poiZ);
		placesOfInterestCoordinates[i].localCoordinates = Vector4(poiX, poiY, poiZ, 1.f);
		placesOfInterestCoordinates[i].distanceFromUser = (float)[self.location distanceFromLocation:poi.location];
		++i;
	}
}

- (void)onDisplayLink:(id)sender {
	CMDeviceMotion *d = self.motionManager.deviceMotion;
	if (d != nil) {
		CMQuaternion q = d.attitude.quaternion;
		cameraRotation = Quat((float)q.x, (float)q.y, (float)q.z, (float)-q.w);
		[self setNeedsDisplay];
	}
}

- (void)drawRect:(CGRect)rect {
	if (placesOfInterestCoordinates.empty())
		return;
	if (!self.location)
		return;

	CGContextRef con = UIGraphicsGetCurrentContext();
	CGContextSetLineJoin(con, kCGLineJoinRound);
	CGContextSetLineCap(con, kCGLineCapRound);
	CGContextSetLineWidth(con, 4);
	CGContextSetRGBStrokeColor(con, 1, 1, 1, 1);
	CGContextBeginPath(con);
	CGContextMoveToPoint(con, -30, 0);
	CGContextAddLineToPoint(con, 0, -30);
	CGContextAddLineToPoint(con, 0, -15);
	CGContextAddLineToPoint(con, 30, -15);
	CGContextAddLineToPoint(con, 30, 15);
	CGContextAddLineToPoint(con, 0, 15);
	CGContextAddLineToPoint(con, 0, 30);
	CGContextClosePath(con);
	CGPathRef arrow = CGContextCopyPath(con);
	CGContextBeginPath(con);
	CGContextMoveToPoint(con, -15, -15);
	CGContextAddEllipseInRect(con, CGRectMake(-15.0, -15.0, 30.0, 30.0));
	CGContextClosePath(con);
	CGPathRef circle = CGContextCopyPath(con);
	CGContextSetTextDrawingMode(con, kCGTextFill);

	static Quat rotation(Quat::identity());
	rotation = slerp(
	        .5f,
	        select(rotation, -rotation, dot(rotation, cameraRotation) < 0),
	        cameraRotation
	        );

	projectionCameraTransform = projectionTransform * armath::GetECEFToViewMatrix(
	        Matrix4::rotation(rotation),
	        self.location.coordinate.latitude,
	        self.location.coordinate.longitude,
	        self.location.altitude
	        );

	int i = -1;
	for (PlaceOfInterest *poi in self.placesOfInterest) {
		++i;

		// project point
		Vector4 pos3d(projectionCameraTransform * placesOfInterestCoordinates[i].localCoordinates);

		// homogenous coordinates
		Vector3 homogenous(pos3d.getXYZ() / pos3d.getW());

		// clipped to [-1.1 1.1] along the X axis
		// (makes the Y coordinate stable even for Z>0)
		Vector3 clipped(homogenous);
		if (clipped.getX() <= -1.1f) clipped *= -1.1f / clipped.getX();
		else if (clipped.getX() >= 1.1f) clipped *= 1.1f / clipped.getX();

		// convert to window's coordinate space
		Vector3 pos;
		pos.setX((1.f + clipped.getX()) * 0.5f * self.bounds.size.width);
		pos.setY((1.f - clipped.getY()) * 0.5f * self.bounds.size.height);
		pos.setZ(1.f);

		// Background for shapes
		CGContextSetRGBFillColor(con, 0.7f, 0.7f, 0.7f, 0.4f);

		float yOffsetForDistanceLabel = 30.f;
		if (pos.getX() < 0) {
			// left arrow
			pos.setX(36.f);
			yOffsetForDistanceLabel = 44.f;
			CGContextSaveGState(con);
			CGContextTranslateCTM(con, pos.getX(), pos.getY());
			CGContextAddPath(con, arrow);
			CGContextFillPath(con);
			CGContextAddPath(con, arrow);
			CGContextStrokePath(con);
			CGContextRestoreGState(con);
		}
		else if (pos.getX() >= self.bounds.size.width) {
			// right arrow
			pos.setX((float)(self.bounds.size.width - 36.f));
			yOffsetForDistanceLabel = 44.f;
			CGContextSaveGState(con);
			CGContextTranslateCTM(con, pos.getX(), pos.getY());
			CGContextScaleCTM(con, -1.0, 1.0);
			CGContextAddPath(con, arrow);
			CGContextFillPath(con);
			CGContextAddPath(con, arrow);
			CGContextStrokePath(con);
			CGContextRestoreGState(con);
		}
		else if (pos3d.getZ() < .0f) {
			// circle
			CGContextSaveGState(con);
			CGContextTranslateCTM(con, pos.getX(), pos.getY());
			CGContextAddPath(con, circle);
			CGContextFillPath(con);
			CGContextAddPath(con, circle);
			CGContextStrokePath(con);
			CGContextRestoreGState(con);
		}
		else continue;

		// Background for text
		CGContextSetRGBFillColor(con, 1, 1, 1, 1);

		// Draw label
		CGPoint textPos = CGPointMake(pos.getX(), pos.getY());
		CGSize textSize = [poi.label sizeWithFont:self.fontForItemID];
		textPos.x -= textSize.width * .5f;
		textPos.y -= textSize.height * .5f;
		[poi.label drawAtPoint:textPos withFont:self.fontForItemID];

		// Draw distance
		float distance = placesOfInterestCoordinates[i].distanceFromUser;
		NSString *distanceLabel = nil;
		if (distance < 1000.f) {
			distanceLabel = [NSString stringWithFormat:@"%im", (int)distance];
		}
		else if (distance < 10000.f) {
			distanceLabel = [NSString stringWithFormat:@"%.1fKm", distance * .001f];
		}
		else {
			distanceLabel = [NSString stringWithFormat:@"%iKm", (int)(distance * .001f)];
		}
		textPos = CGPointMake(pos.getX(), pos.getY() - yOffsetForDistanceLabel);
		textSize = [distanceLabel sizeWithFont:self.fontForItemDistance];
		textPos.x -= textSize.width * .5f;
		textPos.y -= textSize.height * .5f;
		[distanceLabel drawAtPoint:textPos withFont:self.fontForItemDistance];
	}

	CGPathRelease(arrow);
	CGPathRelease(circle);
}

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation {
	self.location = newLocation;
	if (self.placesOfInterest != nil) {
		/*
		   CLLocationCoordinate2D coords = newLocation.coordinate;
		   CLLocationDistance altitude = newLocation.altitude;

		   // North
		   coords.latitude += 1.f;
		   PlaceOfInterest* poi = self.placesOfInterest[0];
		   CLLocation* location = [poi.location initWithCoordinate:coords altitude:altitude horizontalAccuracy:newLocation.horizontalAccuracy verticalAccuracy:newLocation.verticalAccuracy course:newLocation.course speed:newLocation.speed timestamp:newLocation.timestamp];

		   // South
		   coords.latitude -= 2.f;
		   poi = self.placesOfInterest[1];
		   location = [poi.location initWithCoordinate:coords altitude:altitude horizontalAccuracy:newLocation.horizontalAccuracy verticalAccuracy:newLocation.verticalAccuracy course:newLocation.course speed:newLocation.speed timestamp:newLocation.timestamp];

		   // West
		   coords.latitude += 1.f;
		   coords.longitude -= 1.f;
		   poi = self.placesOfInterest[2];
		   location = [poi.location initWithCoordinate:coords altitude:altitude horizontalAccuracy:newLocation.horizontalAccuracy verticalAccuracy:newLocation.verticalAccuracy course:newLocation.course speed:newLocation.speed timestamp:newLocation.timestamp];

		   // East
		   coords.longitude += 2.f;
		   poi = self.placesOfInterest[3];
		   location = [poi.location initWithCoordinate:coords altitude:altitude horizontalAccuracy:newLocation.horizontalAccuracy verticalAccuracy:newLocation.verticalAccuracy course:newLocation.course speed:newLocation.speed timestamp:newLocation.timestamp];

		   // Down
		   coords.longitude -= 1.f;
		   altitude -= 1000.f;
		   poi = self.placesOfInterest[4];
		   location = [poi.location initWithCoordinate:coords altitude:altitude horizontalAccuracy:newLocation.horizontalAccuracy verticalAccuracy:newLocation.verticalAccuracy course:newLocation.course speed:newLocation.speed timestamp:newLocation.timestamp];

		   // Up
		   altitude += 2000.f;
		   poi = self.placesOfInterest[5];
		   location = [poi.location initWithCoordinate:coords altitude:altitude horizontalAccuracy:newLocation.horizontalAccuracy verticalAccuracy:newLocation.verticalAccuracy course:newLocation.course speed:newLocation.speed timestamp:newLocation.timestamp];
		 */

		[self updatePlacesOfInterestCoordinates];
	}
}

- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if (self) {
		[self initialize];
	}
	return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
	self = [super initWithCoder:aDecoder];
	if (self) {
		[self initialize];
	}
	return self;
}

@end
