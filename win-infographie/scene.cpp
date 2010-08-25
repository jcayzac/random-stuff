#include <smlmath.h>
#include "scene.h"
#include "hemi.h"

CScene scene;

CScene::CScene(void): nb_verts(0),nb_mats(0),nb_faces(0) {
	vert_heap = VERTICE_HEAP_INCREMENT;
	face_heap = FACE_HEAP_INCREMENT;
	mat_heap  = MATERIAL_HEAP_INCREMENT;
	vertices  = new SMLVec3f[vert_heap];
	materials = new CMaterial[mat_heap];
	faces     = new CFace[face_heap];
}

CScene::~CScene(void) {
	delete[] vertices;
	delete[] materials;
	delete[] faces;
}

void CScene::increaseVHeap(void) {
	SMLVec3f* newvertices = new SMLVec3f[vert_heap+VERTICE_HEAP_INCREMENT];
	for (unsigned v=0; v<vert_heap; v++) newvertices[v] = vertices[v];
	delete[] vertices;
	vertices   = newvertices;
	vert_heap += VERTICE_HEAP_INCREMENT;
}

void CScene::increaseMHeap(void) {
	CMaterial* newmat = new CMaterial[mat_heap+MATERIAL_HEAP_INCREMENT];
	for (unsigned m=0; m<mat_heap; m++) newmat[m] = materials[m];
	delete[] materials;
	materials = newmat;
	mat_heap += MATERIAL_HEAP_INCREMENT;
}

void CScene::increaseFHeap(void) {
	CFace* newfaces = new CFace[face_heap+FACE_HEAP_INCREMENT];
	for (unsigned f=0; f<face_heap; f++) newfaces[f] = faces[f];
	delete[] faces;
	faces      = newfaces;
	face_heap += FACE_HEAP_INCREMENT;
}

unsigned CScene::addVertex(const SMLVec3f& v) {
	if (nb_verts==vert_heap) increaseVHeap();
	vertices[nb_verts] = v;
	unsigned i = nb_verts;
	nb_verts++;
	return i;
}

unsigned CScene::addFace(const CFace& f) {
	if (nb_faces==face_heap) increaseFHeap();
	faces[nb_faces] = f;
	unsigned i = nb_faces;
	nb_faces++;
	return i;
}

void CScene::addObject(const C3DObject& o) {
	unsigned mat=0;
	unsigned v;
	if (o.material!=NULL) mat = addMaterial(*(o.material));
	unsigned vert_base=nb_verts;
	for (v=0; v<o.nb_verts; v++) addVertex(o.vertices[v]);
	for (unsigned f=0; f<o.nb_faces; f++) {
		CFace *ft = new CFace;
		*ft= o.faces[f];
		ft->mat_ind = mat;
		for (v=0; v<3; v++) ft->pt_ind[v] += vert_base;
		addFace(*ft);
	}
}

unsigned CScene::addMaterial(const CMaterial& m) {
	string n = m.name;
	unsigned i=getMaterialIndex(n);
	if (i!=MATERIAL_NOT_FOUND) return i;
	if (nb_mats==mat_heap) increaseMHeap();
	materials[nb_mats] = m;
	i = nb_mats;
	nb_mats++;
	return i;
}

unsigned CScene::getMaterialIndex(const string& name) {
	unsigned i=0;
	while ((i<nb_mats) && (name.compare(materials[i].name)!=0)) i++;
	// not found
	if (i==nb_mats) return MATERIAL_NOT_FOUND;
	return i;
}

void CScene::dump(const char* filename) {
	FILE *of = fopen(filename,"wb");
	if (of==NULL) return;
	fprintf(of,"OFF\n%i %i 0\n",nb_verts/*+3*nb_faces*/,nb_faces);

	// vertices
	for (int i=0; i<nb_verts; i++)
		fprintf(of,"%f %f %f\n",vertices[i].x,vertices[i].y,vertices[i].z);

	// normales aux faces (10 cm)
	/*for (i=0; i<nb_faces; i++) {
		SMLVec3f o = vertices[faces[i].pt_ind[0]] + vertices[faces[i].pt_ind[1]] + vertices[faces[i].pt_ind[2]];
		o /= 3.0f;
		o += faces[i].norm*100.0f;
		fprintf(of,"%f %f %f\n", o.x, o.y, o.z);
	}*/

	// normales aux vertices (10 cm)
	/*for (i=0; i<nb_faces; i++) {
		SMLVec3f o = faces[i].pt_norm[0]*100.0f;
		o += vertices[faces[i].pt_ind[0]];
		fprintf(of,"%f %f %f\n", o.x, o.y, o.z);
		o = faces[i].pt_norm[1]*100.0f;
		o += vertices[faces[i].pt_ind[1]];
		fprintf(of,"%f %f %f\n", o.x, o.y, o.z);
		o = faces[i].pt_norm[2]*100.0f;
		o += vertices[faces[i].pt_ind[2]];
		fprintf(of,"%f %f %f\n", o.x, o.y, o.z);
	}*/
	// faces
	for (i=0; i<nb_faces; i++)
		fprintf(of,"3 %i %i %i\n", faces[i].pt_ind[0],faces[i].pt_ind[1],faces[i].pt_ind[2]);
	fclose(of);
}

// prepare the initial patch division
void CScene::lock(void) {
	for (unsigned f=0; f<nb_faces; f++) {
		// plane value
		faces[f].plane_value = - faces[f].norm.Dot(vertices[faces[f].pt_ind[0]]);
		// vertices
		faces[f].vert[0] = vertices[faces[f].pt_ind[0]];
		faces[f].vert[1] = vertices[faces[f].pt_ind[1]];
		faces[f].vert[2] = vertices[faces[f].pt_ind[2]];
		// center
		faces[f].center = faces[f].vert[0] + faces[f].vert[1] + faces[f].vert[2];
		faces[f].center /= 3.0f;
		// area
		SMLVec3f tv = (faces[f].vert[1]-faces[f].vert[0]);
		tv.Cross(faces[f].vert[2]-faces[f].vert[0]);
		faces[f].area = .5f*tv.Length();
		// reflectivity
		faces[f].R.x = materials[faces[f].mat_ind].col.X;
		faces[f].R.y = materials[faces[f].mat_ind].col.Y;
		faces[f].R.z = materials[faces[f].mat_ind].col.Z;
		// Emission
		faces[f].E.z = faces[f].E.y = faces[f].E.x = materials[faces[f].mat_ind].light;
		// Gathered Radiosity
		faces[f].Bg.z = faces[f].Bg.y = faces[f].Bg.x = .0f;
		// Shooting Radiosity
		faces[f].Bs = faces[f].E;
		faces[f].root = NULL;

	}
}


void CScene::subdivideAll(void) {

	for (int f=0; f<nb_faces; f++) {
		bool ok = true;
		CRadPatch* root = &faces[f];
		CRadPatch* current = NULL;
		
		while (ok) {
			// recupere toutes les feuilles (patchs)
			current = root->getLeaf();
			if (current == NULL) {
				ok = false;
			} else {
				// pour chaque patch
				current->subdivide();		
				ok = true;
			}
		}
	}
}


void CScene::subdivideIntel(void) {

}


void CScene::calculateFFMatrix(void) {

}







void CScene::trace(CRay ray, CColor& col, unsigned level) {
	
}

void CScene::render(const CScreen& screen, const CImage& image) {
	// "Finalise" les faces
	lock();
	// Radiosite (1: Subdivision)
	subdivideAll();
	// Radiosite (2: Calcul)
	calculateFFMatrix();
	// Ray Tracing
	
}
