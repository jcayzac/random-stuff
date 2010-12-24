#include <arm_neon.h>
typedef unsigned int size_t;
extern float sqrtf(float);

inline void __attribute__((always_inline)) homogenize(float32x4_t& vec) {
	register float32x4_t auto1;
	register float32x4_t auto2;
	register float32x4_t auto3;
	asm volatile (
	"\n\t# make %q0 homogenous\n\t\n\t"
	"vdup.f32   %q1, %f0[1]	@ %q1 = vec4(%q0.w)\n\t"
	"vrecpe.f32 %q2, %q1		@ %q2 = approx(1/%q1)\n\t"
	"vrecps.f32 %q3, %q2, %q1	@ %q3 = next approximation step\n\t"
#if 1
	"vmul.f32   %q2, %q2, %q3	@ %q2 = refine(%q2)\n\t"
	"vrecps.f32 %q3, %q2, %q1	@ %q3 = next approximation step\n\t"
#endif
	"vmul.f32   %q0, %q2, %q3	@ %q0.xyzw *= 1/%q1\n\t\n\t"
	: "=&w" (vec), "=w" (auto1), "=w" (auto2), "=w" (auto3)
	: "0" (vec)
	:
	);
}

void __attribute__((always_inline)) mat44_multiply_neon(float32x4x4_t& result, const float32x4x4_t& a, const float32x4x4_t& b) {
	// Make sure "a" is mapped to registers in the d0-d15 range,
	// as requested by NEON multiply operations below:
	register float32x4_t a0 asm("q0") = a.val[0];
	register float32x4_t a1 asm("q1") = a.val[1];
	register float32x4_t a2 asm("q2") = a.val[2];
	register float32x4_t a3 asm("q3") = a.val[3];
	asm volatile (
	"\n\t# multiply two matrices...\n\t"
	"# result (%q0,%q1,%q2,%q3)  = first column of B (%q4) * first row of A (q0-q3)\n\t"
	"vmul.f32 %q0, %q4, %e8[0]\n\t"
	"vmul.f32 %q1, %q4, %e9[0]\n\t"
	"vmul.f32 %q2, %q4, %e10[0]\n\t"
	"vmul.f32 %q3, %q4, %e11[0]\n\t"
	"# result (%q0,%q1,%q2,%q3) += second column of B (%q5) * second row of A (q0-q3)\n\t"
	"vmla.f32 %q0, %q5, %e8[1]\n\t"
	"vmla.f32 %q1, %q5, %e9[1]\n\t"
	"vmla.f32 %q2, %q5, %e10[1]\n\t"
	"vmla.f32 %q3, %q5, %e11[1]\n\t"
	"# result (%q0,%q1,%q2,%q3) += third column of B (%q6) * third row of A (q0-q3)\n\t"
	"vmla.f32 %q0, %q6, %f8[0]\n\t"
	"vmla.f32 %q1, %q6, %f9[0]\n\t"
	"vmla.f32 %q2, %q6, %f10[0]\n\t"
	"vmla.f32 %q3, %q6, %f11[0]\n\t"
	"# result (%q0,%q1,%q2,%q3) += last column of B (%q7) * last row of A (q0-q3)\n\t"
	"vmla.f32 %q0, %q7, %f8[1]\n\t"
	"vmla.f32 %q1, %q7, %f9[1]\n\t"
	"vmla.f32 %q2, %q7, %f10[1]\n\t"
	"vmla.f32 %q3, %q7, %f11[1]\n\t\n\t"
	: "=&w"  (result.val[0]), "=&w"  (result.val[1]), "=&w"  (result.val[2]), "=&w" (result.val[3])
	: "w"   (b.val[0]),      "w"   (b.val[1]),      "w"   (b.val[2]),      "w"   (b.val[3]),
	  "w"   (a0),            "w"   (a1),            "w"   (a2),            "w"   (a3)
	:
	);
}

