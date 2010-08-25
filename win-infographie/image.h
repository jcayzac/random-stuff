#if !defined(__IMAGE_H)
#define __IMAGE_H

#include "commode.h"
#include "colorRGB.h"

class CImage {
	CRGBColor *data;
	uint32 width;
	uint32 height;

public:
	CImage(uint32 w, uint32 h) {

		width = w; height=h; uint32 sz = w*h;
		data = new CRGBColor[sz];
		for (uint32 curs=0; curs<sz; curs++) {
			data[curs].set(0,0,0);
		}
	}

	CImage(void) {
		width=0; height=0; data=NULL;
	}

	~CImage(void) {
		if is(data) delete[] data;
	}

	// accessors
	CRGBColor&	GetColor(uint32 x, uint32 y) const { return data[x + y*width]; };
	uint32	GetWidth(void)  const { return width; };
	uint32	GetHeight(void) const { return height; };
	CRGBColor* GetBuffer(void) const { return data; };
	void SetBuffer(CRGBColor* b) { if is(data) delete[] data; data = b; };
	bool writeTGA(string filename);
	bool readTGA(string filename);
};

#endif