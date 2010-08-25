#include "tetrahedron.h"

// cree un tetraedre regulier
CTetrahedron::CTetrahedron(void) {
	faces		= new CFace[4];
	nb_faces	= 4;
	vertices	= new SMLVec3f[4];
	nb_verts	= 4;
	// assigne les vertices
	vertices[0].x = 0.0f;
	vertices[0].y = 1.0f;
	vertices[0].z = 0.0f;
	vertices[1].x = .94280904158206336586779248280647f;
	vertices[1].y = -.333333333333333333333333333333f;
	vertices[1].z = 0.0f;
	vertices[2].x = -.47140452079103168293389624140323f;
	vertices[2].y = -.333333333333333333333333333333f;
	vertices[2].z = .81649658092772603273242802490196f;
	vertices[3].x = -.47140452079103168293389624140323f;
	vertices[3].y = -.333333333333333333333333333333f;
	vertices[3].z = -.81649658092772603273242802490196f;
	// assigne les faces et les normales aux points (euh meme)
	// face ABC
	faces[0].pt_ind[0] = 0;	faces[0].pt_norm[0] = vertices[0];
	faces[0].pt_ind[1] = 1; faces[0].pt_norm[1] = vertices[1];
	faces[0].pt_ind[2] = 2; faces[0].pt_norm[2] = vertices[2];
	// face ADB
	faces[1].pt_ind[0] = 0; faces[1].pt_norm[0] = vertices[0];
	faces[1].pt_ind[1] = 3; faces[1].pt_norm[1] = vertices[3];
	faces[1].pt_ind[2] = 1; faces[1].pt_norm[2] = vertices[1];
	// face ACD
	faces[2].pt_ind[0] = 0; faces[2].pt_norm[0] = vertices[0];
	faces[2].pt_ind[1] = 2; faces[2].pt_norm[1] = vertices[2];
	faces[2].pt_ind[2] = 3; faces[2].pt_norm[2] = vertices[3];
	// face BDC
	faces[3].pt_ind[0] = 1; faces[3].pt_norm[0] = vertices[1];
	faces[3].pt_ind[1] = 3; faces[3].pt_norm[1] = vertices[3];
	faces[3].pt_ind[2] = 2; faces[3].pt_norm[2] = vertices[2];
	// calcule les normales aux faces
	compute_normals_from_vertices();
};

CTetrahedron::~CTetrahedron(void) {

}