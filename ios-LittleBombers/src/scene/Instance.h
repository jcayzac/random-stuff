#if !defined(UUID_E92EDCDF0F5D4509B49A542A13972C0B)
#define UUID_E92EDCDF0F5D4509B49A542A13972C0B

#include <vector>
#include "scene/Vertex.h"
#include "geometry/sphere.h"

class Instance {
public:
	
private:
	std::vector<Vertex> mVertices;
	std::vector<short>  mIndices;
	float           mSphereOrigin;
	float radius;
};

#endif // UUID_E92EDCDF0F5D4509B49A542A13972C0B

