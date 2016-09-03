//
//  SensorManager.mm
//  Process
//
//  Created by Cayzac Julien on 9/8/10.
//  Copyright 2010 -. All rights reserved.
//

#import "SensorManager.h"

// Using these values, a constant measurement should become current after 0.1s
#define kFilteringFactor .1f
#define kAccelerometerFrequency 100

/**
 * Computes the inclination matrix <b>I</b> as well as the rotation
 * matrix <b>R</b> transforming a vector from the
 * device coordinate system to the world's coordinate system which is
 * defined as a direct orthonormal basis, where:
 * 
 * <li>X is defined as the vector product <b>Y.Z</b> (It is tangential to
 * the ground at the device's current location and roughly points East).</li>
 * <li>Y is tangential to the ground at the device's current location and
 * points towards the magnetic North Pole.</li>
 * <li>Z points towards the sky and is perpendicular to the ground.</li>
 * <p>
 * <hr>
 * <p>By definition:
 * <p>[0 0 g] = <b>R</b> * <b>gravity</b> (g = magnitude of gravity)
 * <p>[0 m 0] = <b>I</b> * <b>R</b> * <b>geomagnetic</b>
 * (m = magnitude of geomagnetic field)
 * <p><b>R</b> is the identity matrix when the device is aligned with the
 * world's coordinate system, that is, when the device's X axis points
 * toward East, the Y axis points to the North Pole and the device is facing
 * the sky.
 *
 * <p><b>I</b> is a rotation matrix transforming the geomagnetic
 * vector into the same coordinate space as gravity (the world's coordinate
 * space). <b>I</b> is a simple rotation around the X axis.
 * The inclination angle in radians can be computed with
 * {@link #getInclination}.
 * <hr>
 * 
 * <p> Each matrix is returned either as a 3x3 or 4x4 row-major matrix
 * depending on the length of the passed array:
 * <p><u>If the array length is 16:</u>
 * <pre>
 *   /  M[ 0]   M[ 1]   M[ 2]   M[ 3]  \
 *   |  M[ 4]   M[ 5]   M[ 6]   M[ 7]  |
 *   |  M[ 8]   M[ 9]   M[10]   M[11]  |
 *   \  M[12]   M[13]   M[14]   M[15]  /
 *</pre>
 * This matrix is ready to be used by OpenGL ES's glLoadMatrixf. 
 * <p>Note that because OpenGL matrices are column-major matrices you must
 * transpose the matrix before using it. However, since the matrix is a 
 * rotation matrix, its transpose is also its inverse, conveniently, it is
 * often the inverse of the rotation that is needed for rendering; it can
 * therefore be used with OpenGL ES directly.
 * <p>
 * Also note that the returned matrices always have this form:
 * <pre>
 *   /  M[ 0]   M[ 1]   M[ 2]   0  \
 *   |  M[ 4]   M[ 5]   M[ 6]   0  |
 *   |  M[ 8]   M[ 9]   M[10]   0  |
 *   \      0       0       0   1  /
 *</pre>
 * <p><u>If the array length is 9:</u>
 * <pre>
 *   /  M[ 0]   M[ 1]   M[ 2]  \
 *   |  M[ 3]   M[ 4]   M[ 5]  |
 *   \  M[ 6]   M[ 7]   M[ 8]  /
 *</pre>
 *
 * <hr>
 * <p>The inverse of each matrix can be computed easily by taking its
 * transpose.
 *
 * <p>The matrices returned by this function are meaningful only when the
 * device is not free-falling and it is not close to the magnetic north.
 * If the device is accelerating, or placed into a strong magnetic field,
 * the returned matrices may be inaccurate.
 *
 * @param R is an array of 9 floats holding the rotation matrix <b>R</b>
 * when this function returns. R can be null.<p>
 * @param I is an array of 9 floats holding the rotation matrix <b>I</b>
 * when this function returns. I can be null.<p>
 * @param gravity is an array of 3 floats containing the gravity vector
 * expressed in the device's coordinate.<p>
 * @param geomagnetic is an array of 3 floats containing the geomagnetic
 * vector expressed in the device's coordinate..
 * @return
 *   true on success<p>
 *   false on failure (for instance, if the device is in free fall).
 *   On failure the output matrices are not modified.
 */
