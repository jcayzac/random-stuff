#include "box.h"

// cree une boite de 1 d'arete
CBox::CBox(void) {
	faces		= new CFace[12];
	nb_faces	= 12;
	vertices	= new SMLVec3f[8];
	nb_verts	= 8;

	// attribut les vertices
	// A
	vertices[0].x = -.5f;	vertices[0].y =  .5f;	vertices[0].z = -.5f;
	// B
	vertices[1].x =  .5f;	vertices[1].y =  .5f;	vertices[1].z = -.5f;
	// C
	vertices[2].x =  .5f;	vertices[2].y =  .5f;	vertices[2].z =  .5f;
	// D
	vertices[3].x = -.5f;	vertices[3].y =  .5f;	vertices[3].z =  .5f;
	// E
	vertices[4].x = -.5f;	vertices[4].y = -.5f;	vertices[4].z = -.5f;
	// F
	vertices[5].x =  .5f;	vertices[5].y = -.5f;	vertices[5].z = -.5f;
	// G
	vertices[6].x =  .5f;	vertices[6].y = -.5f;	vertices[6].z =  .5f;
	// H
	vertices[7].x = -.5f;	vertices[7].y = -.5f;	vertices[7].z =  .5f;
	// attribut les faces
	// ABC
	faces[0].pt_ind[0] = 0;	faces[0].pt_ind[1] = 1;	faces[0].pt_ind[2] = 2;
	// CDA
	faces[1].pt_ind[0] = 2;	faces[1].pt_ind[1] = 3;	faces[1].pt_ind[2] = 0;
	// ADH
	faces[2].pt_ind[0] = 0;	faces[2].pt_ind[1] = 3;	faces[2].pt_ind[2] = 7;
	// HEA
	faces[3].pt_ind[0] = 7;	faces[3].pt_ind[1] = 4;	faces[3].pt_ind[2] = 0;
	// CBF
	faces[4].pt_ind[0] = 2;	faces[4].pt_ind[1] = 1;	faces[4].pt_ind[2] = 5;
	// FGC
	faces[5].pt_ind[0] = 5;	faces[5].pt_ind[1] = 6;	faces[5].pt_ind[2] = 2;
	// FBA
	faces[6].pt_ind[0] = 5;	faces[6].pt_ind[1] = 1;	faces[6].pt_ind[2] = 0;
	// AEF
	faces[7].pt_ind[0] = 0;	faces[7].pt_ind[1] = 4;	faces[7].pt_ind[2] = 5;
	// HDC
	faces[8].pt_ind[0] = 7;	faces[8].pt_ind[1] = 3;	faces[8].pt_ind[2] = 2;
	// CGH
	faces[9].pt_ind[0] = 2;	faces[9].pt_ind[1] = 6;	faces[9].pt_ind[2] = 7;
	// FEH
	faces[10].pt_ind[0] =5;	faces[10].pt_ind[1] =4;	faces[10].pt_ind[2] = 7;
	// HGF
	faces[11].pt_ind[0] =7;	faces[11].pt_ind[1] =6;	faces[11].pt_ind[2] = 5;
	// calcule les normales aux points et aux faces
	compute_normals();
	smoothen(false);
};

CBox::~CBox(void) {

}