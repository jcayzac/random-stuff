#if !defined(__FACE_H)
#define __FACE_H
#include <smlmath.h>
#include "radpatch.h"
#include "ray.h"

typedef struct {
	float u,v;
} Tuv;

class CFace: public CRadPatch {
public:
	float		plane_value;
	unsigned	mat_ind;
	unsigned	pt_ind[3];
	SMLVec3f	pt_norm[3];
				CFace(void);
	virtual		~CFace(void);
	const Tuv&	getUV(const SMLVec3f& p);
	bool		intersection(CRay& ray);
	void		compute_vertices_normals_flat(void);
	virtual const CFace& operator=(const CFace& src);
};

#endif
