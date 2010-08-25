
#if !defined(__RGBCOLOR_H) 
#define __RGBCOLOR_H

class CRGBColor {

	unsigned red;
	unsigned green;
	unsigned blue;

public:

	CRGBColor() { red = 0; green = 0; blue = 0; };
	void set(unsigned r, unsigned g, unsigned b) { red = r; green = g; blue = b; };
	unsigned getRed() { return red; };
	unsigned getGreen() { return green; };
	unsigned getBlue() { return blue; };
};


#endif