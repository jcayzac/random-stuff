#if !defined(__HEMI_H)
#define __HEMI_H
#include <smlmath.h>
#include "radpatch.h"


#define SEUIL_FORMFACTOR .2000f

// hemisphere
class CHemi {
	SMLVec3f *ffgrid;
	unsigned divisions;
	
public:
	CHemi(unsigned precision);
	virtual ~CHemi(void);

	void calculateFormFactors(CRadPatch* patch);
	void addLink(CLink& link);

	CLink* links;
	int    nb_links;


};

#endif
