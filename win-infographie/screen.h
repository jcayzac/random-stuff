#if !defined(__SCREEN_H)
#define __SCREEN_H

#include <smlmath.h>
#include <smlxmatrix.h>
#include "commode.h"
#include "color.h"

typedef enum {
	PHOSPHOR_SHORT,
	PHOSPHOR_LONG,
	PHOSPHOR_NTSC,
	PHOSPHOR_EBU,
	PHOSPHOR_SMPTE
} EPhosphor;

typedef enum {
	ILLU_D50,
	ILLU_5335,
	ILLU_StdE,
	ILLU_D55,
	ILLU_D65,
	ILLU_StdC,
	ILLU_D75,
	ILLU_D93
} EIlluminant;

class CScreen {
	float			g;	 // gamma
	float			w,h;	 // width, height
public:
	SMLXMatrix		XYZtoRGB;
					CScreen(EPhosphor p, EIlluminant i, float gamma, int size_inch, float ratio);
	const SMLVec3f&	getColorRGB(const CColor& c);
};

#endif
