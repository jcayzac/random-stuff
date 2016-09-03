/*
 *  CalibrationParameters.cpp
 *  
 *  Created by Cayzac Julien on 9/14/10.
 *  Copyright 2010 -. All rights reserved.
 *  
 *  Intrinsic parameters and distortion coefficients.
 *  
 */
#include "CalibrationParameters.h"

static const float genericIntrinsicParams[] = { \
	680.f, 0.f, 320.f, \
	0.f, 680.f, 240.f, \
	0.f, 0.f, 1.f \
};

// The following values have been computed offline, using calibration.cpp from
// OpenCV's samples directory:
// $ ./calibration -w 11 -h 7 -s 0.02 -o output.txt list_of_pngs.txt
// ...then get camera_matrix from output.txt

#if 0 // VGA parameters
static const float iPhone4IntrinsicParams[] = {
	680.3738790276293f, 0.f, 326.14035383431434f,
	0.f, 681.96563450484291f, 234.51342509514751f,
	0.f, 0.f, 1.f
};

static const float iPhone4DistortionCoefficients[] = {
	.17506646817233029f, -.48861409344977241f, -.00091311785312679118f, -.00040295402776615436f
};
#endif

#if 1 // QVGA
static const float iPhone4IntrinsicParams[] = {
	77.915945702592921f, 0.f, 177.57389309504691f,
	0.f, 79.421484234806087f, 167.17180112551441f,
	0.f, 0.f, 1.f
};

static const float iPhone4DistortionCoefficients[] = {
	.0012725163025448405f, .00067978164206263512f, .0026488303009681150f, -.0038446101860488942f
};
#endif


#include <string.h>
#include <sys/utsname.h>

const float* const CalibrationParameters::intrinsicParameters() {
	struct utsname name;
	if (!uname(&name)) {
		//if (strcmp(name.machine, "iPhone1,1")==0) return iPhoneIntrinsicParams;
		//if (strcmp(name.machine, "iPhone1,2")==0) return iPhone3GIntrinsicParams;
		//if (strcmp(name.machine, "iPhone2,1")==0) return iPhone3GSIntrinsicParams;
		if (strcmp(name.machine, "iPhone3,1")==0) return iPhone4IntrinsicParams;
		//if (strcmp(name.machine, "iPod1,1")==0) return iPodTouch1IntrinsicParams;
		//if (strcmp(name.machine, "iPod2,1")==0) return iPodTouch2IntrinsicParams;
		//if (strcmp(name.machine, "iPod3,1")==0) return iPodTouch3IntrinsicParams;
	}
	return genericIntrinsicParams;
}

const float* const CalibrationParameters::distortionCoefficients() {
	struct utsname name;
	if (!uname(&name)) {
		//if (strcmp(name.machine, "iPhone1,1")==0) return iPhoneDistortionCoefficients;
		//if (strcmp(name.machine, "iPhone1,2")==0) return iPhone3GDistortionCoefficients;
		//if (strcmp(name.machine, "iPhone2,1")==0) return iPhone3GSDistortionCoefficients;
		if (strcmp(name.machine, "iPhone3,1")==0) return iPhone4DistortionCoefficients;
		//if (strcmp(name.machine, "iPod1,1")==0) return iPodTouch1DistortionCoefficients;
		//if (strcmp(name.machine, "iPod2,1")==0) return iPodTouch2DistortionCoefficients;
		//if (strcmp(name.machine, "iPod3,1")==0) return iPodTouch3DistortionCoefficients;
	}
	return 0;
}
