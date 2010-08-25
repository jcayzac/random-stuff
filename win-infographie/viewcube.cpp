
#include "viewcube.h"
#include "radpatch.h"


// constructor
CViewCube::CViewCube(SMLVec3f cVert, SMLVec3f N, CHemicube *h) {
	centVert = cVert;
	norm = N;
	hCube = h;
	
	numPixelsWide = hCube->getPixelsWide();
	
	
	// init arrays
	int numPixelsTop = numPixelsWide * numPixelsWide;
	
	topPixels = new CZPixel[ numPixelsTop ];
	
	int numPixelsSide = numPixelsTop / 2;
	
	for( int i=0; i<4; i++) {
		sidePixels[i] = new CZPixel[ numPixelsSide ];
		}
	
	worldToPixUnit = numPixelsWide / hCube->getWorldWide();
		
	// precalc y offsets into pixel planes
	yOffset = new int[numPixelsWide];
	
	for( int y=0; y<numPixelsWide; y++ ) {
		yOffset[y] = numPixelsWide * y;
		}
		
	// precalculate several values
		SMLVec3f cubeCenter(0,0,0);
		deltaCenter = cubeCenter - centVert;
		
		// calculate rotation needed to line normal up with Z+
		thetaX = 0;
		thetaY = 0;
////////////////////////////////////////////////////////////////////////	norm.calcRotationToZ( thetaX, thetaY );
		
		halfWorldWide = (hCube->getWorldWide()) * 0.5;
		startXWorld = - halfWorldWide;
		endXWorld = halfWorldWide;
		startYWorld = - halfWorldWide;
		endYWorld =  halfWorldWide;
		startSideYWorld = 0;
	}
	
CViewCube::~CViewCube() {
	delete [] topPixels;
	for( int i=0; i<4; i++) {
		delete [] sidePixels[i];
		}
	delete [] yOffset;
	}
	
	

void CViewCube::projPatchOnCube(int pID, CRadPatch *pIn) {
	
	// make a copy of the patch to preserver original
	CRadPatch *p = pIn->copy();
	
	// for top:		(special case)
			

		// move patch so that cube center is at (0,0,0)
		p->move(deltaCenter);		// move the patch


		// Now rotate patch so that cube normal lines up with Z+
		p->rotateX(thetaX);	
		p->rotateY(thetaY);
		
					
		// add this patch to the top z-buffer
		CzBufferPatch(topPixels, yOffset, p, pID, numPixelsWide, numPixelsWide,
							startXWorld, endXWorld, 
							startYWorld, endYWorld,
							halfWorldWide);
								
			
	// now rotate patch and cube so that side[0] normal points along Z+
	// rotate clockwise 90 degrees around y axis
	
	float delTheta = pi/2;
	p->rotateY(-delTheta);
	p->rotateZ(-delTheta);		// now x and y dirs on side[0] match world x, y dirs
	
	int yPixWide = numPixelsWide>>1;
	
	// note:  can use the same y offsets for top and sides
	
	// add this patch to side[0] z-buffer
	CzBufferPatch(sidePixels[0], yOffset, p, pID, numPixelsWide, yPixWide,
						startXWorld, endXWorld,
						startSideYWorld, endYWorld,
						halfWorldWide);		
				
	for( int s=1; s<4; s++) {		// for each remaining side
		
		// rotate 90 deg. in cw y direction to make side face z+
		
		p->rotateY(-delTheta);
		// add this patch to side[s] z-buffer
		CzBufferPatch(sidePixels[s], yOffset, p, pID, numPixelsWide, yPixWide,
						startXWorld, endXWorld,
						startSideYWorld, endYWorld,
						halfWorldWide);
		}
	
	// delete the copy of the patch
	delete p;
		
	}	// end projPatchOnCube()
	
	
	
	
void CViewCube::sumFormFactors(float *formFactors) {
		
	// look in top of cube
	int pixWideTop = hCube->getPixelsWide();
	int pixInTop = pixWideTop * pixWideTop;
		
	for( int pix=0; pix<pixInTop; pix++) {
		int id = topPixels[pix].getObjectID();
		// if an object is in buffer at this point, and it is facing the buffer
		if( id >= 0 && !(topPixels[pix].getIsBack()) ) {		
			formFactors[id] += hCube->getFormFactor(0, pix, -1);
			}
		}
			
	int pixHighSide = pixWideTop >> 2;		
	int pixInSide = pixHighSide * pixWideTop;
	
	for( int s=0; s<4; s++) {	// look in each cube side
		for( int pix=0; pix<pixInSide; pix++) {
			int id = sidePixels[s][pix].getObjectID();
			// if an object is in buffer at this point, and it is facing the buffer
			if( id >= 0 && !(sidePixels[s][pix].getIsBack()) ){
				formFactors[id] += hCube->getFormFactor(s+1, pix, -1);
				}
			}
		}	
	
	}	// end sumFormFactors()
	
/*	
void CViewCube::print() {
	printf("view cube top z buffer contents:\n");
	for( int y=0; y<numPixelsWide; y++) {
		int yOffset = y * numPixelsWide;
		for(int x=0; x<numPixelsWide; x++) {
			int val = topPixels[yOffset + x].getObjectID();
			if(val == -1) printf(" x  "); 
			else { 
				printf("%2d", val);
				if( topPixels[yOffset + x].getIsBack() ) {
					printf("b ");
					}
				else{
					printf("  ");
					}	
				}
			}
		printf("\n");
		}
		
	for( int s=0; s<4; s++ ) {		// side number to print	
		printf("view cube side %d z buffer contents:\n", s);
		int pixHighSide = numPixelsWide >> 2;
		for( int y=0; y<pixHighSide; y++) {
			int yOffset = y * numPixelsWide;
			for(int x=0; x<numPixelsWide; x++) {
				int val = sidePixels[s][yOffset + x].getObjectID();
				if(val == -1) printf(" x  "); 
				else { 
					printf("%2d", val);
					if( topPixels[yOffset + x].getIsBack() ) {
						printf("b ");
						}
					else{
						printf("  ");
						}	
					}	
				}
			printf("\n");
			}
		}
		
	printf("\n\n");
	}
*/