#if !defined(UUID_2E4E8B0847884AB4A033BB60D661FC9B)
#define UUID_2E4E8B0847884AB4A033BB60D661FC9B

#include "geometry/lowlevel/implementation.h"
#include "geometry/vec3f.h"

class sphere {
private:
	vec3f mCenter;
	float mRadius;
public:
	sphere() { simd::zero_initializer(&mCenter[0],4); }
	sphere(const float x, const float y, const float z, const float r): mCenter(x,y,z), mRadius(r) { }
	sphere(const vec3f& c, float r): mCenter(c), mRadius(r) { }
	sphere(const float* p)  { simd::copy<4>(&mCenter[0],p); }
	sphere(const sphere& o) { simd::copy<4>(&mCenter[0],&o.mCenter[0]); }
	const vec3f& center() const { return mCenter; }
	const float& radius() const { return mRadius; }
};

#endif // UUID_2E4E8B0847884AB4A033BB60D661FC9B

