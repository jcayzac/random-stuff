#include "color.h"
#include "math.h"

CColor::CColor(): X(.0f), Y(.0f), Z(.0f) {

}

CColor::~CColor(void) {

}

const CColor& CColor::operator=(const CColor& c) {
	X = c.X;
	Y = c.Y;
	Z = c.Z;
	return *this;
}

// additif
CColor& CColor::operator+=(const CColor& c) {
	X += c.X;
	Y += c.Y;
	Z += c.Z;
	return *this;
}

CColor& CColor::operator+(const CColor& c) const {
	CColor* c3 = new CColor;
	c3->X = X+c.X;
	c3->Y = Y+c.Y;
	c3->Z = Z+c.Z;
	return *c3;
}

// teste l'egalite de deux couleurs
bool operator==(const CColor& c1, const CColor& c2) {
	return ((c1.X==c2.X) && (c1.Y==c2.Y) && (c1.Z==c2.Z));
}

// teste la luminance de deux couleurs
bool operator<(const CColor& c1, const CColor& c2) {
	return (c1.Y<c2.Y);
}

bool operator>(const CColor& c1, const CColor& c2) {
	return (c1.Y>c2.Y);
}

void CColor::set(unsigned wavelength) {
	if (wavelength<min_lambda) wavelength=min_lambda;
	if (wavelength>max_lambda) wavelength=max_lambda;
	unsigned i=wavelength-min_lambda;
	X = lambda_XYZ[i].X;
	Y = lambda_XYZ[i].Y;
	Z = lambda_XYZ[i].Z;
}

void CColor::set(float X, float Y, float Z) {
	this->X = X;
	this->Y = Y;
	this->Z = Z;
}

void CColor::addRange(unsigned w_min, unsigned w_max) {
	CColor t;
	if (w_min<min_lambda) w_min=min_lambda;
	if (w_max>max_lambda) w_max=max_lambda;
	for (unsigned w=w_min; w<=w_max; w++) {
		t.set(w);
		*this+=t;
	}
}
