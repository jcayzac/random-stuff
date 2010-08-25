#if !defined(__OCTREE_H)
#define __OCTREE_H

#include "ray.h"
#include "face.h"
#include "commode.h"

class CSpace {
	bool		isInto(const SMLVec3f& v);
	void		check(void);
public:
				CSpace(bool root=false);
				CSpace(const SMLVec3f& min, const SMLVec3f& max);
	virtual		~CSpace(void);
	bool		intersects(const CRay& r);
	void		iterate(void);
	void		traverse(CRay& r);
	void		print(void);
	unsigned	nb_faces;
	unsigned*	face;
	CSpace*		leaf;
	SMLVec3f	min, max;
};


#endif
