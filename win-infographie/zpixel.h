
#ifndef ZPIXEL_INCLUDED
#define ZPIXEL_INCLUDED

#include <float.h>

class CZPixel {
	
	public:
		
		
		// center of top, Normal of top, pointer to hemicube
		CZPixel();
		
		int getObjectID();
		float getZValue();
		char getIsBack();
		
		void add(int oID, float z, char back);		// add into this Z pixel with comparisson to current value
		
	private :
	
		int objectID;
		float zValue;
		char isBack;		// is the back side of this patch facing the z buffer
		// back info is used to tell when an object may be blocking other objects
		// in the buffer, but not contributing light (or whatever) to image
	};







inline CZPixel::CZPixel() {
	objectID = -1;
	zValue = +FLT_MAX;
	isBack = true;
	}	

inline void CZPixel::add(int oID, float z, char back) {
	if( zValue > z ) {
		zValue = z;
		objectID = oID;
		isBack = back;
		}
	}

inline int CZPixel::getObjectID() {
	return objectID;
	}
	
inline float CZPixel::getZValue() {
	return zValue;
	}
	
inline char CZPixel::getIsBack() {
	return isBack;
	}	
	
#endif