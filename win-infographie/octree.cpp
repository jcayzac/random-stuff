#include "octree.h"
#include "scene.h"


CSpace::CSpace(bool root): face(NULL), leaf(NULL) {
	if (root) {
		min.Set(infinity,infinity,infinity);
		max.Set(-infinity,-infinity,-infinity);
		for (unsigned f=0; f<scene.nb_faces; f++) {
			for (unsigned v=0; v<3; v++) {
				if (scene.faces[f].vert[v].x<min.x) min.x = scene.faces[f].vert[v].x;
				if (scene.faces[f].vert[v].y<min.y) min.y = scene.faces[f].vert[v].y;
				if (scene.faces[f].vert[v].z<min.z) min.z = scene.faces[f].vert[v].z;
				if (scene.faces[f].vert[v].x>max.x) max.x = scene.faces[f].vert[v].x;
				if (scene.faces[f].vert[v].y>max.y) max.y = scene.faces[f].vert[v].y;
				if (scene.faces[f].vert[v].z>max.z) max.z = scene.faces[f].vert[v].z;
			}
		}
		nb_faces = scene.nb_faces;
		face = new unsigned[nb_faces];
		for (f=0; f<nb_faces; f++) face[f]=f;
	}
	else {
		min.Set(infinity,infinity,infinity);
		max.Set(-infinity,-infinity,-infinity);
	}
}

CSpace::CSpace(const SMLVec3f& min, const SMLVec3f& max): face(NULL), leaf(NULL) {
	this->min = min;
	this->max = max;
}

CSpace::~CSpace(void) {
	if (face!=NULL) delete[] face;
	nb_faces = 0;
	if (leaf!=NULL) delete[] leaf;
}

inline bool CSpace::intersects(const CRay& r) {
// from Graphics Gems I
	bool inside = true;
	char quadrant[3];
	register int i;
	int whichPlane;
	float maxT[3];
	float candidatePlane[3];
	float* minA = min.data();
	float* maxA = max.data();
	const float* startA = r.start.data();
	const float* dirA = r.direction.data();

	// Find candidate planes; this loop can be avoided if
   	// rays cast all from the eye(assume perpsective view)
	for (i=0; i<3; i++)
		if(startA[i] < minA[i]) {
			quadrant[i] = 1;
			candidatePlane[i] = minA[i];
			inside = false;
		}
		else if (startA[i] > maxA[i]) {
			quadrant[i] = 0;
			candidatePlane[i] = maxA[i];
			inside = false;
		}
		else quadrant[i] = 2;

	/* Ray origin inside bounding box */
	if(inside) return true;

	/* Calculate T distances to candidate planes */
	for (i = 0; i<3; i++)
		if (quadrant[i] != 2 && dirA[i] !=0.)
			maxT[i] = (candidatePlane[i]-startA[i]) / dirA[i];
		else maxT[i] = -1.;

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i<3; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) return false;
	for (i = 0; i<3; i++)
		if (whichPlane != i) {
			float coord = startA[i] + maxT[whichPlane] * dirA[i];
			if (coord < minA[i] || coord > maxA[i])
				return false;
		}
	return true;
}

inline void	CSpace::check(void) {
	register float t;
	if (min.x>max.x) { t=min.x; min.x = max.x; max.x = t; }
	if (min.y>max.y) { t=min.y; min.y = max.y; max.y = t; }
	if (min.z>max.z) { t=min.z; min.z = max.z; max.z = t; }
	min.x -= epsilon;
	min.y -= epsilon;
	min.z -= epsilon;
	max.x += epsilon;
	max.y += epsilon;
	max.z += epsilon;
}

inline bool CSpace::isInto(const SMLVec3f& v) {
	return ((v.x>=min.x) && (v.x<=max.x) && (v.y>=min.y) && (v.y<=max.y) && (v.z>=min.z) && (v.z<=max.z));
}

