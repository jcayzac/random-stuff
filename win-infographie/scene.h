#if !defined(__SCENE_H)
#define __SCENE_H
#include <smlmath.h>
#include <string>
#include "object.h"
#include "material.h"
#include "screen.h"
#include "image.h"
using namespace std;

class CScene {
	unsigned	vert_heap;
	unsigned	mat_heap;
	unsigned	face_heap;
	void		increaseVHeap(void);
	void		increaseMHeap(void);
	void		increaseFHeap(void);
	unsigned	addFace(const CFace& f);
	unsigned	addVertex(const SMLVec3f& v);
	// preparation des faces pour la radiosite
	void		lock(void);
	// subdivision des faces de la scene
	void		subdivideAll(void);
	void		subdivideIntel(void);
	// radiosite :matrice des FF
	void		calculateFFMatrix(void);

	// lancer de rayons
	void		trace(CRay ray, CColor& col, unsigned level);
public:
	unsigned	nb_verts;
	SMLVec3f	*vertices;
	unsigned	nb_mats;
	CMaterial	*materials;
	unsigned	nb_faces;
	CFace		*faces;
	//
				CScene(void);
	virtual		~CScene(void);
	void		addObject(const C3DObject& o);
	unsigned	addMaterial(const CMaterial& m);
	unsigned	getMaterialIndex(const string& name);
	void		dump(const char* filename);
	void		render(const CScreen& screen, const CImage& image);
};

#define MATERIAL_NOT_FOUND		0xdeaddead
#define VERTICE_HEAP_INCREMENT	500
#define FACE_HEAP_INCREMENT		500
#define MATERIAL_HEAP_INCREMENT	50

extern CScene scene;
extern CScene* scn;

#endif
