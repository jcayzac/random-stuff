#include "ray.h"
#include "commode.h"

// Create a new Ray. When it's not iterated yet, it is endless.
CRay::CRay(const SMLVec3f& origin, const SMLVec3f& direction):
SMLRay(origin, direction,infinity) {

}

bool CRay::intersection(CRadPatch* patch) {
	float nv_dot = patch->norm.Dot(direction);
	// direction is parallel to plane
	if (fabs(nv_dot) == .0f) return false;
	
	float ti = - ((patch->norm.Dot(patch->vert[0]) + patch->norm.Dot(start))/nv_dot);
	// intersection is behind ray origin
	if (ti<=.0f) return false;

	// a closer intersection already exists
	if (ti>length) return false;

	// ray has a length equal to ti
	length = ti;
	end = start+direction*ti;
	

	return true;
}

