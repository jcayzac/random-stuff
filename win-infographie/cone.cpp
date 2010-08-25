#include "cone.h"
#include <math.h>

// Cree un cone de rayon 1 et de hauteur 1
CCone::CCone(unsigned segments) {
	nb_verts = segments+2;
	nb_faces = 2*segments;
	vertices = new SMLVec3f[nb_verts];
	faces    = new CFace[nb_faces];
	// attribut les vertices
	float itrad = 2*pi/((float)segments);
	for (index v=0; v<segments; v++) {
		float angle = itrad*((float)v);
		vertices[v].x = cos(angle);
		vertices[v].y = .0f;
		vertices[v].z = -sin(angle);
	}
	vertices[segments].x = .0f;
	vertices[segments].y = 1.0f;
	vertices[segments].z = .0f;
	vertices[1+segments].x = .0f;
	vertices[1+segments].y = .0f;
	vertices[1+segments].z = .0f;

	// attribut les faces
	index f=0;
	for (v=0; v<segments; v++) {
		// up faces
		faces[f].pt_ind[0]	= v;
		faces[f].pt_ind[1]	= segments;
		faces[f].pt_ind[2]= (v+1)%segments;
		f++;
		// bottom faces
		faces[f].pt_ind[0]	= (v+1)%segments;
		faces[f].pt_ind[1]	= 1+segments;
		faces[f].pt_ind[2]= v;
		f++;
	}
	// normales
	compute_normals();
	f=0;
	for (v=0; v<segments; v++) {
		// up faces: normal = sqrt(vert² + (0,1,0)²)
		faces[f].pt_norm[0]	= vertices[faces[f].pt_ind[0]];
		faces[f].pt_norm[0].y = 1.0f;
		faces[f].pt_norm[0].Normalize();

		faces[f].pt_norm[1].x = .5f*(faces[f].pt_norm[0].x + faces[f].pt_norm[2].x);
		faces[f].pt_norm[1].z = .5f*(faces[f].pt_norm[0].z + faces[f].pt_norm[2].z);
		faces[f].pt_norm[1].y = .0f;
		faces[f].pt_norm[1].Normalize();
		faces[f].pt_norm[1].y = 1.0f;
		faces[f].pt_norm[1].Normalize();

		faces[f].pt_norm[2]	= vertices[faces[f].pt_ind[2]];
		faces[f].pt_norm[2].y = 1.0f;
		faces[f].pt_norm[2].Normalize();
		f++;
		// bottom faces: normal = (0,-1,0)
		faces[f].pt_norm[0].x	= 0.0f;
		faces[f].pt_norm[0].y	=-1.0f;
		faces[f].pt_norm[0].z	= 0.0f;

		faces[f].pt_norm[1].x	= 0.0f;
		faces[f].pt_norm[1].y	=-1.0f;
		faces[f].pt_norm[1].z	= 0.0f;

		faces[f].pt_norm[2].x	= 0.0f;
		faces[f].pt_norm[2].y	=-1.0f;
		faces[f].pt_norm[2].z	= 0.0f;
		f++;
	}
}

CCone::~CCone(void) {

}
