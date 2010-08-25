
#ifndef __HEMICUBE_H
#define __HEMICUBE_H


class CHemicube {
	
	public:
		
		
		CHemicube(float worldW);
		
		~CHemicube();
		
		// side number (top side 0), pixel coords
		float getFormFactor(int side, int pixX, int pixY) ;	// get form factor of pixel on one of the sides
		// if pixY = -1, function will access pixels in cube side linearly using pixX
		
		float getWorldWide();
		int getPixelsWide();
		
	private :
	
		float worldWide;
		int pixelsWide;
		float *topPixels;		// form factors
		float *sidePixels[4];	
		int *yOffsetPixels;		// precalculated array indices
	};
	
	






	
inline CHemicube::~CHemicube() {
	delete [] yOffsetPixels;
	delete [] topPixels;
	delete [] sidePixels[0];
	}
	


	
inline float CHemicube::getFormFactor(int side, int pixX, int pixY = -1) {
	
	if( pixY == -1) {	// access pixels linearly using pixX
		if( side == 0) {
			return topPixels[pixX];
			}
		if( side > 0) {
			return sidePixels[side-1][pixX];
			}
		return 0;
		}
	
	// else access pixels as 2d array by using yOffset[] precalcs 
		
	if( side == 0) {
		return topPixels[yOffsetPixels[pixY] + pixX];
		}
	if( side > 0) {
		return sidePixels[side-1][yOffsetPixels[pixY] + pixX];
		}
			
	return 0;
	}




inline float CHemicube::getWorldWide() {
	return worldWide;
	}




inline int CHemicube::getPixelsWide() {
	return pixelsWide;
	}
	
	
	
#endif