void WWWWW(float32x4x4_t& result, const float32x4x4_t& a, const float32x4x4_t& b) {
	// this will generate near-optimal code:
	float32x4x4_t tmp;
	mat44_multiply_neon(tmp, a, b);
	mat44_multiply_neon(tmp, b, tmp);
	homogenize(tmp.val[2]);
	mat44_multiply_neon(result, tmp, a);
}

void vec3_cross_product(float32x4_t& result, const float32x4_t& a, const float32x4_t& b) {
    register float32x4_t v1 asm("q0") = a;
    register float32x4_t v2 asm("q2") = b;
    register float32x4_t& r asm("q8") = result;
    asm volatile(
        "vmov    q1, q0              \n\t"
        "vmov    q3, q2              \n\t"
        "vzip.32 q0, q1              \n\t" // d0,d1,d3 = xx1, yy1, zz1
        "vzip.32 q2, q3              \n\t" // d4,d5,d6 = xx2, yy2, zz2
        "vmul.f32 d16, d1, d6        \n\t" // x' = yy1 * zz2
        "vmul.f32 d18, d3, d4        \n\t" // y' = zz1 * xx2
        "vmul.f32 d17, d0, d5        \n\t" // z' = xx1 * yy2
        "vmls.f32 d16, d3, d5        \n\t" // x' = x' - zz1 * yy2
        "vmls.f32 d18, d0, d6        \n\t" // y' = y' - xx1 * zz2
        "vmls.f32 d17, d1, d4        \n\t" // z' = z' - yy1 * xx2
        "vuzp.32 d16, d18"                 // result.xyzw = xyzz'
        : "=w" (v1), "=w" (v2), "=w" (r)
        : "0" (v1), "1" (v2)
        : "q1", "q3", "q9"
    );
}

void vec3_cross_product_x4(float const* v1, float const* v2, float* result) {
    unsigned int const offset = 16; // 128-bit vectors

    asm volatile(
        "vld3.f32 {d0[0], d2[0], d4[0]}, [%[v1]], %[offset]         \n\t"
        "vld3.f32 {d0[1], d2[1], d4[1]}, [%[v1]], %[offset]         \n\t"
        "vld3.f32 {d1[0], d3[0], d5[0]}, [%[v1]], %[offset]         \n\t"
        "vld3.f32 {d1[1], d3[1], d5[1]}, [%[v1]], %[offset]         \n\t"
        "vld3.f32 {d16[0], d18[0], d20[0]}, [%[v2]], %[offset]      \n\t"
        "vld3.f32 {d16[1], d18[1], d20[1]}, [%[v2]], %[offset]      \n\t"
        "vld3.f32 {d17[0], d19[0], d21[0]}, [%[v2]], %[offset]      \n\t"
        "vld3.f32 {d17[1], d19[1], d21[1]}, [%[v2]], %[offset]      \n\t"
        "vmul.f32 q12, q1, q10                                      \n\t"
        "vmul.f32 q13, q2, q8                                       \n\t"
        "vmul.f32 q14, q0, q9                                       \n\t"
        "vmls.f32 q12, q2, q9                                       \n\t"
        "vmls.f32 q13, q0, q10                                      \n\t"
        "vmls.f32 q14, q1, q8                                       \n\t"
        "vst3.f32 {d24[0], d26[0], d28[0]}, [%[result]], %[offset]  \n\t"
        "vst3.f32 {d24[1], d26[1], d28[1]}, [%[result]], %[offset]  \n\t"
        "vst3.f32 {d25[0], d27[0], d29[0]}, [%[result]], %[offset]  \n\t"
        "vst3.f32 {d25[1], d27[1], d29[1]}, [%[result]], %[offset]"
        : [v1] "=r" (v1), [v2] "=r" (v2), [result] "=r" (result)
        : "0" (v1), "1" (v2), "2" (result), [offset] "r" (offset)
        : "memory", "q0", "q1", "q2", "q8", "q9", "q10", "q12", "q13", "q14"
    );
}

