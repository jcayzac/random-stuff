#if !defined(__RAY_H)
#define __RAY_H
#include <smlmath.h>
#include "radpatch.h"

class CRay: public SMLRay {
public:
	CRay(const SMLVec3f& origin, const SMLVec3f& direction);
	bool intersection(CRadPatch* patch);
};

#endif
