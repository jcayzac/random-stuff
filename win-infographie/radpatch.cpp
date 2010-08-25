#include "radpatch.h"
#include "commode.h"

CRadPatch::CRadPatch(): quad(NULL), sub_level(0) {
	vert = new SMLVec3f[3];
}

CRadPatch::~CRadPatch(void) {
	if (vert!=NULL) delete[] vert;
	vert=NULL;
	if (quad!=NULL) delete[] quad;
	quad=NULL;
}


bool CRadPatch::subdivide(void) {
	if (sub_level==MAX_SUBLEVEL) {
		return false;
	}

	// calcul des mid points
	SMLVec3f* midpoints = new SMLVec3f[3];

	for (int p=0; p<3; p++) {

		midpoints[p].x = .5f*(vert[p%3].x + vert[(p+1)%3].x);
		midpoints[p].y = .5f*(vert[p%3].y + vert[(p+1)%3].y);
		midpoints[p].z = .5f*(vert[p%3].z + vert[(p+1)%3].z);
	}

	quad = new CRadPatch[4];

	// vertices 
	quad[0].vert[0] = vert[0];
	quad[0].vert[1] = midpoints[0];
	quad[0].vert[2] = midpoints[2];

	quad[1].vert[0] = midpoints[0];
	quad[1].vert[1] = vert[1];
	quad[1].vert[2] = midpoints[1];

	quad[2].vert[0] = midpoints[2];
	quad[2].vert[1] = midpoints[1];
	quad[2].vert[2] = vert[2];

	quad[2].vert[0] = midpoints[0];
	quad[2].vert[1] = midpoints[1];
	quad[2].vert[2] = midpoints[2];

	for (int i=0; i<4; i++) {
		CRadPatch patch;
		quad[i].area = area / 4;
		quad[i].sub_level = sub_level + 1;
		quad[i].center = quad[i].vert[0] + quad[i].vert[1] + quad[i].vert[2];
		quad[i].center /= 3.0f;
	}

//////////////
	FILE *of = fopen("d:\\infographie\\subdivide.off","awb");
	if (of==NULL) return false;

	// vertices
	for (i=0; i<4; i++) {
		for (int j=0; j<3; j++) {
			fprintf(of,"%f %f %f\n",quad[i].vert[j].x,quad[i].vert[j].y,quad[i].vert[j].z);
		}
	}
	fclose(of);
/////////////////

	//	sub_level++;
	return true;
}





CRadPatch* CRadPatch::getLeaf() {
	if (quad == NULL) {
		return this;	
	} else {
		for (int i=0; i<4; i++) {
			CRadPatch* leaf = quad[i].getLeaf();
			if ((leaf == &quad[i]) && (leaf->sub_level != MAX_SUBLEVEL)) {  
				return leaf;
			}
		}
		return NULL;
	}
}





const CRadPatch& CRadPatch::operator=(const CRadPatch& src) {
	vert[0] = src.vert[0];
	vert[1] = src.vert[1];
	vert[2] = src.vert[2];
	if (src.quad!=NULL) {
		if (quad==NULL) quad=new CRadPatch[4];
		for (unsigned i=0; i<4; i++) quad[i] = src.quad[i];
	}
	return *this;
}
