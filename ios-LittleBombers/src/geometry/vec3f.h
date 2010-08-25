#if !defined(UUID_43F16388EE6D427E8093EC1ED0D0D42E)
#define UUID_43F16388EE6D427E8093EC1ED0D0D42E

#include "geometry/lowlevel/implementation.h"

class vec3f {
private:
	float f[3];
	vec3f& operator^=(const vec3f& rhs); // make this inaccessible since v3_cross wants restricted pointers
public:
	vec3f()                                            { simd::zero_initializer(f,3); }
	vec3f(const float x, const float y, const float z) { f[0]=x; f[1]=y; f[2]=z; }
	vec3f(const float* p)                              { simd::v3_copy(f,p); }
	vec3f(const vec3f& o)                              { simd::v3_copy(f,o.f); }
	float& operator[](size_t i)                        { assert(i>=0 && i<3); return f[i]; }
	const float& operator[](size_t i) const            { assert(i>=0 && i<3); return f[i]; }
	vec3f& operator+=(const vec3f& rhs)                { simd::v3_add(f,rhs.f); return *this; }
	vec3f& operator-=(const vec3f& rhs)                { simd::v3_sub(f,rhs.f); return *this; }
	vec3f& operator*=(const vec3f& rhs)                { simd::v3_mul(f,rhs.f); return *this; }
	vec3f& operator/=(const vec3f& rhs)                { simd::v3_div(f,rhs.f); return *this; }
	vec3f& operator+=(const float rhs)                 { simd::v3_add(f,rhs); return *this; }
	vec3f& operator-=(const float rhs)                 { simd::v3_sub(f,rhs); return *this; }
	vec3f& operator*=(const float rhs)                 { simd::v3_mul(f,rhs); return *this; }
	vec3f& operator/=(const float rhs)                 { simd::v3_div(f,rhs); return *this; }
	float sum() const                                  { float r; simd::v3_sum(r,f); return r; }
	float dot(const vec3f& rhs) const                  { float r; simd::v3_dot(r,f,rhs.f); return r; }
	float squareLength() const                         { float r; simd::v3_length2(r,f); return r; }
	float length() const                               { return sqrtf(squareLength()); }
	float squareDistance(const vec3f& rhs) const       { return (rhs-*this).squareLength(); }
	float distance(const vec3f& rhs) const             { return sqrtf(squareDistance(rhs)); }
	void  normalize()                                  { simd::v3_normalize(f); }
	friend vec3f operator-(const vec3f& lhs, const vec3f& rhs);
};

inline vec3f operator+(const vec3f& lhs, const vec3f& rhs) { vec3f r; simd::v3_add(&r[0],&lhs[0],&rhs[0]); return r; }
inline vec3f operator-(const vec3f& lhs, const vec3f& rhs) { vec3f r; simd::v3_sub(&r[0],&lhs[0],&rhs[0]); return r; }
inline vec3f operator*(const vec3f& lhs, const vec3f& rhs) { vec3f r; simd::v3_mul(&r[0],&lhs[0],&rhs[0]); return r; }
inline vec3f operator/(const vec3f& lhs, const vec3f& rhs) { vec3f r; simd::v3_div(&r[0],&lhs[0],&rhs[0]); return r; }
inline vec3f operator+(const vec3f& lhs, const float  rhs) { vec3f r; simd::v3_add(&r[0],&lhs[0],rhs); return r; }
inline vec3f operator-(const vec3f& lhs, const float  rhs) { vec3f r; simd::v3_sub(&r[0],&lhs[0],rhs); return r; }
inline vec3f operator*(const vec3f& lhs, const float  rhs) { vec3f r; simd::v3_mul(&r[0],&lhs[0],rhs); return r; }
inline vec3f operator/(const vec3f& lhs, const float  rhs) { vec3f r; simd::v3_div(&r[0],&lhs[0],rhs); return r; }
inline vec3f operator-(const vec3f& rhs)                   { vec3f r; simd::v3_neg(&r[0],&rhs[0]); return r; }
inline vec3f operator^(const vec3f& lhs, const vec3f& rhs) { vec3f r; simd::v3_cross(&r[0],&lhs[0],&rhs[0]); return r; }

#endif // UUID_43F16388EE6D427E8093EC1ED0D0D42E