void mat44_transform_point4(float32x4_t& r, const float32x4x4_t& m, const float32x4_t& p) {
	register float32x4_t _p asm("q3") = p;
	asm volatile
	(
	"# Transform a point (%q5) using matrix M={%q1,%q2,%q3,%q4}, putting the result in %q0\n\t"
	"vmul.f32 %q0, %q1, %e5[0] @ %q0 = %q5.x * 1st column of M\n\t"
    "vmla.f32 %q0, %q2, %e5[1] @ %q0+= %q5.y * 2nd column of M\n\t"
    "vmla.f32 %q0, %q3, %f5[0] @ %q0+= %q5.z * 3rd column of M\n\t"
    "vmla.f32 %q0, %q4, %f5[1] @ %q0+= %q5.w * 4th column of M\n\t"
    : "=&w" (r)
    : "w" (m.val[0]), "w" (m.val[1]), "w" (m.val[2]), "w" (m.val[3]), "w" (_p)
    :
    );
}

void mat44_transform_point3(float32x4_t& r, const float32x4x4_t& m, const float32x4_t& p) {
	float32x4_t tmp;
	mat44_transform_point4(tmp,m,p);
	homogenize(tmp);
	r=tmp;
}

void fast_640x480xBGRA_to_320x240xY(void* destination_buffer, const void* source_data) {
	const uint8x8_t* __restrict__ bgraValues((const uint8x8_t*) source_data);
	uint8x8_t* __restrict__ greyValues((uint8x8_t*) destination_buffer);
	unsigned int off1(0);
	for (unsigned int y(0); y<240; ++y, greyValues+=80) {
		unsigned int off2(off1+320);
		for (unsigned int x=0; x<40; ++x, ++off1, ++off2) {
			// greyValues[x] = sum((bgraValues[off1] >> [1,2,1,1]) + (bgraValues[off2] >> [1,2,1,2]))
		}
		off1=off2;
	}
	// see http://hilbert-space.de/?p=22
	// http://torus.untergrund.net/code/rgb_to_gray.s
}

// Fast divide-image-by-2 function:
// http://forums.arm.com/index.php?/topic/14488-how-to-efficiently-sum-4-x-8bit-integers-with-arm-or-neon/page__p__34538#entry34538

void fast_downscale_8bpp(void* destination_buffer, const void* source_data, unsigned int source_width, unsigned int source_height) {
	const unsigned int* __restrict__ src((const unsigned int*) source_data);
	unsigned int* __restrict__ dst((unsigned int*) destination_buffer);
	unsigned int off1(0);
	for (unsigned int y=0; y<(source_height>>1); ++y, dst+=(source_width>>3)) {
		unsigned int off2(off1+(source_width>>2));
		for (unsigned int x=0; x<(source_width>>3); ++x) {
			unsigned int L1(0), L2(0), L3(0), L4(0);
			L1+=(src[off1]&0xff)+((src[off1]>>8)&0xff);
			L2+=((src[off1]>>16)&0xff)+((src[off1]>>24)&0xff);
			++off1;
			L3+=(src[off1]&0xff)+((src[off1]>>8)&0xff);
			L4+=((src[off1]>>16)&0xff)+((src[off1]>>24)&0xff);
			++off1;
			L1+=(src[off2]&0xff)+((src[off2]>>8)&0xff);
			L2+=((src[off2]>>16)&0xff)+((src[off2]>>24)&0xff);
			++off2;
			L3+=(src[off2]&0xff)+((src[off2]>>8)&0xff);
			L4+=((src[off2]>>16)&0xff)+((src[off2]>>24)&0xff);
			++off2;
			dst[x]=((L4<<22)&0xff000000) | ((L3<<14)&0xff0000) | ((L2<<6)&0xff00) | ((L1>>2)&0xff);
		}
		off1=off2;
	}
}

#include <arm_neon.h>
//#define CIRCLE 6.28318530717958647692528676655900576f
#define CIRCLE 0x1.921FB54442D18p2f

#undef __ARM_NEON__

