#include <math.h>
#include <smlmath.h>
#include <smlxmatrix.h>
#include "commode.h"
#include "object.h"

C3DObject::C3DObject(void): faces(NULL),vertices(NULL),nb_verts(0),nb_faces(0),material(NULL) {
	// no rotation
	orientation.z = orientation.y = orientation.x = .0f;
	// no translation
	origin.z = origin.y = origin.x = .0f;
	// default size = unit
	size.z = size.y = size.x = 1.0f;
}

// detruit l'objet
C3DObject::~C3DObject(void) {
	delete[] faces;
	delete[] vertices;
}

// calcule l'espace occupe par l'objet
void C3DObject::compute_bbox(void) {
	boundings.Xmin = infinity;
	boundings.Ymin = infinity;
	boundings.Zmin = infinity;
	boundings.Xmax = -infinity;
	boundings.Ymax = -infinity;
	boundings.Zmax = -infinity;
	for (int v=0; v<nb_verts; v++) {
		if (vertices[v].x<boundings.Xmin) boundings.Xmin = vertices[v].x;
		if (vertices[v].x>boundings.Xmax) boundings.Xmax = vertices[v].x;
		if (vertices[v].y<boundings.Ymin) boundings.Ymin = vertices[v].y;
		if (vertices[v].y>boundings.Ymax) boundings.Ymax = vertices[v].y;
		if (vertices[v].z<boundings.Zmin) boundings.Zmin = vertices[v].z;
		if (vertices[v].z>boundings.Zmax) boundings.Zmax = vertices[v].z;
	}
}

// ecrase l'objet avec un autre
void C3DObject::set(C3DObject& o) {
	if (faces)		delete[] faces;
	if (vertices)	delete[] vertices;
	nb_verts	= o.nb_verts;
	nb_faces	= o.nb_faces;
	vertices	= new SMLVec3f[nb_verts];
	faces		= new CFace[nb_faces];
	for (int v=0; v<o.nb_verts; v++) {
		vertices[v] = o.vertices[v];
	}
	for (int f=0; f<o.nb_faces; f++) {
		faces[f] = o.faces[f];
	}
	material  = o.material;
	boundings = o.boundings;
	orientation = o.orientation;
	origin		= o.origin;
	size        = o.size;
}

void C3DObject::add(C3DObject& o) {
	unsigned nverts = nb_verts+o.nb_verts;
	unsigned nfaces = nb_faces+o.nb_faces;

	SMLVec3f*	newvertices	= new SMLVec3f[nverts];
	CFace*		newfaces	= new CFace[nfaces];

	index nv=0;
	for (index v=0; v<nb_verts; v++)	newvertices[v] = vertices[v];
	for (v=0; v<o.nb_verts; v++)		newvertices[nb_verts+v] = o.vertices[v];

	for (index f=0; f<nb_faces; f++)	newfaces[f] = faces[f];
	for (f=0; f<o.nb_faces; f++) {
		newfaces[nb_faces+f] = o.faces[f];
		for (index p=0; p<3; p++) newfaces[nb_faces+f].pt_ind[p] += nb_verts;
	}
	nb_verts = nverts;
	nb_faces = nfaces;
	if (vertices!=NULL) delete[] vertices;
	if (faces!=NULL) delete[] faces;
	vertices = newvertices;
	faces = newfaces;
}


// inverse les normales
void C3DObject::inverse(void) {
	for (int f=0; f<nb_faces; f++) {
		faces[f].norm.x = - faces[f].norm.x;
		faces[f].norm.y = - faces[f].norm.y;
		faces[f].norm.z = - faces[f].norm.z;
		for (int v=0; v<3; v++) {
			faces[f].pt_norm[v].x = - faces[f].pt_norm[v].x;
			faces[f].pt_norm[v].y = - faces[f].pt_norm[v].y;
			faces[f].pt_norm[v].z = - faces[f].pt_norm[v].z;
		}
	}
}

// calcule les normales aux faces
void C3DObject::compute_normals_from_vertices(void) {
	for (int f=0; f<nb_faces; f++) {
		faces[f].norm.x = faces[f].pt_norm[0].x
						+ faces[f].pt_norm[1].x
						+ faces[f].pt_norm[2].x;
		faces[f].norm.y = faces[f].pt_norm[0].y
						+ faces[f].pt_norm[1].y
						+ faces[f].pt_norm[2].y;
		faces[f].norm.z = faces[f].pt_norm[0].z
						+ faces[f].pt_norm[1].z
						+ faces[f].pt_norm[2].z;
		faces[f].norm.Normalize();
	}
}

