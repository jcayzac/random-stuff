#include <math.h>

enum {
	FLOAT_FPU,
//	FLOAT_NOFPU,
	FIXED_1_15_16
};

template<int N> struct scalar_t { };

template<>
struct scalar_t<FLOAT_FPU> {
	static const long  GL_TYPE            = 0x1406; // GL_FLOAT
	static const float ZERO               = 0.f;
	static const float ONE                = 1.f;
	static const float TWO                = 2.f;
	static const float THREE              = 3.f;
	static const float FOUR               = 4.f;
	static const float EPSILON            = 1.19209290e-07f;
	static const float MIN_VALUE          = 1.17549435e-38f;
	static const float MAX_VALUE          = 3.40282347e+38f;
	static const float ONE_HALF           = 0.5f;               // 1/2
	static const float ONE_THIRD          = 0.333333333333333f; // 1/3
	static const float ONE_QUARTER        = 0.25f;              // 1/4
	static const float E                  = 2.718281828459045f; // e
	static const float LOG2E              = 1.442695040888963f; // log2(e)
	static const float LOG10E             = 0.434294481903251f; // log10(e)
	static const float LN2                = 0.693147180559945f; // ln(2)
	static const float LN10               = 2.302585092994045f; // ln(10)
	static const float SQRT_2             = 1.414213562373095f; // sqrt(2)
	static const float REC_HALF_SQRT_2    = 1.128379167095512f; // 2/sqrt(pi)
	static const float REC_SQRT_2         = 0.707106781186547f; // 1/sqrt(2)
	
	bool valid() const { return value==value && fabsf(value)!=__builtin_inff(); }
	
	scalar_t sin() const { return scalar_t(sinf(DEGREES_TO_RADIANS*value)); }
	scalar_t cos() const { return scalar_t(cosf(DEGREES_TO_RADIANS*value)); }
	scalar_t tan() const { return scalar_t(tanf(DEGREES_TO_RADIANS*value)); }
	void sincos(scalar_t& s, scalar_t& c) const {
		s.value=sinf(DEGREES_TO_RADIANS*value);
		c.value=cosf(DEGREES_TO_RADIANS*value);
	}
	
	scalar_t asin() const { return scalar_t(RADIANS_TO_DEGREES*asinf(value)); }
	scalar_t acos() const { return scalar_t(RADIANS_TO_DEGREES*acosf(value)); }
	scalar_t atan() const { return scalar_t(RADIANS_TO_DEGREES*atanf(value)); }
	
	static scalar_t atan2(const scalar_t& a, const scalar_t& b) { return scalar_t(RADIANS_TO_DEGREES*atan2f(a.value, b.value)); }
	static scalar_t hypot(const scalar_t& a, const scalar_t& b) { return scalar_t(hypotf(a.value, b.value)); }
	
	scalar_t ceil() const { return scalar_t(ceilf(value)); }
	scalar_t floor() const { return scalar_t(floorf(value)); }
	scalar_t round() const { return scalar_t(ceilf(value)); }
	scalar_t abs() const { return scalar_t(fabsf(value)); }

	scalar_t exp() const { return scalar_t(expf(value)); }
	scalar_t exp2() const { return scalar_t(exp2f(value)); }
	
	scalar_t log () const { return scalar_t(logf(value)); }
	scalar_t log10 () const { return scalar_t(log10f(value)); }
	scalar_t log2 () const { return scalar_t(log2f(value)); }
	
	scalar_t pow (const scalar_t& n) const { return scalar_t(powf(value, n.value)); }
	scalar_t sqrt() const { return scalar_t(sqrtf(value)); }
	
	scalar_t() { }
	explicit scalar_t(float v): value(v) { }
	scalar_t(const scalar_t& o): value(o.value) { }
	scalar_t& operator= (const scalar_t& o) { value=o.value; return *this; }
	scalar_t& operator+=(const scalar_t& o) { value+=o.value; return *this; }
	scalar_t& operator-=(const scalar_t& o) { value-=o.value; return *this; }
	scalar_t& operator*=(const scalar_t& o) { value*=o.value; return *this; }
	scalar_t& operator/=(const scalar_t& o) { value/=o.value; return *this; }
	friend scalar_t<FLOAT_FPU> operator+(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs);
	friend scalar_t<FLOAT_FPU> operator-(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs);
	friend scalar_t<FLOAT_FPU> operator-(const scalar_t<FLOAT_FPU>& rhs);
	friend scalar_t<FLOAT_FPU> operator*(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs);
	friend scalar_t<FLOAT_FPU> operator/(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs);
	friend scalar_t<FLOAT_FPU> operator<(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs);
	friend scalar_t<FLOAT_FPU> operator>(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs);
	friend scalar_t<FLOAT_FPU> operator==(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs);
	friend scalar_t<FLOAT_FPU> operator!=(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs);
	friend scalar_t<FLOAT_FPU> operator<=(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs);
	friend scalar_t<FLOAT_FPU> operator>=(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs);
private:
	static const float RADIANS_TO_DEGREES = 57.295779513082325f;
	static const float DEGREES_TO_RADIANS = .01745329251994329f;
	float value;
};