void CSpace::iterate(void) {
	if (leaf!=NULL) {
		for (register unsigned i=0; i<8; i++)
			leaf[i].iterate();
	}
	else if (nb_faces>0) {
		// builds leaves
		leaf = new CSpace[8];
		SMLVec3f med = (min+max)/2.0f;
		leaf[0].min.x = min.x;
		leaf[0].min.y = med.y;
		leaf[0].min.z = med.z;
		leaf[0].max.x = med.x;
		leaf[0].max.y = max.y;
		leaf[0].max.z = max.z;
		leaf[1].min.x = med.x;
		leaf[1].min.y = med.y;
		leaf[1].min.z = med.z;
		leaf[1].max.x = max.x;
		leaf[1].max.y = max.y;
		leaf[1].max.z = max.z;
		leaf[2].min.x = min.x;
		leaf[2].min.y = min.y;
		leaf[2].min.z = med.z;
		leaf[2].max.x = med.x;
		leaf[2].max.y = med.y;
		leaf[2].max.z = max.z;
		leaf[3].min.x = med.x;
		leaf[3].min.y = min.y;
		leaf[3].min.z = med.z;
		leaf[3].max.x = max.x;
		leaf[3].max.y = med.y;
		leaf[3].max.z = max.z;
		leaf[4].min.x = min.x;
		leaf[4].min.y = med.y;
		leaf[4].min.z = min.z;
		leaf[4].max.x = med.x;
		leaf[4].max.y = max.y;
		leaf[4].max.z = med.z;
		leaf[5].min.x = med.x;
		leaf[5].min.y = med.y;
		leaf[5].min.z = min.z;
		leaf[5].max.x = max.x;
		leaf[5].max.y = max.y;
		leaf[5].max.z = med.z;
		leaf[6].min.x = min.x;
		leaf[6].min.y = min.y;
		leaf[6].min.z = min.z;
		leaf[6].max.x = med.x;
		leaf[6].max.y = med.y;
		leaf[6].max.z = med.z;
		leaf[7].min.x = med.x;
		leaf[7].min.y = min.y;
		leaf[7].min.z = min.z;
		leaf[7].max.x = max.x;
		leaf[7].max.y = med.y;
		leaf[7].max.z = med.z;
		// count faces
		register unsigned count[8];
		register bool* pushed = new bool[nb_faces];
		register unsigned pushed_faces=0;
		for (register unsigned b=0; b<nb_faces; b++)
			pushed[b]=false;
		for (b=0; b<8; b++) {
			count[b]=0;
			leaf[b].check();
		}
		for (register unsigned f=0; f<nb_faces; f++) {
			for (b=0; b<8; b++) {
				// si un point de la face est dans cette feuille, on l'y met
				if (leaf[b].isInto(scene.faces[face[f]].vert[0]) || leaf[b].isInto(scene.faces[face[f]].vert[1]) || leaf[b].isInto(scene.faces[face[f]].vert[2])) {
					count[b]++;
					// mark face
					pushed[f] = true;
				}
			}
			// if the face is marked, increment pushed_faces
			if (pushed[f]) pushed_faces++;
		}
		// alloc faces
		for (b=0; b<8; b++) {
			if (count[b]!=0) leaf[b].face = new unsigned[count[b]];
			leaf[b].nb_faces = count[b];
		}
		unsigned* new_face = new unsigned[nb_faces-pushed_faces];
		// dispatch'em
		register unsigned new_index=0;
		for (b=0; b<8; b++) count[b]=0;
		for (f=0; f<nb_faces; f++) {
			// if the face is marked, it has to be moved to a leaf
			if (pushed[f]) {
				for (b=0; b<8; b++) {
					if (leaf[b].isInto(scene.faces[face[f]].vert[0]) || leaf[b].isInto(scene.faces[face[f]].vert[1]) || leaf[b].isInto(scene.faces[face[f]].vert[2])) {
						leaf[b].face[count[b]++] = f;
					}
				}
			}
			// else it stays at this level
			else new_face[new_index++] = face[f];
		}
		// clear current node
		delete[] face;
		delete[] pushed;
		face = new_face;
		nb_faces -= pushed_faces;
	}
}

void CSpace::traverse(CRay& r) {
	// if there are faces at that level
	if (nb_faces>0) {
		for (unsigned f=0; f<nb_faces; f++) scene.faces[face[f]].intersection(r,face[f]);
	}
	// process to the next level
	if (leaf!=NULL) {
		for (unsigned f=0; f<8; f++) {
			if (leaf[f].intersects(r)) leaf[f].traverse(r);
		}
	}
}

void CSpace::print(void) {
	char dummy[512];
	if (leaf==NULL)	{
		sprintf(dummy, "* %u faces\n", nb_faces);
		debug(dummy);
	}
	else {
		debug("{\n");
		for (unsigned f=0; f<8; f++) {
			leaf[f].print();
		}
		debug("}\n");
	}
}