void fastAtan2Rad(float& result, const float& y, const float& x ) {
	static const float q[] = { CIRCLE*.125f, CIRCLE*.375f };
	const bool cnd0(y<0.f), cnd1(x<0.f);
	const float yabs(cnd0?-y:y), a(x + yabs), b(x - yabs), c(cnd1?-a:b), d(cnd1?b:a);
	result  = 1.f/d;
	result *= c;
	result *= (.1821f*result*result - .9675f);
	result += q[cnd1];
	result  = cnd0?-result:result;
}

float fastAtan2Rad0(const float& y, const float& x ) {
	static const float q[] = { CIRCLE*.125f, CIRCLE*.375f };
	const bool cnd0(y<0.f), cnd1(x<0.f);
	const float yabs(cnd0?-y:y), a(x + yabs), b(x - yabs), c(cnd1?-a:b), d(cnd1?b:a);
	float result  = 1.f/d;
	result *= c;
	result *= (.1821f*result*result - .9675f);
	result += q[cnd1];
	return cnd0?-result:result;
}

template<typename T> const T& tmpref(const T& x) { return (const T&)*&x; }

const float32x4_t testtest(const float32x4_t& x) {
	float32x4_t z = vaddq_f32(x, x);
	z = vmulq_f32(z, x);
	float32x4_t* ret = &z;
	return z;
}

void testtest2(const float& y, const float& x) {
	volatile float r;
	r = fastAtan2Rad0(y,x);
}

void fastAtan2Deg(float& result, const float& y, const float& x ) {
	static const float q[] = { 45.f, 3.f*45.f };
	const bool cnd0(y<0.f), cnd1(x<0.f);
	const float yabs(cnd0?-y:y), a(x + yabs), b(x - yabs), c(cnd1?-a:b), d(cnd1?b:a);
	result  = 1.f/d;
	result *= c;
	result *= (10.43356144933229063166f*result*result - 55.43366667890714544829f);
	result += q[cnd1];
	result  = cnd0?-result:result;
}

extern unsigned int init_rand_state();

unsigned int rand(unsigned int high) {
	static unsigned int state=init_rand_state();
	state *= 1103515245;
	state += 12345;
	return (high*(state>>16))>>16;
}