scalar_t<FLOAT_FPU> operator+(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(lhs.value+rhs.value);
}
scalar_t<FLOAT_FPU> operator-(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(lhs.value-rhs.value);
}
scalar_t<FLOAT_FPU> operator-(const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(-rhs.value);
}
scalar_t<FLOAT_FPU> operator*(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(lhs.value*rhs.value);
}
scalar_t<FLOAT_FPU> operator/(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(lhs.value/rhs.value);
}
scalar_t<FLOAT_FPU> operator<(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(lhs.value<rhs.value);
}
scalar_t<FLOAT_FPU> operator>(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(lhs.value>rhs.value);
}
scalar_t<FLOAT_FPU> operator==(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(lhs.value==rhs.value);
}
scalar_t<FLOAT_FPU> operator!=(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(lhs.value!=rhs.value);
}
scalar_t<FLOAT_FPU> operator<=(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(lhs.value<=rhs.value);
}
scalar_t<FLOAT_FPU> operator>=(const scalar_t<FLOAT_FPU>& lhs, const scalar_t<FLOAT_FPU>& rhs) {
	return scalar_t<FLOAT_FPU>(lhs.value>=rhs.value);
}



template<>
struct scalar_t<FIXED_1_15_16> {
	static const long GL_TYPE            = 0x140C; // GL_FIXED
	static const long ZERO               = 0;
	static const long ONE                = 65536;
	static const long TWO                = 131072;
	static const long THREE              = 196608;
	static const long FOUR               = 262144;
	static const long EPSILON            = 1;
	static const long MIN_VALUE          =-0x7fffffff;
	static const long MAX_VALUE          = 0x7fffffff;
	static const long ONE_HALF           = 32768;  // 1/2
	static const long ONE_THIRD          = 21845;  // 1/3
	static const long ONE_QUARTER        = 16384;  // 1/4
	static const long E                  = 178145; // e
	static const long LOG2E              = 94548;  // log2(e)
	static const long LOG10E             = 28462;  // log10(e)
	static const long LN2                = 45426;  // ln(2)
	static const long LN10               = 150902; // ln(10)
	static const long SQRT_2             = 92682;  // sqrt(2)
	static const long REC_HALF_SQRT_2    = 73949;  // 2/sqrt(pi)
	static const long REC_SQRT_2         = 46341;  // 1/sqrt(2)
	
	bool valid() const { return (value!=_NAN); }
	
	scalar_t sin() const { return scalar_t(); }
	scalar_t cos() const { return scalar_t(); }
	scalar_t tan() const { return scalar_t(); }
	void sincos(scalar_t& s, scalar_t& c) const { }
	
	scalar_t asin() const { return scalar_t(); }
	scalar_t acos() const { return scalar_t(); }
	scalar_t atan() const { return scalar_t(); }
	
	static scalar_t atan2(const scalar_t& a, const scalar_t& b) { return scalar_t(); }
	static scalar_t hypot(const scalar_t& a, const scalar_t& b) { return scalar_t(); }
	
	scalar_t ceil() const { return scalar_t(); }
	scalar_t floor() const { return scalar_t(value&0xFFFF0000); }
	scalar_t round() const { return scalar_t(  value&0xffff0000-0x10000*(((value&0xffff)-0x8001)>>15)    ); }
	scalar_t abs() const { return scalar_t(value<0?-value:value); }
	
	scalar_t exp() const { return scalar_t(); }
	scalar_t exp2() const { return scalar_t(); }
	
	scalar_t log () const { return scalar_t(); }
	scalar_t log10 () const { return scalar_t(); }
	scalar_t log2 () const { return scalar_t(); }
	
	scalar_t pow (const scalar_t& n) const { return scalar_t(); }
	scalar_t sqrt() const { return scalar_t(); }
	
