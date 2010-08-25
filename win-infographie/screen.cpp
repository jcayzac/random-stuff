#include <math.h>
#include <smlmath.h>
#include <smlxmatrix.h>
#include "screen.h"

CScreen::CScreen(EPhosphor p, EIlluminant i, float gamma, int size_inch, float ratio) {
	h = (size_inch-1)*(float)sqrt(1 + ratio*ratio)*inch;
	w = ratio*h;
	g = gamma;
	SMLXSpatialVector white(.0f,.0f,.0f);
	SMLXMatrix rgb_chromacities(3,3);
	switch(p) {
		default:
		case PHOSPHOR_SHORT:
			rgb_chromacities[0][0] = .61f; // xr
			rgb_chromacities[0][1] = .29f; // xg
			rgb_chromacities[0][2] = .15f; // xb
			rgb_chromacities[1][0] = .35f; // yr
			rgb_chromacities[1][1] = .59f; // yg
			rgb_chromacities[1][2] = .063f; // yb
			break;
		case PHOSPHOR_LONG:
			rgb_chromacities[0][0] = .62f; // xr
			rgb_chromacities[0][1] = .21f; // xg
			rgb_chromacities[0][2] = .15f; // xb
			rgb_chromacities[1][0] = .33f; // yr
			rgb_chromacities[1][1] = .685f; // yg
			rgb_chromacities[1][2] = .063f; // yb
			break;
		case PHOSPHOR_NTSC:
			rgb_chromacities[0][0] = .67f; // xr
			rgb_chromacities[0][1] = .21f; // xg
			rgb_chromacities[0][2] = .14f; // xb
			rgb_chromacities[1][0] = .33f; // yr
			rgb_chromacities[1][1] = .71f; // yg
			rgb_chromacities[1][2] = .08f; // yb
			break;
		case PHOSPHOR_EBU:
			rgb_chromacities[0][0] = .64f; // xr
			rgb_chromacities[0][1] = .3f; // xg
			rgb_chromacities[0][2] = .15f; // xb
			rgb_chromacities[1][0] = .33f; // yr
			rgb_chromacities[1][1] = .6f; // yg
			rgb_chromacities[1][2] = .06f; // yb
			break;
		case PHOSPHOR_SMPTE:
			rgb_chromacities[0][0] = .63f; // xr
			rgb_chromacities[0][1] = .31f; // xg
			rgb_chromacities[0][2] = .155f; // xb
			rgb_chromacities[1][0] = .34f; // yr
			rgb_chromacities[1][1] = .595f; // yg
			rgb_chromacities[1][2] = .07f; // yb
			break;
	};
	rgb_chromacities[2][0] = 1-(rgb_chromacities[0][0]+rgb_chromacities[1][0]); // zr
	rgb_chromacities[2][1] = 1-(rgb_chromacities[0][1]+rgb_chromacities[1][1]); // zg
	rgb_chromacities[2][2] = 1-(rgb_chromacities[0][2]+rgb_chromacities[1][2]); // zb
	switch (i) {
		case ILLU_D50:	white.vec[0]=.3457f; white.vec[1]=.3585f; break;
		case ILLU_5335:	white.vec[0]=.3362f; white.vec[1]=.3502f; break;
		case ILLU_StdE:	white.vec[0]=.3333f; white.vec[1]=.3333f; break;
		case ILLU_D55:	white.vec[0]=.3324f; white.vec[1]=.3474f; break;
		default:
		case ILLU_D65:	white.vec[0]=.3127f; white.vec[1]=.3290f; break;
		case ILLU_StdC:	white.vec[0]=.3101f; white.vec[1]=.3162f; break;
		case ILLU_D75:	white.vec[0]=.2990f; white.vec[1]=.3149f; break;
		case ILLU_D93:	white.vec[0]=.2848f; white.vec[1]=.2932f; break;
	};
	// cree la matrice de tranformation XYZ->RGB
	// transforme (xw,yw) en (Xw,Yw,Zw)
	white.vec[2] = (1.0f - (white.vec[0]+white.vec[1]))/white.vec[1];
	white.vec[0] /= white.vec[1];
	white.vec[1] = 1.0f;
	// matrice inverse de rgb_chromacities
	SMLXMatrix T1 = rgb_chromacities;
	T1.Invert();
	// matrice D = T1*white
	SMLXMatrix D = T1*white;
if ((D.ncols()!=1) && (D.nrows()!=3)) debug("D pas correct\n");
else debug("D correct\n");
	SMLXMatrix D3(3,3);
	D3[0][0]=D[0][0];
	D3[1][1]=D[1][0];
	D3[2][2]=D[2][0];
	SMLXMatrix T2 = rgb_chromacities*D3;
	T2.Invert();
	XYZtoRGB = T2;
};

const SMLVec3f& CScreen::getColorRGB(const CColor& c) {
	SMLXSpatialVector XYZ(c.X,c.Y,c.Z);
	SMLXSpatialVector RGB = XYZtoRGB*XYZ;
	SMLVec3f *rgb = new SMLVec3f;
	rgb->x = RGB.vec[0];
	rgb->y = RGB.vec[1];
	rgb->z = RGB.vec[2];
	if (rgb->x<0.f) rgb->x = .0f;
	if (rgb->y<0.f) rgb->y = .0f;
	if (rgb->z<0.f) rgb->z = .0f;
	if (rgb->x>1.0f) rgb->x = 1.0f;
	if (rgb->y>1.0f) rgb->y = 1.0f;
	if (rgb->z>1.0f) rgb->z = 1.0f;
	return *rgb;
}