static bool getRotationMatrix(float* R, float* I, const float* const gravity, const float* const geomagnetic, const bool is16x16=true) {
	float Ax = gravity[0];
	float Ay = gravity[1];
	float Az = gravity[2];
	const float Ex(geomagnetic[0]);
	const float Ey(geomagnetic[1]);
	const float Ez(geomagnetic[2]);
	float Hx = Ey*Az - Ez*Ay;
	float Hy = Ez*Ax - Ex*Az;
	float Hz = Ex*Ay - Ey*Ax;
	const float normH(sqrtf(Hx*Hx + Hy*Hy + Hz*Hz));
	if (normH < 0.1f) {
		// device is close to free fall (or in space?), or close to
		// magnetic north pole. Typical values are  > 100.
		return false;
	}
	const float invH(1.0f / normH);
	Hx *= invH;
	Hy *= invH;
	Hz *= invH;
	const float invA(1.0f / sqrtf(Ax*Ax + Ay*Ay + Az*Az));
	Ax *= invA;
	Ay *= invA;
	Az *= invA;
	const float Mx(Ay*Hz - Az*Hy);
	const float My(Az*Hx - Ax*Hz);
	const float Mz(Ax*Hy - Ay*Hx);
	if (R) {
		if (is16x16) {
			R[0]  = Hx;    R[1]  = Hy;    R[2]  = Hz;   R[3]  = 0;
			R[4]  = Mx;    R[5]  = My;    R[6]  = Mz;   R[7]  = 0;
			R[8]  = Ax;    R[9]  = Ay;    R[10] = Az;   R[11] = 0;
			R[12] = 0;     R[13] = 0;     R[14] = 0;    R[15] = 1;
		}
		else {
			R[0] = Hx;     R[1] = Hy;     R[2] = Hz;
			R[3] = Mx;     R[4] = My;     R[5] = Mz;
			R[6] = Ax;     R[7] = Ay;     R[8] = Az;
		}
	}
	if (I) {
		// compute the inclination matrix by projecting the geomagnetic
		// vector onto the Z (gravity) and X (horizontal component
		// of geomagnetic vector) axes.
		const float invE = 1.0f / sqrtf(Ex*Ex + Ey*Ey + Ez*Ez);
		const float c = (Ex*Mx + Ey*My + Ez*Mz) * invE;
		const float s = (Ex*Ax + Ey*Ay + Ez*Az) * invE;
		if (is16x16) {
			I[0] = 1;     I[1] = 0;     I[2] = 0;
			I[4] = 0;     I[5] = c;     I[6] = s;
			I[8] = 0;     I[9] =-s;     I[10]= c;
			I[3] = I[7] = I[11] = I[12] = I[13] = I[14] = 0;
			I[15] = 1;
		}
		else {
			I[0] = 1;     I[1] = 0;     I[2] = 0;
			I[3] = 0;     I[4] = c;     I[5] = s;
			I[6] = 0;     I[7] =-s;     I[8] = c;
		}
	}
	return true;
}

/**
 * Computes the geomagnetic inclination angle in radians from the
 * inclination matrix <b>I</b> returned by {@link #getRotationMatrix}.
 * @param I inclination matrix see {@link #getRotationMatrix}.
 * @return The geomagnetic inclination angle in radians.
 */
inline static float getInclination(const float* const I, const bool is16x16=true) {
	return atan2f(I[5+is16x16], I[4+is16x16]);
}

/**
 * Computes the device's orientation based on the rotation matrix.
 * <p> When it returns, the array values is filled with the result:
 * <li>values[0]: <i>azimuth</i>, rotation around the Z axis.</li>
 * <li>values[1]: <i>pitch</i>, rotation around the X axis.</li>
 * <li>values[2]: <i>roll</i>, rotation around the Y axis.</li>
 * <p>
 *
 * @param R rotation matrix see {@link #getRotationMatrix}.
 * @param values an array of 3 floats to hold the result.
 * @return The array values passed as argument.
 */
inline static void getOrientation(const float* const R, float& azimuth, float& pitch, float& roll, const bool is16x16=true) {
	azimuth = atan2f(R[1], R[4+is16x16]);
	pitch   = asinf(-R[7+(int(is16x16)<<1)]);
	roll    = atan2f(-R[6+(int(is16x16)<<1)], R[8+(int(is16x16)<<1)]);
}


@interface SensorManager()<CLLocationManagerDelegate, UIAccelerometerDelegate>
@property (nonatomic, retain) CLLocationManager* locationManager;
@property (readwrite) float* gravity;
@property (readwrite) float* geomagnetic;
@end

@implementation SensorManager
@synthesize locationManager;
@synthesize gravity, geomagnetic;
@synthesize delegate;

- (void)locationManager:(CLLocationManager *)manager
       didUpdateHeading:(CLHeading *)newHeading {
	self.geomagnetic[0] = newHeading.x;
	self.geomagnetic[1] = newHeading.y;
	self.geomagnetic[2] = newHeading.z;
}

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
	//Use a basic low-pass filter to only keep the gravity in the accelerometer values
	//The value will get normalized in getRotationMatrix, so we really don't care about its scale...
	self.gravity[0] = acceleration.x * kFilteringFactor + self.gravity[0] * (1.0f - kFilteringFactor);
	self.gravity[1] = acceleration.y * kFilteringFactor + self.gravity[1] * (1.0f - kFilteringFactor);
	self.gravity[2] = acceleration.z * kFilteringFactor + self.gravity[2] * (1.0f - kFilteringFactor);
	float rotation[16];
	float inclination[16];
	if (getRotationMatrix(rotation, inclination, gravity, geomagnetic)) {
		[self.delegate deviceToWorldRotation:rotation inclination:inclination];
	}
}

- (id) init {
	self = [super init];
	if (self != nil) {
		self.gravity = new float[3];
		self.geomagnetic = new float[3];
		
		self.locationManager = [[[CLLocationManager alloc] init] autorelease];
		self.locationManager.delegate = self;
		[self.locationManager setDesiredAccuracy:kCLLocationAccuracyBest];
		[self.locationManager setDistanceFilter:kCLDistanceFilterNone];
		[self.locationManager setHeadingFilter:kCLHeadingFilterNone];
		[self.locationManager setHeadingOrientation:CLDeviceOrientationPortrait];
		[self.locationManager startUpdatingLocation];
		[self.locationManager startUpdatingHeading];
		
		[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0/kAccelerometerFrequency)];
		[[UIAccelerometer sharedAccelerometer] setDelegate:self];
	}
	return self;
}

- (void)dealloc {
	delete[] self.gravity;
	delete[] self.geomagnetic;
	[super dealloc];
}

@end
