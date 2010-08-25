#if !defined(__RADPATCH_H)
#define __RADPATCH_H
#include <smlmath.h>


class CLink;

class CRadPatch {
public:
    SMLVec3f		Bg;		// GATHERED RADIOSITY (ACCUMULATED, RESULTING)
    SMLVec3f		Bs;		// SHOOTING RADIOSITY (UNSHOT)
    SMLVec3f		E;		// EMISSION (LIGHT SOURCE INTENSITY)
    SMLVec3f		R;		// REFLECTIVITY ("COLOR" OF THE PATCH)

    float			area;	// AREA OF THE PATCH
    SMLVec3f		norm;	// NORMAL OF THE PATCH
    SMLVec3f		center;	// CENTER OF PATCH (AVERAGE OF VERTS)

	CRadPatch*		quad;

	CRadPatch*		root;
	SMLVec3f*		vert;
	unsigned		sub_level;
					CRadPatch(void);
	virtual			~CRadPatch(void);
	bool			subdivide(void);

	CRadPatch*		getLeaf();

	virtual const	CRadPatch& operator=(const CRadPatch&);
};

class CLink {
public:
	float			F;	// formfactor avec  q
	CRadPatch*		q;	// element lié
};




#define MAX_SUBLEVEL	3

#endif
