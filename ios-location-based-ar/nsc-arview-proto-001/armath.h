#pragma once
#include "vectormath/scalar/cpp/vectormath_aos.h"

#define noalias(x) __restrict__ x
#define expect(cnd, result) __builtin_expect(cnd, result)

#if defined(ARMATH_NAMESPACE)
namespace ARMATH_NAMESPACE {
#else
namespace armath {
#endif

using namespace ::Vectormath::Aos;

static inline Matrix4 GetPerspectiveProjectionMatrix(
													 const double &noalias(near), const double &noalias(far),
													 const double &noalias(fov), const double &noalias(aspect)) {
	const double focal(1.0 / tan(fov * 0.5));
	const double invNF(1.0 / (near - far));
	return Matrix4(Vector4(float(focal), 0.f, 0.f, 0.f), Vector4(0.f, float(aspect * focal), 0.f, 0.f),
				   Vector4(0.f, 0.f, float((far + near) * invNF), -1.f),
				   Vector4(0.f, 0.f, float(2 * far * near * invNF), 0.f));
}
	
	static inline Matrix4
	GetInfiniteProjectionMatrix(const double &noalias(near),
								const double &noalias(fov),
								const double &noalias(aspect)) {
		const double focal(1.0 / tan(fov * 0.5));
		static const double e(0.00001);
		return Matrix4(Vector4(focal, 0, 0, 0), Vector4(0, aspect * focal, 0, 0),
					   Vector4(0, 0, e - 1, -1), Vector4(0, 0, (e - 2) * near, 0));
	}
	
	// WGS 84 semi-major axis constant in meters
	static inline const double &WGS84_A() {
		static const double value(6378137);
		return value;
	}
	
	// WGS 84 first eccentricity squared
	static inline const double &WGS84_E2() {
		static const double value(0.00669437999014);
		return value;
	}
	
	static inline void GetSinesFromAngles(const double &noalias(phi),
										  const double &noalias(lambda),
										  double &noalias(cosineOfPhi),
										  double &noalias(sineOfPhi),
										  double &noalias(cosineOfLambda),
										  double &noalias(sineOfLambda)) {
		cosineOfPhi = cos(phi);
		sineOfPhi = sin(phi);
		cosineOfLambda = cos(lambda);
		sineOfLambda = sin(lambda);
	}
	
	static inline void GetSinesFromAnglesInDegrees(const double &noalias(phi),
												   const double &noalias(lambda),
												   double &noalias(cosineOfPhi),
												   double &noalias(sineOfPhi),
												   double &noalias(cosineOfLambda),
												   double &noalias(sineOfLambda)) {
		return GetSinesFromAngles(phi * M_PI / 180, lambda * M_PI / 180, cosineOfPhi,
								  sineOfPhi, cosineOfLambda, sineOfLambda);
	}
	
	static inline void GetECEFFromSinesAndAltitude(
												   const double &noalias(cosineOfPhi), const double &noalias(sineOfPhi),
												   const double &noalias(cosineOfLambda), const double &noalias(sineOfLambda),
												   const double &noalias(altitude), double &noalias(x), double &noalias(y),
												   double &noalias(z)) {
		const double N(WGS84_A() / sqrt(1 - WGS84_E2() * sineOfPhi * sineOfPhi));
		x = (N + altitude) * cosineOfPhi * cosineOfLambda;
		y = (N + altitude) * cosineOfPhi * sineOfLambda;
		z = (N * (1 - WGS84_E2()) + altitude) * sineOfPhi;
	}
	
	static inline void GetECEFFromGeodeticCoordinates(
													  const double &noalias(latitude), const double &noalias(longitude),
													  const double &noalias(altitude), double &noalias(x), double &noalias(y),
													  double &noalias(z)) {
		double cosineOfPhi, sineOfPhi, cosineOfLambda, sineOfLambda;
		GetSinesFromAnglesInDegrees(latitude, longitude, cosineOfPhi, sineOfPhi,
									cosineOfLambda, sineOfLambda);
		GetECEFFromSinesAndAltitude(cosineOfPhi, sineOfPhi, cosineOfLambda,
									sineOfLambda, altitude, x, y, z);
	}
	
	// e.g. On iOS,
	// Quat cameraRotation(Quat::identity());
	//
	// // Use CMAttitudeReferenceFrameXTrueNorthZVertical when starting the motion
	// manager
	// CMDeviceMotion *d = motionManager.deviceMotion;
	// if (d) {
	//     CMQuaternion q = d.attitude.quaternion;
	//     cameraRotation = Quat(q.x, q.y, q.z, -q.w);
	// }
	//
	// Matrix4 transform(projectionMatrix * GetECEFToViewMatrix(
	//   Matrix4::rotation(cameraRotation),
	//   location.coordinate.latitude,
	//   location.coordinate.longitude,
	//   location.altitude
	// ));
	//
	// Then you can transform using
	// Vector4 poiScreen(transform * poiECEF);
	// ...or, better, pass the transform matrix to your GLES2
	// shader.
	static inline Matrix4
	GetECEFToViewMatrix(const Matrix4 &noalias(cameraRotation),
						const double &noalias(cameraLatitude),
						const double &noalias(cameraLongitude),
						const double &noalias(cameraAltitude)) {
		double cPhi, sPhi, cLambda, sLambda;
		GetSinesFromAnglesInDegrees(cameraLatitude, cameraLongitude, cPhi, sPhi,
									cLambda, sLambda);
		
		double rX, rY, rZ;
		GetECEFFromSinesAndAltitude(cPhi, sPhi, cLambda, sLambda, cameraAltitude, rX,
									rY, rZ);
		
		return (cameraRotation // P(NWU) -> view
				) *
		Matrix4( // P(ENU) -> P(NWU)
				-Vector4::yAxis(), Vector4::xAxis(), Vector4::zAxis(),
				Vector4::wAxis()) *
		Matrix4( // P(ECEF) -> P(ENU)
				Vector4(sLambda, sPhi * cLambda, -cPhi * cLambda, 0),
				Vector4(-cLambda, sPhi * sLambda, -cPhi * sLambda, 0),
				Vector4(0, -cPhi, -sPhi, 0),
				Vector4(cLambda * rY - sLambda * rX,
						cPhi * rZ - sPhi * cLambda * rX - sPhi * sLambda * rY,
						cPhi * (cLambda * rX + sLambda * rY) + sPhi * rZ -
						cameraAltitude,
						1)
				/*
				 ) * Matrix4( // P(ENU) -> P(NWU)
				 -Vector4::yAxis(),
				 Vector4::xAxis(),
				 Vector4::zAxis(),
				 Vector4::wAxis()
				 ) * Matrix4( // P'(ECEF) -> P(ENU)
				 Vector4(-sLambda, -sPhi*cLambda,    cPhi*cLambda, 0),
				 Vector4( cLambda, -sPhi*sLambda,    cPhi*sLambda, 0),
				 Vector4(       0,          cPhi,            sPhi, 0),
				 Vector4(       0,             0, -cameraAltitude, 1)
				 ) * Matrix4( // P(ECEF) -> R(ECEF)-P(ECEF) = P'(ECEF)
				 -Vector4::xAxis(),
				 -Vector4::yAxis(),
				 -Vector4::zAxis(),
				 Vector4(rX, rY, rZ, 1)
				 */
				);
	}
	
} // armath
	
#undef noalias
#undef expect
