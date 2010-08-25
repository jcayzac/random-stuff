#include <math.h>
#include "sphere.h"
#include "tetrahedron.h"

// cree une sphere en tesselant un tetraedre regulier
CSphere::CSphere(unsigned tessel) {
	CTetrahedron o;
	unsigned f=(unsigned)pow(4,tessel+1);
	unsigned v=f;

	nb_faces=4;
	nb_verts=4;
	vertices	= new SMLVec3f[v];
	faces		= new CFace[f];

	for (v=0; v<o.nb_verts; v++) {
		vertices[v] = o.vertices[v];
	}
	for (f=0; f<o.nb_faces; f++) {
		faces[f].pt_ind[0] = o.faces[f].pt_ind[0];
		faces[f].pt_ind[1] = o.faces[f].pt_ind[1];
		faces[f].pt_ind[2] = o.faces[f].pt_ind[2];
	}
	tesselate(tessel);
	// calcule les normales
	for (f=0; f<nb_faces; f++) {
		faces[f].pt_norm[0] = vertices[faces[f].pt_ind[0]];
		faces[f].pt_norm[1] = vertices[faces[f].pt_ind[1]];
		faces[f].pt_norm[2] = vertices[faces[f].pt_ind[2]];
	}
	compute_normals_from_vertices();
}

CSphere::~CSphere(void) {

}

void CSphere::tesselate(unsigned level) {
	while (level-->0) {
		unsigned oldv = nb_verts;
		unsigned loutre = nb_faces;
		// pour toutes les faces
		for(int f=0; f<loutre; f++) {
			// calcule les mid-points
			for (int p=0; p<3; p++) {
				vertices[oldv+p].x = .5f*(vertices[faces[f].pt_ind[p%3]].x +
										  vertices[faces[f].pt_ind[(p+1)%3]].x);
				vertices[oldv+p].y = .5f*(vertices[faces[f].pt_ind[p%3]].y +
										  vertices[faces[f].pt_ind[(p+1)%3]].y);
				vertices[oldv+p].z = .5f*(vertices[faces[f].pt_ind[p%3]].z +
										  vertices[faces[f].pt_ind[(p+1)%3]].z);
			}
			// calcule les faces
			// 0 -> 0/1 -> 0/2
			faces[nb_faces].pt_ind[0]	= faces[f].pt_ind[0];
			faces[nb_faces].pt_ind[1]	= oldv;
			faces[nb_faces++].pt_ind[2]	= oldv+2;

			// 1 -> 1/2 -> 0/1
			faces[nb_faces].pt_ind[0]	= faces[f].pt_ind[1];
			faces[nb_faces].pt_ind[1]	= oldv+1;
			faces[nb_faces++].pt_ind[2]	= oldv;

			// 2 -> 0/2 -> 1/2
			faces[nb_faces].pt_ind[0]	= faces[f].pt_ind[2];
			faces[nb_faces].pt_ind[1]	= oldv+2;
			faces[nb_faces++].pt_ind[2]	= oldv+1;

			// 0/1 -> 1/2 -> 0/2
			faces[f].pt_ind[0]	= oldv;
			faces[f].pt_ind[1]	= oldv+1;
			faces[f].pt_ind[2]	= oldv+2;

			// repositionne le curseur des vertices
			oldv+=3;
		}
		nb_verts = oldv;
	}
	// joint les doubles
	joint_double();
	// normalise les vertices:
	for (index v=0; v<nb_verts; v++) {
		float d;
		d = 1/sqrt(	 vertices[v].x*vertices[v].x
					+vertices[v].y*vertices[v].y
					+vertices[v].z*vertices[v].z);
		vertices[v].x *= d;
		vertices[v].y *= d;
		vertices[v].z *= d;
	}
}