	scalar_t() { }
	explicit scalar_t(float v) { value=(long)(v*65536.f); }
	explicit scalar_t(long v): value(v) { }
	scalar_t(const scalar_t& o): value(o.value) { }
	scalar_t& operator= (const scalar_t& o) { value=o.value; return *this; }
	scalar_t& operator+=(const scalar_t& o) { value+=o.value; return *this; }
	scalar_t& operator-=(const scalar_t& o) { value-=o.value; return *this; }
	scalar_t& operator*=(const scalar_t& o) {
		value=(((long long)value*o.value)+0x8000) >> 16);
		return *this;
	}
	scalar_t& operator/=(const scalar_t& o) {
#ifdef __VFP_FP__
		value=(((float)value)*65536.f/(float)o.value);
#else
		value=((long long)value << 16)/o.value;
#endif
		return *this;
	}
	friend scalar_t<FIXED_1_15_16> operator+(const scalar_t<FIXED_1_15_16>& lhs, const scalar_t<FIXED_1_15_16>& rhs);
	friend scalar_t<FIXED_1_15_16> operator-(const scalar_t<FIXED_1_15_16>& lhs, const scalar_t<FIXED_1_15_16>& rhs);
	friend scalar_t<FIXED_1_15_16> operator-(const scalar_t<FIXED_1_15_16>& rhs);
	friend scalar_t<FIXED_1_15_16> operator*(const scalar_t<FIXED_1_15_16>& lhs, const scalar_t<FIXED_1_15_16>& rhs);
	friend scalar_t<FIXED_1_15_16> operator/(const scalar_t<FIXED_1_15_16>& lhs, const scalar_t<FIXED_1_15_16>& rhs);
	friend scalar_t<FIXED_1_15_16> operator<(const scalar_t<FIXED_1_15_16>& lhs, const scalar_t<FIXED_1_15_16>& rhs);
	friend scalar_t<FIXED_1_15_16> operator>(const scalar_t<FIXED_1_15_16>& lhs, const scalar_t<FIXED_1_15_16>& rhs);
	friend scalar_t<FIXED_1_15_16> operator==(const scalar_t<FIXED_1_15_16>& lhs, const scalar_t<FIXED_1_15_16>& rhs);
	friend scalar_t<FIXED_1_15_16> operator!=(const scalar_t<FIXED_1_15_16>& lhs, const scalar_t<FIXED_1_15_16>& rhs);
	friend scalar_t<FIXED_1_15_16> operator<=(const scalar_t<FIXED_1_15_16>& lhs, const scalar_t<FIXED_1_15_16>& rhs);
	friend scalar_t<FIXED_1_15_16> operator>=(const scalar_t<FIXED_1_15_16>& lhs, const scalar_t<FIXED_1_15_16>& rhs);
private:
	static const long _NAN = 0x80000000L;
	long value;
};


#if 0
void transform_vectors(
	int num,
	const float* src_mat,
	int src_stride, const float* src_vec_array,
	int dst_stride, float* dst_vec_array
) {
#ifdef USE_NEON_ASM
	asm volatile (
	"vldmia %2, { q8-q11 }\n\t"
#if GPU_USES_FIXED=1
	"mov r0, #65536\n\t"
    "fmsr s3, r0\n\t"
#endif
				  "L2000:\n\t"
	"vldmia %0, { q1 }\n\t"
	"adds %0, %0, %3\n\t"
	"vmul.f32 q0, q8, d2[0]\n\t"
	"vmla.f32 q0, q9, d2[1]\n\t"
	"vmla.f32 q0, q10, d3[0]\n\t"
	"vadd.f32 q0, q0, q11\n\t"
	"vstmia %1, { q0 }"
	"adds %1, %1, %4\n\t"
	"subs %5, %5, #1\n\t"
	"bne L2000\n\t"	
	);
#endif
	
	
	
	asm volatile (
		"fldmias  %2, {s8-s23}        \n\t" // s8…s23 = src_mat
		"fmrx     r0, fpscr           \n\t" // simd_unit = 4
		"bic      r0, r0, #0x00370000 \n\t" //
		"orr      r0, r0, #0x00030000 \n\t" //
		"fmxr     fpscr, r0           \n\t" //
#if GPU_USES_FIXED=1
		"mov      r0, #65536          \n\t"
		"fmsr     s3, r0              \n\t" // s4 = 65536.f
#endif
		"L2000:                       \n\t" // do {
		"fldmias  %0, {s0-s2}         \n\t" //     [x,y,z,w] = *src_vec_array
		"adds %0, %0, %3              \n\t" //     src_vec_array += src_stride (in bytes)
		"fmuls s24, s8, s0            \n\t" //     s24…s26  =  s8…s10 * x
		"fmacs s24, s12, s1           \n\t" //     s24…s26 += s12…s14 * y
		"fmacs s24, s16, s2           \n\t" //     s24…s26 += s16…s18 * z
		"fadds s24, s24, s20          \n\t" //     s24…s26 += s20…s22 * 1 (ignore w)
#if GPU_USES_FIXED=1
		"fmuls  s24, s24, s3          \n\t" //     s24…s26 *= 65536.f
		"ftosis s24, s24              \n\t" //     s24 = (int)s24
		"ftosis s25, s25              \n\t" //     s25 = (int)s25
		"ftosis s26, s26              \n\t" //     s26 = (int)s26
#endif
		"fstmias  %1, {s24-s26}       \n\t" //    *dst_vec_array = s24…s26
		"adds %1, %1, %4              \n\t" //     dst_vec_array += dst_stride (in bytes)
		"subs %5, %5, #1              \n\t" //     --num
		"bne L2000                    \n\t" // } while(num);
		"fmrx     r0, fpscr           \n\t" // simd_unit = 1
		"bic      r0, r0, #0x00370000 \n\t" //
		"fmxr     fpscr, r0           \n\t" //
		:"=r"(src_vec_array),"=r"(dst_vec_array)
		:"r"(src_mat),"r"(src_stride),"r"(dst_stride),"r"(num),"0"(src_vec_array),"1"(dst_vec_array)
		:"r0","cc","memory",
		 "s0", "s1", "s2",       // *src_vec_array
#if GPU_USES_FIXED=1
		 "s3",                   // factor
#endif
		 "s8", "s9","s10","s11", //  src_matrix
		"s12","s13","s14","s15", //
		"s16","s17","s18","s19", //
		"s20","s21","s22","s23", //
		"s24","s25","s26"        // *dst_vec_array
	);
}

