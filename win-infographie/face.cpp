#include <smlmath.h>
#include "scene.h"
#include "face.h"

CFace::CFace(void) {
	mat_ind=0;
	plane_value=.0f;
}

CFace::~CFace(void) {

}

// return barycentric coordinates for a point on the face
const Tuv& CFace::getUV(const SMLVec3f& p) {
	Tuv *c = new Tuv;
	float nax = fabs(norm.x);
	float nay = fabs(norm.y);
	float naz = fabs(norm.z);
	SMLVec3f C_A = vert[2]-vert[0];
	SMLVec3f B_A = vert[1]-vert[0];
	if ((naz>=nax) && (naz>=nay)) {
		float tmp = vert[0].x*vert[2].y - vert[0].y*vert[2].x;
		float den = vert[1].y*C_A.x - vert[1].x*C_A.y + tmp;
		c->u = (p.x*C_A.y + p.y*C_A.x + tmp) / den;
		c->v = (p.x*B_A.y - p.y*B_A.x + vert[0].y*vert[1].x - vert[0].x*vert[1].y) / den;
	}
	else if ((nax>=nay) && (nax>=naz)) {
		float tmp = vert[0].y*vert[2].z - vert[0].z*vert[2].y;
		float den = vert[1].z*C_A.y - vert[1].y*C_A.z + tmp;
		c->u = (p.y*C_A.z + p.z*C_A.y + tmp) / den;
		c->v = (p.y*B_A.z - p.z*B_A.y + vert[0].z*vert[1].y - vert[0].y*vert[1].z) / den;
	}
	else {
		float tmp = vert[0].z*vert[2].x - vert[0].x*vert[2].z;
		float den = vert[1].x*C_A.z - vert[1].z*C_A.x + tmp;
		c->u = (p.z*C_A.x + p.x*C_A.z + tmp) / den;
		c->v = (p.z*B_A.x - p.x*B_A.z + vert[0].x*vert[1].z - vert[0].z*vert[1].x) / den;
	}
	return *c;
}

bool CFace::intersection(CRay& ray) {
	float nv_dot = norm.Dot(ray.direction);
	// ray.direction is parallel to plane
	if (fabs(nv_dot)==.0f) return false;
	float ti = - ((plane_value + norm.Dot(ray.start))/nv_dot);
	// intersection is behind ray origin
	if (ti<=.0f) return false;
	// a closer intersection already exists
	if (ti>ray.length) return false;
	SMLVec3f i = ray.start+ray.direction*ti;
	// Compute U and V
	Tuv uv = getUV(i);
	// Test if it's in the triangle
	if ((uv.u<0) || (uv.v<0) || (uv.u+uv.v>1.0f)) return false;
	// ray has a length equal to ti
	ray.length = ti;
	ray.end = i;
	return true;
}

// attribut aux vertices la meme normale que la face (flat)
void CFace::compute_vertices_normals_flat(void) {
	for (unsigned n=0; n<3; n++)
		pt_norm[n] = norm;
}

const CFace& CFace::operator=(const CFace& src) {
	for (unsigned i=0; i<3; i++) pt_ind[i] = src.pt_ind[i];
	for (i=0; i<3; i++) pt_norm[i] = src.pt_norm[i];
	norm = src.norm;
	plane_value = src.plane_value;
	mat_ind = src.mat_ind;
	return *this;
}

