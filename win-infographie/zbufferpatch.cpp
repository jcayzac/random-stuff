
#include "zbufferpatch.h"


void CzBufferPatch( CZPixel *viewPlane, int *yOffset, CRadPatch *p, int pID, 
						int planePixWide, int planePixHigh,
						float planeStartX, float planeEndX, 
						float planeStartY, float planeEndY,
						float d) {
	
	float minDist = 0.001 * d;	// near clipping plane
	
	char isBack = false;	// test whether patch back is facing this viewplane
	
	SMLVec3f patchNorm = p->norm; 
	SMLVec3f planeNorm(0,0,1);
	if( patchNorm.Dot(planeNorm) >0 ) {		// patch facing away from view plane
		isBack = true;
	}
	
	// now project patch onto plane
	// ys = (y * d)/z
	// xs = (x * d)/z
	// zs = -1/z
			
	char areZValsBehind = 0;		// does polygon have all three verts with negative z?				
	CRadPatch *projected = p->copy();

	for(int i=0; i<3; i++) {		// calculate projected coords
		
		if( projected->vert[i].z < minDist ) {		// this vertex needs to be clipped
			// walk along edge towards +z to find a new vertex
			int nextInd = 1;
			
			// walk towards other vertex with greatest Z value
			// 	( this lets patches that share vertices remain in contact )
			float maxZ = -1;
			int indOfMaxZ = 0;
			
			for( nextInd = 1; nextInd < 3; nextInd++ ) {
				float thisZ = projected->vert[(i+nextInd)%3].z;
				if( thisZ > maxZ ) {
					maxZ = thisZ; 
					indOfMaxZ = nextInd;
					}
				}
				
			nextInd = indOfMaxZ;	
			if( maxZ < minDist ) {		// check if patch is entirely clipped
				areZValsBehind = 1;
				}
						
			// nextInd points to a vertext with non-negative z value
			
			if( !areZValsBehind ) {
				float deltaX = projected->vert[(i+nextInd)%3].x - projected->vert[i].x;
				float deltaY = projected->vert[(i+nextInd)%3].y - projected->vert[i].y;
				float deltaZ = projected->vert[(i+nextInd)%3].z - projected->vert[i].z;
				
				float invDeltaZ = 1 / deltaZ;
				float slopeX = deltaX * invDeltaZ;
				float slopeY = deltaY * invDeltaZ;
				
				float deltaToZero = minDist - projected->vert[i].z;
				
				// new x and y of vert created by clipping
				projected->vert[i].x = deltaToZero * slopeX + projected->vert[i].x;
				projected->vert[i].y = deltaToZero * slopeY + projected->vert[i].y;
			
				// new z is 0
				projected->vert[i].z = minDist;
				}
			}
		
		float invZ = 1 / projected->vert[i].z;
		
		projected->vert[i].x = (projected->vert[i].x * d) * invZ;
		projected->vert[i].y = (projected->vert[i].y * d) * invZ;
		projected->vert[i].z = -invZ;
		}		
	
	SMLVec3f *vert = projected->vert;
	
		
	// make sure this polygon has some verts with positive z vals
	if( ! areZValsBehind ) {
				
		int *maxRowX = new int[planePixHigh];
		int *minRowX = new int[planePixHigh];
		float *zOfMaxRowX = new float[planePixHigh];
		float *zOfMinRowX = new float[planePixHigh];
					
					
		for( int i=0; i<planePixHigh; i++) {
			maxRowX[i] = -planePixWide;
			minRowX[i] = planePixWide;
			zOfMaxRowX[i] = 1;
			zOfMinRowX[i] = 1;
			}
					
		
		float worldToPixUnit = planePixWide/(planeEndX - planeStartX);
		
		int planeStartXInPix = (int)(worldToPixUnit * planeStartX);		// start and end of view plane in pixels
		int planeEndXInPix = planeStartXInPix + planePixWide -1;
		
		int planeStartYInPix = (int)(worldToPixUnit * planeStartY);
		int planeEndYInPix = planeStartYInPix + planePixHigh -1;
		
		int yPixArrayOffset = 0 - planeStartYInPix;		// how much offset needed to get into positive array indices
		int xPixArrayOffset = 0 - planeStartXInPix;
		
					
		for( i=0; i<3; i++) {		// for each side of polygon
			int startXInPix = (int)(vert[i].x * worldToPixUnit);
			int startYInPix = (int)(vert[i].y * worldToPixUnit);
			int endXInPix = (int)(vert[(i+1)%3].x * worldToPixUnit);
			int endYInPix = (int)(vert[(i+1)%3].y * worldToPixUnit);
			
			float startZ = vert[i].z;
			float endZ = vert[(i+1)%3].z;
			
			
			
			// make sure y increasing along edge
			if( startYInPix > endYInPix) {
				int temp = startYInPix;
				startYInPix = endYInPix;
				endYInPix = temp;
				
				temp = startXInPix;
				startXInPix = endXInPix;
				endXInPix = temp;
				
				float tempf = startZ;
				startZ = endZ;
				endZ = tempf;
				}

			// watch for case where side is outside y range of viewplane
			if( endYInPix > planeStartYInPix && startYInPix < planeEndYInPix ) {

				// find max and min x contributions from this edge	
			
				float deltaX = endXInPix - startXInPix;
				float deltaY = endYInPix - startYInPix;
				float deltaZ = endZ - startZ;
						
				float invDeltaY = 1/deltaY;
						
				float slopeX = deltaX * invDeltaY;
				float slopeZ = deltaZ * invDeltaY;
						
				int realStartY = startYInPix;
				int realEndY = endYInPix;
						
				if( startYInPix < planeStartYInPix )  realStartY = planeStartYInPix;	
				if( endYInPix > planeEndXInPix )  realEndY = planeEndYInPix;
						
				int currentY = realStartY;
				float currentX = slopeX * (realStartY - startYInPix) + startXInPix;
				float currentZ = slopeZ * (realStartY - startYInPix) + startZ;
					
				// while in bounds
				while( currentY <= realEndY ) {
					
					int arrayIndex = currentY + yPixArrayOffset;
						
					// compare x against max for this row
					if( maxRowX[ arrayIndex ] < currentX ) {
						maxRowX[ arrayIndex ] = (int)(currentX);
						zOfMaxRowX[ arrayIndex ] = currentZ;
						}
					// compare x against min for this row
					if( minRowX[ arrayIndex ] > currentX ) {
						minRowX[ arrayIndex ] = (int)(currentX);
						zOfMinRowX[ arrayIndex ] = currentZ;
						}
					currentY++;
					currentZ += slopeZ;
					currentX += slopeX;
					}	// end while
				}	// end if in y range
			}	// end for all sides of poly
					
					
					
		// now have max and min X for each row, with Z values.... fill in the Z buffer
		
		
		// clip max and min values for each row 
		for( int y=0; y<planePixHigh; y++ ) {
			float startZ = zOfMinRowX[y];
			float endZ = zOfMaxRowX[y];
			float deltaZ = endZ - startZ;
			int deltaPix = maxRowX[y] - minRowX[y];
			float slopeZ = deltaZ/deltaPix;
			
			if( maxRowX[y] > planeEndXInPix ) {		// need to clip this max
				
				float deltaX = (float)(maxRowX[y] - planeEndXInPix);
				zOfMaxRowX[y] = zOfMaxRowX[y] - deltaX * slopeZ;
				
				maxRowX[y] = planeEndXInPix;
				}
			 
			 if( minRowX[y] < planeStartXInPix ) {		// need to clip this min
				
				float deltaX = (float)(minRowX[y] - planeStartXInPix);
				zOfMinRowX[y] = zOfMinRowX[y] - deltaX * slopeZ;
				
				minRowX[y] = planeStartXInPix;
				}
			}
								
		for( y=0; y<planePixHigh; y++) {		// for each row
			if( maxRowX[y] >= planeStartXInPix && minRowX[y] <= planeEndXInPix) {
				// this poly affects this row
				
				float startZ = zOfMinRowX[y];
				float deltaZ = zOfMaxRowX[y] - startZ;
				int deltaPix = maxRowX[y] - minRowX[y];
				float slopeZ = deltaZ/deltaPix;

				
				int startX = minRowX[y] + xPixArrayOffset;
				int endX = maxRowX[y] + xPixArrayOffset;
				
				float currentZ = startZ;
				for( int x=startX; x<=endX; x++) {		// for each pixel in row
					viewPlane[yOffset[y] + x].add( pID, currentZ, isBack);
					currentZ += slopeZ;
					} 
							
				}
					
			}	// end for each row					

		delete [] maxRowX;
		delete [] minRowX;
		delete [] zOfMaxRowX;
		delete [] zOfMinRowX;
		
		}	// end check for positive z vals	
	// patch has been added to zbuffer
		
	delete projected;
					
						
	} // end of zbufferpatch function