void C3DObject::compute_normals(void) {
	for (index f=0; f<nb_faces; f++) {
		SMLVec3f A,B;
		A.x = vertices[faces[f].pt_ind[2]].x-vertices[faces[f].pt_ind[0]].x;
		A.y = vertices[faces[f].pt_ind[2]].y-vertices[faces[f].pt_ind[0]].y;
		A.z = vertices[faces[f].pt_ind[2]].z-vertices[faces[f].pt_ind[0]].z;
		B.x = vertices[faces[f].pt_ind[1]].x-vertices[faces[f].pt_ind[0]].x;
		B.y = vertices[faces[f].pt_ind[1]].y-vertices[faces[f].pt_ind[0]].y;
		B.z = vertices[faces[f].pt_ind[1]].z-vertices[faces[f].pt_ind[0]].z;
		faces[f].norm.x = A.y*B.z - A.z*B.y;
		faces[f].norm.y = A.z*B.x - A.x*B.z;
		faces[f].norm.z = A.x*B.y - A.y*B.x;
		faces[f].norm.Normalize();
	}
}

void C3DObject::joint_double(void) {
	unsigned removed=0;
	// pour tous les couples de vertices
	for (index v=0; v<nb_verts-1; v++) {
		if (vertices[v].x!=infinity) {
		for (index c=v+1; c<nb_verts; c++) {
			// si v=c et si c n'est pas marqué
			if (   (vertices[v].x==vertices[c].x)
				&& (vertices[v].y==vertices[c].y)
				&& (vertices[v].z==vertices[c].z)
				&& (vertices[c].x!=infinity)) {
				// on le marque
				vertices[c].x = infinity;
				// on dereference c
				for (index f=0; f<nb_faces; f++) {
					for (index p=0; p<3; p++) {
						if (faces[f].pt_ind[p]==c)
							// le nouvel index
							faces[f].pt_ind[p]=v;
					}
				}
				removed++;
			}
		}
		}
	}
	SMLVec3f *newvertices = new SMLVec3f[nb_verts-removed];
	if (newvertices==NULL) return;
	index i=0;
	for (v=0; v<nb_verts;v++) {
		if (vertices[v].x!=infinity) {
			newvertices[i] = vertices[v];
			for (index f=0; f<nb_faces; f++) {
				for (index p=0; p<3; p++) {
					if (faces[f].pt_ind[p]==v) faces[f].pt_ind[p]=i;
				}
			}
			i++;
		}
	}
	delete[] vertices;
	nb_verts = i;
	vertices = newvertices;
}

// prepare vertices normals for phong if sm=true. Assume CFace::norm is correct.
void C3DObject::smoothen(const bool sm) {
	if (sm) {
		SMLVec3f *pt_norm = new SMLVec3f[nb_verts];
		for (unsigned v=0; v<nb_verts; v++) {
			pt_norm[v].z = pt_norm[v].y = pt_norm[v].z = .0f;
			unsigned face_count = 0;
			for(unsigned f=0; f<nb_faces; f++) {
				if ((v==faces[f].pt_ind[0]) || (v==faces[f].pt_ind[1]) || (v==faces[f].pt_ind[2])) {
					face_count++;
					pt_norm[v] += faces[f].norm;
				}
			}
			if (face_count>0) pt_norm[v] /= face_count;
		}
		for (unsigned f=0; f<nb_faces; f++) {
			faces[f].pt_norm[0] = pt_norm[faces[f].pt_ind[0]];
			faces[f].pt_norm[1] = pt_norm[faces[f].pt_ind[1]];
			faces[f].pt_norm[2] = pt_norm[faces[f].pt_ind[2]];
		}
		delete pt_norm;
	}
	else {
		for (unsigned f=0; f<nb_faces; f++) {
			faces[f].pt_norm[0] = faces[f].norm;
			faces[f].pt_norm[1] = faces[f].norm;
			faces[f].pt_norm[2] = faces[f].norm;
		}
	}
}

void C3DObject::dump(const char* filename) {
	FILE *of = fopen(filename,"wb");
	if (of==NULL) return;
	fprintf(of,"OFF\n%i %i 0\n",nb_verts,nb_faces);
	for (int i=0; i<nb_verts; i++)
		fprintf(of,"%f %f %f\n",vertices[i].x,vertices[i].y,vertices[i].z);
	for (i=0; i<nb_faces; i++)
		fprintf(of,"3 %i %i %i\n", faces[i].pt_ind[0],faces[i].pt_ind[1],faces[i].pt_ind[2]);
	fclose(of);
}

