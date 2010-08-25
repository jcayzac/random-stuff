
#ifndef __VIEWCUBE_H
#define __VIEWCUBE_H

#include "commode.h"
#include "Hemicube.h"
#include "radpatch.h"

#include "ZPixel.h"
#include "zBufferPatch.h"



class CViewCube {
	
	public:
		// center of cube, Normal of top, pointer to hemicube
		CViewCube(SMLVec3f cVert, SMLVec3f N, CHemicube *h);
		~CViewCube();
		
		void projPatchOnCube(int pID, CRadPatch *p);	// add patch to viewcube and Z-buffer
		// fill array formFactors[] with summed form factors from pixel form factors in cube
		// assumes formFactors[] inited to size of patch ID range
		void sumFormFactors(float *formFactors);
		void print();	// print out top of view cube
		
	private :
	
		CZPixel *topPixels;		// array of patch IDs at each pixel on cube
		CZPixel *sidePixels[4];
		
		SMLVec3f centVert;		// position of center of cube in world space
		SMLVec3f norm;
		
		int numPixelsWide;
		
		float worldToPixUnit;	// conversion factor from world coords to pixels
		
		CHemicube *hCube;		// pointer to global hemicube
		
		
		int *yOffset;
		
		
		
		// precalculated as an optimization
		
		
		// based on width of hemicube
		float halfWorldWide;
		float startXWorld;
		float endXWorld;
		float startYWorld;
		float endYWorld;
		float startSideYWorld;	// sides of view cube are "shorter"
		
		// difference between cube center and origin
		SMLVec3f deltaCenter;
		
		// rotation needed to line normal up with Z+
		float thetaX;
		float thetaY;
		
		
		
		
	};
	
	
#endif