#if !defined(UUID_9B0150F626614B75AD46F057AC03A151)
#define UUID_9B0150F626614B75AD46F057AC03A151

#include "geometry/lowlevel/implementation.h"
#include "geometry/vec3f.h"

class cone {
private:
	vec3f mApex;
	vec3f mDirection;
	float mSinCos[2];
public:
	cone(const cone& o) { simd::copy<8>(&mApex[0],&o.mApex[0]); }
	cone() { simd::zero_initializer(&mApex[0],8); }
	cone(const vec3f& apex, const vec3f& direction, const float angle): mApex(apex), mDirection(direction) {
		simd::sincos(&mSinCos[0], angle);
	}
	cone(const vec3f& apex, const vec3f& direction, const float sincos[2]): mApex(apex), mDirection(direction) {
		simd::copy<2>(mSinCos,sincos);
	}
	cone(const float* p) {
		simd::copy<8>(&mApex[0],p);
	}
	const vec3f& apex() const { return mApex; }
	const vec3f& direction() const { return mDirection; }
	const float (&sincos() const)[2] { return mSinCos; }
};


#endif // UUID_9B0150F626614B75AD46F057AC03A151