void C3DObject::setOrigin(float x, float y, float z) {
	// position en millimetres
	origin.x = x;
	origin.y = y;
	origin.z = z;
}

void C3DObject::setSize(float sx, float sy, float sz) {
	// taille en millimetres
	size.x = sx;
	size.y = sy;
	size.z = sz;
}

void C3DObject::setOrientation(float rx, float ry, float rz) {
	// angles en radians
	orientation.x = rx;
	orientation.y = ry;
	orientation.z = rz;
}

// retourne l'objet en World Coordinates
C3DObject* C3DObject::getWorldObject(void) {
	C3DObject *o = new C3DObject;
	unsigned i,j;
	float *farray;
	// sinus & cosinus
	float cox = cos(orientation.x);
	float six = sin(orientation.x);
	float coy = cos(orientation.y);
	float siy = sin(orientation.y);
	float coz = cos(orientation.z);
	float siz = sin(orientation.z);
	// --- Rotation Matrix ---
	SMLXMatrix Rx(4,4);
	// initialise une matrice identite
	for(j=0;j<4;j++) for(i=0;i<4;i++) Rx[j][i]=(i==j?1.0f:.0f);
	Rx[1][1] = cox;
	Rx[1][2] =-six;
	Rx[2][1] = six;
	Rx[2][2] = cox;
	SMLXMatrix Ry(4,4);
	// initialise une matrice identite
	for(j=0;j<4;j++) for(i=0;i<4;i++) Ry[j][i]=(i==j?1.0f:.0f);
	Ry[0][0] = coy;
	Ry[0][2] = siy;
	Ry[2][0] =-siy;
	Ry[2][2] = coy;
	SMLXMatrix Rz(4,4);
	// initialise une matrice identite
	for(j=0;j<4;j++) for(i=0;i<4;i++) Rz[j][i]=(i==j?1.0f:.0f);
	Rz[0][0] = coz;
	Rz[0][1] =-siz;
	Rz[1][0] = siz;
	Rz[1][1] = coz;
	// Matrice tenant compte des 3 rotations
	SMLXMatrix R = Rz*Ry*Rx;
	// --- Scale Matrix ---
	SMLXMatrix S(4,4);
	for(j=0;j<4;j++) for(i=0;i<4;i++) S[j][i]=(i==j?1.0f:.0f);
	farray = size.data();
	for (i=0; i<3; i++) S[i][i] = farray[i];
	// --- Translation Matrix ---
	SMLXMatrix T(4,4);
	for(j=0;j<4;j++) for(i=0;i<4;i++) T[j][i]=(i==j?1.0f:.0f);
	farray = origin.data();
	for (i=0; i<3; i++) T[i][3] = farray[i];

	SMLXMatrix M = T*R*S;

	o->set(*this);
	// converti les vertices
	for (unsigned v=0; v<o->nb_verts; v++) {
		SMLXMatrix vec(4,1);
		vec[0][0] = o->vertices[v].x;
		vec[1][0] = o->vertices[v].y;
		vec[2][0] = o->vertices[v].z;
		vec[3][0] = 1.0f;
		SMLXMatrix nvec = M*vec;
		o->vertices[v].x = nvec[0][0];
		o->vertices[v].y = nvec[1][0];
		o->vertices[v].z = nvec[2][0];
	}
	// converti les normales (rotation seulement)
	for (unsigned f=0; f<o->nb_faces; f++) {
		SMLXMatrix vec(4,1);
		vec[0][0] = o->faces[f].norm.x;
		vec[1][0] = o->faces[f].norm.y;
		vec[2][0] = o->faces[f].norm.z;
		vec[3][0] = 1.0f;
		SMLXMatrix nvec = R*vec;
		o->faces[f].norm.x = nvec[0][0];
		o->faces[f].norm.y = nvec[1][0];
		o->faces[f].norm.z = nvec[2][0];
		for (v=0; v<3; v++) {
			SMLXMatrix nml(4,1);
			nml[0][0] = o->faces[f].pt_norm[v].x;
			nml[1][0] = o->faces[f].pt_norm[v].y;
			nml[2][0] = o->faces[f].pt_norm[v].z;
			nml[3][0] = 1.0f;
			SMLXMatrix nnml = R*nml;
			o->faces[f].pt_norm[v].x = nnml[0][0];
			o->faces[f].pt_norm[v].y = nnml[1][0];
			o->faces[f].pt_norm[v].z = nnml[2][0];
		}
	}
	o->compute_bbox();
	return o;
}
