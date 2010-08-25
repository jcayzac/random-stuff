
// Z-buffer patch onto viewplane
// Assumes that plane perp to z-axis, facing z+ dir
// params:
//		pID					= patchID to go into Z-buffer
//		planePixWide 		= view plane pixels wide
//		planeStartX 		= minX of view plane in world coords
//		planeStartY 		= minY of view plane in world coords
// 		d					= distance of viewplane from projection point


#ifndef __ZBUFFERPATCH_H
#define __ZBUFFERPATCH_H

#include "zpixel.h"
#include "radpatch.h"

void CzBufferPatch( CZPixel *viewPlane, int *yOffset, CRadPatch *p, int pID,
						int planePixWide, int planePixHigh,
						float planeStartX, float planeEndX, 
						float planeStartY, float planeEndY,
						float d);
						
						
						
						
#endif