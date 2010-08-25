#if !defined(__OBJECT_H)
#define __OBJECT_H

#include <math.h>
#include <smlmath.h>
#include "face.h"
#include "material.h"

typedef unsigned index;

typedef struct {
	float Xmin,Ymin,Zmin,Xmax,Ymax,Zmax;
} boundingbox;

class C3DObject {
	SMLVec3f		origin;
	SMLVec3f		orientation;
	SMLVec3f		size;
public:
	CMaterial*		material;
	boundingbox		boundings;
	unsigned		nb_verts;
	SMLVec3f*		vertices;
	unsigned		nb_faces;
	CFace*			faces;
					C3DObject(void);
	virtual			~C3DObject(void);
	void			compute_bbox(void);
	void			set(C3DObject& o);
	void			add(C3DObject& o);
	void			inverse(void);
	void			compute_normals_from_vertices(void);
	void			compute_normals(void);
	void			joint_double(void);
	void			smoothen(const bool sm);
	void			dump(const char* filename);
	//
	void			setOrigin(float x, float y, float z);
	void			setSize(float sx, float sy, float sz);
	void			setOrientation(float rx, float ry, float rz);
	C3DObject*		getWorldObject(void);
};

#endif
