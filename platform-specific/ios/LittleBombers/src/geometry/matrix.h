#if !defined(UUID_D0286F2DABC7414290710D750E9C5CBC)
#define UUID_D0286F2DABC7414290710D750E9C5CBC

#include "geometry/lowlevel/implementation.h"
#include "geometry/vec3f.h"

class matrix {
private:
	float f[16];
public:
	matrix()                                           { simd::zero_initializer(f,16); }
	matrix(const float* p)                             { simd::m_copy(f,p); }
	matrix(const matrix& o)                            { simd::m_copy(f,o.f); }
	float& operator[](size_t i)                        { assert(i>=0 && i<16); return f[i]; }
	const float& operator[](size_t i) const            { assert(i>=0 && i<16); return f[i]; }
	void identity()                                    { simd::m_build_identity(f); }
	void buildTranslation(const vec3f& d)              { simd::m_build_translation(f,&d[0]); }
	void buildScale(const float scale)                 { simd::m_build_scale(f,scale); }
	void buildRotationX(const float angle)             { simd::m_build_rotation_x(f,angle); }
	void buildRotationY(const float angle)             { simd::m_build_rotation_y(f,angle); }
	void buildRotationZ(const float angle)             { simd::m_build_rotation_z(f,angle); }
	void buildRotation(const vec3f& v, const float a)  { simd::m_build_rotation(f,&v[0], a); }
	matrix& operator*=(const matrix& o)                { simd::m_mul(f,o.f); return *this; }
	void transform(vec3f& dst, const vec3f& src, float w) const {
		simd::m_mul_v3(&dst[0],&src[0],f,w);
	}
	void transform(vec3f* __restrict__ dst, int dstStride, const vec3f* __restrict__ src, int srcStride, int length, float w) const {
		assert((dstStride&3)==0 && (srcStride&3)==0);
		simd::m_mul_v3(&dst[0][0],dstStride,&src[0][0],srcStride,f,length,w);
	}
};

inline matrix operator*(const matrix& lhs, const matrix& rhs) {
	matrix tmp;
	simd::m_mul(&tmp[0],&lhs[0],&rhs[0]);
	return tmp;
}

inline matrix operator/(const int i, const matrix& rhs) {
	assert(i==1);
	matrix tmp;
	simd::m_inverse_transform(&tmp[0],&rhs[0]);
	return tmp;
}

#endif // UUID_D0286F2DABC7414290710D750E9C5CBC

