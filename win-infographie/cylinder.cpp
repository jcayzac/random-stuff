#include "cylinder.h"
#include <math.h>

CCylinder::CCylinder(unsigned segments) {
	nb_verts = 2*(segments+1);
	nb_faces = 4*segments;
	vertices = new SMLVec3f[nb_verts];
	faces    = new CFace[nb_faces];
	// attribut les vertices
	float shift = pi/((float)segments);
	float itrad = 2*shift;
	for (index v=0; v<segments; v++) {
		float angle = itrad*((float)v);
		vertices[v].x = cos(angle);
		vertices[v].y = .5f;
		vertices[v].z = -sin(angle);
		angle += shift;
		vertices[v+segments].x = cos(angle);
		vertices[v+segments].y = -.5f;
		vertices[v+segments].z = -sin(angle);
	}
	vertices[2*segments].x = .0f;
	vertices[2*segments].y = .5f;
	vertices[2*segments].z = .0f;
	vertices[1+2*segments].x = .0f;
	vertices[1+2*segments].y = -.5f;
	vertices[1+2*segments].z = .0f;

	// attribut les faces
	index f=0;
	for (v=0; v<segments; v++) {
		// \/ faces
		faces[f].pt_ind[0]	  = v;
		faces[f].pt_ind[1]	  = (v+1)%segments;
		faces[f].pt_ind[2]    = v+segments;
		faces[f].pt_norm[0]   = vertices[faces[f].pt_ind[0]];
		faces[f].pt_norm[0].y = .0f;
		faces[f].pt_norm[1]   = vertices[faces[f].pt_ind[1]];
		faces[f].pt_norm[1].y = .0f;
		faces[f].pt_norm[2] = vertices[faces[f].pt_ind[2]];
		f++;
		// /\ faces
		faces[f].pt_ind[0]	  = v+segments;
		faces[f].pt_ind[1]	  = (v+1)%segments;
		faces[f].pt_ind[2]    = segments+((v+1)%segments);
		faces[f].pt_norm[0]   = vertices[faces[f].pt_ind[0]];
		faces[f].pt_norm[1]   = vertices[faces[f].pt_ind[1]];
		faces[f].pt_norm[1].y = .0f;
		faces[f].pt_norm[2] = vertices[faces[f].pt_ind[2]];
		f++;
		// up faces
		faces[f].pt_ind[0]	= v;
		faces[f].pt_ind[1]	= 2*segments;
		faces[f].pt_ind[2]= (v+1)%segments;
		faces[f].pt_norm[0].x = .0f;
		faces[f].pt_norm[0].y = 1.0f;
		faces[f].pt_norm[0].z = .0f;
		faces[f].pt_norm[1] = faces[f].pt_norm[2] = faces[f].pt_norm[0];
		f++;
		// bottom faces
		faces[f].pt_ind[0]	= segments+((v+1)%segments);
		faces[f].pt_ind[1]	= 1+2*segments;
		faces[f].pt_ind[2]= v+segments;
		faces[f].pt_norm[0].x = .0f;
		faces[f].pt_norm[0].y = -1.0f;
		faces[f].pt_norm[0].z = .0f;
		faces[f].pt_norm[1] = faces[f].pt_norm[2] = faces[f].pt_norm[0];
		f++;
	}
	compute_normals();
}

CCylinder::~CCylinder(void) {

}
