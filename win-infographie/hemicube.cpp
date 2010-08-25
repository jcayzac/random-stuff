#include "commode.h"
#include "Hemicube.h"
#include <math.h>

CHemicube::CHemicube(float worldW) {
	
	worldWide = worldW;
	float cubeWitdh = worldW;
	int sizeOfRegion = cubeWitdh / 2;	// = size of upper left corner of cube top
										// = size of left half of cube side 
	
	int pixInRegion = sizeOfRegion * sizeOfRegion;
	
	float* regionBuffer1 = new float[pixInRegion];	 // region buffer, filled and flushed to file twice
	
	// first region, upper left corner of cube top
	for( int buffY= sizeOfRegion - 1; buffY>=0; buffY--) {
		for( int buffX = 0; buffX < sizeOfRegion; buffX++) {
			float x = buffX - sizeOfRegion - 0.5;	// fix center of pixel error with 0.5 offset
			float y = buffY + 0.5;
			float temp = x*x + y*y + sizeOfRegion * sizeOfRegion;
			temp = temp * temp;
				
			regionBuffer1[buffY * sizeOfRegion + buffX] = (sizeOfRegion * sizeOfRegion) / (pi * temp);
		}
	}
	
	float* regionBuffer2 = new float[pixInRegion];		// second region, left half of cube side
	for( buffY= sizeOfRegion - 1; buffY>=0; buffY--) {
		for( int buffX = 0; buffX < sizeOfRegion; buffX++) {
			float x = buffX - sizeOfRegion - 0.5;		// fix center of pixel error with 0.5 offset
			float y = buffY + 0.5;
			float temp = x*x + sizeOfRegion * sizeOfRegion;
			float temp2 = temp + y*y;
			temp2 = temp2 * temp2;
			temp = sqrt(temp);
				
			regionBuffer2[buffY * sizeOfRegion + buffX] = (y * temp) / (pi * temp2);
		}
	}
	
	// regionBuffer 1 and 2 created
	
	pixelsWide = cubeWitdh;
	
	// precalc offsets
	yOffsetPixels = new int[pixelsWide];
	
	for( int y=0; y<pixelsWide; y++) {
		int offset = y*pixelsWide;
		yOffsetPixels[y] = offset;
	}
		
	int regionSize = pixelsWide / 2;	
	
	int numPixelsTop = pixelsWide * pixelsWide;
	int numPixelsSide = numPixelsTop / 2;
	
	int numPixelsRegion = numPixelsSide / 2;
	
	// init arrays
	topPixels = new float[ numPixelsTop ];
	
	// note that form factors for corresponding pixels on each side are identical
	// side 0 only side that's actually an array
	sidePixels[0] = new float[ numPixelsSide ];	


	
	// read first region into top
	// must read one row at a time...
	int cpt = 0;
	for(int r=0; r<regionSize; r++) {
		// new way (using UniversalFileIO): must read one pixel at a time
		for( int q=0; q<regionSize; q++) {
			topPixels[yOffsetPixels[r] + q] = regionBuffer1[cpt++];
		}
	}


	// read second region into side 0
	// must read one row at a time...
	cpt = 0;
	for(r=0; r<regionSize; r++) {
		// new way (using UniversalFileIO): must read one pixel at a time
		for( int q=0; q<regionSize; q++) {
			sidePixels[0][yOffsetPixels[r] + q] = regionBuffer2[cpt++];
		}
	}
	
	
	// copy into rest of top, and into sides
	for(int fileY =0; fileY < regionSize; fileY++) {
		for( int fileX = 0; fileX < regionSize; fileX++) {		// mirror left to right
			topPixels[yOffsetPixels[fileY] + pixelsWide - fileX - 1] = topPixels[yOffsetPixels[fileY] + fileX];
			sidePixels[0][yOffsetPixels[fileY] + pixelsWide - fileX - 1] = sidePixels[0][yOffsetPixels[fileY] + fileX];		// side 0
			}
		for( fileX = 0; fileX < pixelsWide; fileX++) {		// mirror top to bottom
			topPixels[yOffsetPixels[pixelsWide - fileY -1] + fileX] = topPixels[yOffsetPixels[fileY] + fileX];
			}
		}
	
	for(int i=0; i<4; i++) {	// put pointer to side 0 in other sides
		sidePixels[i] = sidePixels[0];
		}	
		
	}