/*
 *  CalibrationParameters.h
 *
 */
#pragma once

class CalibrationParameters {
public:
	// Returns an array of 9 floats [ax s u0 0 ay v0 0 0 1],
	// representing a 3x3 matrix.
	// 
	// From http://en.wikipedia.org/wiki/Camera_resectioning#Intrinsic_parameters
	// 
	//     /ax  s u0\
	// A = | 0 ay v0|
	//     \ 0  0  1/
	//
	// ・ax = f*mx and ay = f*my represent focal length in terms of pixels,
	//  where mx and my are the scale factors relating pixels to distance.
	// ・s is the skew coefficient between the x and y axis, and is often 0.
	// ・u0 and v0 represent the focal point, which would ideally be at the
	//  center of the image.
	static const float* const intrinsicParameters();
	
	// Returns an array of 4 floats
	static const float* const distortionCoefficients();	
};