#endif


void 
normalize4_neon(float v[4], float d[4])
{
	asm volatile (
				  "vld1.32                {d4, d5}, [%0]                  \n\t"   //d4={x0,y0}, d5={z0, w0}
				  "vmul.f32               d0, d4, d4                      \n\t"   //d0= d4*d4
				  "vmla.f32               d0, d5, d5                      \n\t"   //d0 = d0 + d5*d5 
				  "vpadd.f32              d0, d0                          \n\t"   //d0 = d[0] + d[1]
				  
				  "vmov.f32               d1, d0                          \n\t"   //d1 = d0
				  "vrsqrte.f32            d0, d0                          \n\t"   //d0 = ~ 1.0 / sqrt(d0)
				  "vmul.f32               d2, d0, d1                      \n\t"   //d2 = d0 * d1
				  "vrsqrts.f32            d3, d2, d0                      \n\t"   //d3 = (3 - d0 * d2) / 2        
				  "vmul.f32               d0, d0, d3                      \n\t"   //d0 = d0 * d3
				  "vmul.f32               d2, d0, d1                      \n\t"   //d2 = d0 * d1  
				  "vrsqrts.f32            d3, d2, d0                      \n\t"   //d3 = (3 - d0 * d2) / 2        
				  "vmul.f32               d0, d0, d3                      \n\t"   //d0 = d0 * d3
				  
				  "vmul.f32               q2, q2, d0[0]                   \n\t"   //q2= q2*d0[0]
				  "vst1.32                {d4, d5}, [%1]                  \n\t"   //*d={x0, y0, z0, w0}
				  
				  :: "r"(v), "r"(d) 
				  : "d0", "d1", "d2", "d3", "d4", "d5"
				  );      
}

float 
dot4_neon(float v0[4], float v1[4])
{
	float r;
	asm volatile (
				  "vld1.32                {d2, d3}, [%1]                  \n\t"   //d2={x0,y0}, d3={z0, w0}
				  "vld1.32                {d4, d5}, [%2]                  \n\t"   //d4={x1,y1}, d5={z1, w1}
				  "vmul.f32               d0, d2, d4                      \n\t"   //d0= d2*d4
				  "vmla.f32               d0, d3, d5                      \n\t"   //d0 = d0 + d3*d5 
				  "vpadd.f32              d0, d0                          \n\t"   //d0 = d[0] + d[1]
				  "vmov.f32               %0, s0                          \n\t"   //r0 = s0
				  :"=r"(r): "r"(v0), "r"(v1): 
				  );      
	return r;
}


/*
 Division by constant
 r0 is input/output register.
 
 Division by 3
 ldr r1,=0x55555555
 umull r2,r0,r1,r0
 
 Division by 5
 ldr r1,=0x33333333
 umull r2,r0,r1,r0
 
 Division by 7
 ldr r1,=0x24924924
 umull r2,r0,r1,r0
 
 Division by N
 ldr r1,=0x100000000/N
 umull r2,r0,r1,r0
 
 
 With the addition of a look-up table of pre-calculated constants (for r1) I would say you've got a pretty fast divide function
 */

int main() {
	scalar_t<FLOAT_FPU> f(13.5f);
	scalar_t<FIXED_1_15_16> l(12.8f);
	return 0;
}