void vAtan2Deg(float* a, const float* y, const float* x, size_t n) {
#ifdef __ARM_NEON__
		static const float constants[] = {
			10.43356144933229063166f,
			-55.43366667890714544829f,
			135.f,
			45.f
		};
		size_t m(n&3);
		n=n&~3;
		asm volatile("mov r1, %[n]\n\t"
					 "vldmia %[constants], {q6}\n\t"
					 "2:\n\t"
					 "vmov.f32 q0, #0.0\n\t"	// q0=0.
					 "vldmia %[y]!, {q1}\n\t"	// q1=y.
					 "vldmia %[x]!, {q2}\n\t"	// q2=x.
					 "vclt.f32 q3, q1, q0\n\t"	// q3=(y<0)=cnd0
					 "vclt.f32 q4, q2, q0\n\t"	// q4=(x<0)=cnd1
					 "vabs.f32 q1, q1\n\t"		// q1=abs(y).
					 "vadd.f32 q0, q2, q1\n\t"	// q0=x + abs(y)=a.
					 "vsub.f32 q1, q2, q1\n\t"	// q1=x - abs(y)=b.
					 "vneg.f32 q5, q0\n\t"		// q5=-a.
					 "vand.f32 q2, q5, q4\n\t"	// q2=-a if cnd1=c.
					 "vbif.f32 q2, q1, q4\n\t"	// q2=b if !cnd1=c.
					 "vand.f32 q5, q1, q4\n\t"	// q5=b if cnd1=d.
					 "vbif.f32 q5, q0, q4\n\t"	// q5=a if !cnd1=d.
					 "vrecpe.f32 q5, q5\n\t"	// q5=1/d
					 "vdup.f32 q0, d12[0]\n\t"	// q0=10.43356144933229063166
					 "vdup.f32 q6, d12[1]\n\t"	// q6=-55.43366667890714544829
					 "vmul.f32 q2, q2, q5\n\t"	// q2=c/d=r
					 "vmul.f32 q1, q2, q2\n\t"	// q1=r*r
					 "vmla.f32 q6, q1, q0\n\t"	// q6=10.43356144933229063166f*r*r - 55.43366667890714544829f
					 "vmul.f32 q0, q2, q6\n\t"	// q0=r*q6
					 "vdup.f32 q1, d13[0]\n\t"	// q1=135
					 "vdup.f32 q2, d13[1]\n\t"	// q2=45
					 "vand.f32 q5, q1, q4\n\t"	// q5=135 if cnd1
					 "vbif.f32 q5, q2, q4\n\t"	// q5=45 if !cnd1
					 "vadd.f32 q0, q0, q5\n\t"	// q0+=q5
					 "vneg.f32 q1, q0\n\t"		// q1=-q0
					 "vbit.f32 q0, q1, q3\n\t"	// q0=-q0 if cnd0
					 "vstmia %[a]!, {q0}\n\t"
					 "subs r1, r1, #4\n\t"
					 "bne 2b\n\t"
					 : [a] "+r" (a), [y] "+r" (y), [x] "+r" (x)
					 : [n] "r" (n), [constants] "r" (constants)
					 : "memory", "cc", "r1", "q0", "q1", "q2", "q3", "q4", "q5", "q6"
					 );
		if (!m) return; *a=fastAtan2Deg(*y, *x); ++a; ++y; ++x; --m;
		if (!m) return; *a=fastAtan2Deg(*y, *x); ++a; ++y; ++x; --m;
		if (!m) return; *a=fastAtan2Deg(*y, *x); ++a; ++y; ++x;
#else
		for (size_t i(0); i<(n&~3); i+=4) {
			fastAtan2Deg(a[i  ], y[i  ], x[i  ]);
			fastAtan2Deg(a[i+1], y[i+1], x[i+1]);
			fastAtan2Deg(a[i+2], y[i+2], x[i+2]);
			fastAtan2Deg(a[i+3], y[i+3], x[i+3]);
		}
		for (size_t i(n&~3); i<n; ++i) fastAtan2Deg(a[i], y[i], x[i]);
#endif
	}

	// Long vector squared euclidean distance. n must be a multiple of 4.
	void vSquaredDistance(float& result, const float* a, const float* b, unsigned int n) {
#ifdef __ARM_NEON__
		asm volatile ("vmov.f32 q0, #0.0\n\t"
					  "1:\n\t"
					  "vldmia %[a]!, {q1}\n\t"
					  "vldmia %[b]!, {q2}\n\t"
					  "vsub.f32 q1, q1, q2\n\t" // q1-=q2
					  "vmla.f32 q0, q1, q1\n\t" // q0+=q1*q1
					  "subs %[n], %[n], #4\n\t"
					  "bne 1b\n\t" 
					  "vpadd.f32 d0,d0\n\t" // q0.lo=q0.lo[0]+q0.lo[1]
					  "vpadd.f32 d1,d1\n\t" // q0.hi=q0.hi[0]+q0.hi[1]
					  "vadd.f32  d0,d1\n\t" // q0.lo[1]=q0.hi[0]
					  "vmov.f32 %[result], s0\n\t"
					  : [a] "+r" (a), [b] "+r" (b), [n] "+r" (n), [result] "=w" (result)
					  :
					  : "memory", "cc", "q0", "q1", "q2"
					  );
#else
		result=0.f;
		for (unsigned int i(0); i<n; ++i) {
			const float tmp(a[i] - b[i]);
			result+=tmp*tmp;
		}
#endif
	}

	// Long vector dot product. n must be a multiple of 4.
	void vDot(float& result, const float* a, const float* b, unsigned int n) {
#ifdef __ARM_NEON__
		asm volatile ("vmov.f32 q0, #0.0\n\t"
					  "1:\n\t"
					  "vldmia %[a]!, {q1}\n\t"
					  "vldmia %[b]!, {q2}\n\t"
					  "vmla.f32 q0, q1, q2\n\t" // q0+=q1*q2
					  "subs %[n], %[n], #4\n\t"
					  "bne 1b\n\t" 
					  "vpadd.f32 d0,d0\n\t" // q0.lo=q0.lo[0]+q0.lo[1]
					  "vpadd.f32 d1,d1\n\t" // q0.hi=q0.hi[0]+q0.hi[1]
					  "vadd.f32  s0, s2\n\t" // q0.lo[1]=q0.hi[0]
					  "vmov.f32 %[result], s0\n\t"
					  : [a] "+r" (a), [b] "+r" (b), [n] "+r" (n), [result] "=w" (result)
					  : 
					  : "memory", "cc", "q0", "q1", "q2"
					  );
#else
		result=0.f;
		for (unsigned int i(0); i<n; ++i) {
			result+=a[i]*b[i];
		}
#endif
	}
	
	// Long vector squared magnitude. n must be a multiple of 4.
	void vSquareMagnitude(float& result, const float* v, unsigned int n) {
#ifdef __ARM_NEON__
		asm volatile (//"vand.i32 q0, #0\n\t"
					  "vmov.f32 q0, #0.0\n\t"
					  "1:\n\t"
					  "vldmia %[v]!, {q1}\n\t"
					  "vmla.f32 q0, q1, q1\n\t"
					  "subs %[n], %[n], #4\n\t"
					  "bne 1b\n\t" 
					  "vpadd.f32 d0,d0\n\t"
					  "vpadd.f32 d1,d1\n\t"
					  "vadd.f32  d0,d1\n\t"
					  "vmov.f32 %[result], s0\n\t"
					  : [v] "+r" (v), [n] "+r" (n), [result] "=w" (result)
					  : 
					  : "memory", "cc", "q0", "q1"
					  );
#else
		result=0.f;
		for (unsigned int i(0); i<n; i+=4) {
			result+=v[i  ]*v[i  ];
			result+=v[i+1]*v[i+1];
			result+=v[i+2]*v[i+2];
			result+=v[i+3]*v[i+3];
		}
#endif
	}
	
	void vScale(float* v, float s, unsigned int n) {
#ifdef __ARM_NEON__
		asm volatile ("vmov.f32 s0, %[s]\n\t" // s0 = [s]
					  "vmov.f32 s1, %[s]\n\t" // d0 = [s,s]
					  "vmov.f32 d1, d0\n\t"   // q0=[s,s,s,s]
					  "3:\n\t"
					  "mov r1, %[v]\n\t"
					  "vldmia %[v]!, {q1}\n\t"
					  "vmul.f32 q1, q1, q0\n\t"
					  "vstmia r1, {q1}\n\t"
					  "subs %[n], %[n], #4\n\t"
					  "bne 3b\n\t"
					  : [v] "+r" (v), [n] "+r" (n)
					  : [s] "w" (s) 
					  : "memory", "cc", "r1", "q0", "q1"
					  );
#else
		for (unsigned int i(0); i<n; i+=4) {
			v[i  ]*=s;
			v[i+1]*=s;
			v[i+2]*=s;
			v[i+3]*=s;
		}
#endif
	}
	
	// Long vector normalize. n must be a multiple of 4.
	void vNormalize(float* v, unsigned int n) {
#ifdef __ARM_NEON__
		asm volatile ("vmov.f32 q0, #0.0\n\t"
					  "push {%[v], %[n]}\n\t"
					  "1:\n\t"
					  "vldmia %[v]!, {q1}\n\t"
					  "vmla.f32 q0, q1, q1\n\t"
					  "subs %[n], %[n], #4\n\t"
					  "bne 1b\n\t" 
					  "vpadd.f32 d0,d0\n\t"
					  "vpadd.f32 d1,d1\n\t"
					  "vadd.f32  d0,d1\n\t"
					  "vrsqrte.f32 d0,d0\n\t"
					  "pop {%[v], %[n]}\n\t"
					  "vmov.f32  d1,d0\n\t" // q0 = 1/len
					  "3:\n\t"
					  "mov r1, %[v]\n\t"
					  "vldmia %[v]!, {q1}\n\t"
					  "vmul.f32 q1, q1, q0\n\t"
					  "vstmia r1, {q1}\n\t"
					  "subs %[n], %[n], #4\n\t"
					  "bne 3b\n\t"
					  : [v] "+r" (v), [n] "+r" (n)
					  : 
					  : "memory", "cc", "r1", "q0", "q1"
					  );
#else
		float tmp(0.f);
		for (unsigned int i(0); i<n; i+=4) {
			tmp+=v[i  ]*v[i  ];
			tmp+=v[i+1]*v[i+1];
			tmp+=v[i+2]*v[i+2];
			tmp+=v[i+3]*v[i+3];
		}
		tmp=1.f/sqrtf(tmp);
		for (unsigned int i(0); i<n; i+=4) {
			v[i  ]*=tmp;
			v[i+1]*=tmp;
			v[i+2]*=tmp;
			v[i+3]*=tmp;
		}
#endif
	}
	
	
	
	extern void sgeqrf_(int*,int*,float*,int*,float*,float*,int*,int*);
	extern void sorgqr_(int*,int*,int*,float*,int*,float*,float*,int*,int*);
	bool find_homography_4_pairs(float* __restrict__ H,
											   const float* __restrict__ from, 
											   const float* __restrict__ to,
											   const unsigned int* __restrict__ indices) {
		int m(9);
		int n(8);
		int lwork(8*64);
		float A[9*9]; /* this needs to be m*m to hold the final Q matrix */
		float tau[8];
		float work[8*64];
		float* B(A);
		for(size_t i(0); i<4; ++i, B=B+18) {
			const unsigned int j(indices[i]<<1);
			const float& sx(from[j]);
			const float& sy(from[j+1]);
			const float& dx(to[j]);
			const float& dy(to[j+1]);
			
			B[0] = 0.f;		B[1] = 0.f;		B[2] = 0.f; //  [0,0,0]
			B[3] =-sx;		B[4] =-sy;		B[5] =-1.f; // -[sx,sy,1]
			B[6] = dy*sx;	B[7] = dy*sy;	B[8] = dy;  // [sx,sy,1]*dy
			//
			B[ 9]  = sx;	B[10] = sy;		B[11] = 1.f; // [sx,sy,1]
			B[12] = 0.f;	B[13] = 0.f;	B[14] = 0.f; // [0,0,0]
			B[15] =-dx*sx;	B[16] =-dx*sy;	B[17] = -dx; // [sx,sy,1]*-dx
		}
		int info1, info2;
		sgeqrf_(&m, &n, A, &m, tau, work, &lwork, &info1);
		sorgqr_(&m, &m, &n, A, &m, tau, work, &lwork, &info2);
		const float* Res(&A[9*8]);
		const float norm(1.f/Res[8]);
		for(size_t i(0); i<9; ++i) H[i] = Res[i]*norm;
		return (!info1 && !info2);
	}
	

	float dot4_neon(float v0[4], float v1[4]) {
		float r;
		asm volatile (
				  "vld1.32				{d2, d3}, [%1] \n\t"   //d2={x0,y0}, d3={z0, w0}
				  "vld1.32				{d4, d5}, [%2] \n\t"   //d4={x1,y1}, d5={z1, w1}
				  "vmul.f32			   d0, d2, d4	 \n\t"   //d0= d2*d4
				  "vmla.f32			   d0, d3, d5	 \n\t"   //d0 = d0 + d3*d5
				  // 8-cycle stall because of vmla… better do 4 dots at once
				  "vpadd.f32			  d0, d0		 \n\t"   //d0 = d[0] + d[1]
				  "vmov.f32			   %0, s0		 \n\t"   //r0 = s0
				  :"=r"(r): "r"(v0), "r"(v1): 
				  );	  
		return r;
	}
