#pragma once

#include <sys/time.h>
#include <Accelerate/Accelerate.h>
#include <arm_neon.h>

namespace accel {

	struct xy_t { float x,y; };
	
	
// Full circle, in radians (I don't like PI)
#define CIRCLE 6.28318530717958647692528676655900576f

static inline float fastAtan2Rad(const float& y, const float& x ) {
	static const float q[] = { CIRCLE*.125f, CIRCLE*.375f };
	const bool cnd0(y<0.f), cnd1(x<0.f);
	const float yabs(fabsf(y)), a(x + yabs), b(x - yabs), c(cnd1?-a:b), d(cnd1?b:a);
	float r[2];
	r[0]=(c/d);
	r[0] *= (.1821f*r[0]*r[0] - .9675f);
	r[0] += q[cnd1];
	r[1] = -r[0];
	return r[cnd0];
}
static inline float fastAtan2Deg(const float& y, const float& x ) {
	static const float q[] = { 45.f, 3.f*45.f };
	const bool cnd0(y<0.f), cnd1(x<0.f);
	const float yabs(fabsf(y)), a(x + yabs), b(x - yabs), c(cnd1?-a:b), d(cnd1?b:a);
	float r[2];
	r[0] = (c*(1.f/d));
	r[0] *= (10.43356144933229063166f*r[0]*r[0] - 55.43366667890714544829f);
	r[0] += q[cnd1];
	r[1]  = -r[0];
	return r[cnd0];
}

// Random number generator
static unsigned int init_rand_state() {
	struct ::timeval tv;
	gettimeofday(&tv, 0);
	return (unsigned int)(tv.tv_sec)^(unsigned int)(tv.tv_usec);
}

static inline unsigned int rand(unsigned int high) {
	//return (unsigned int)((double(high)*std::rand())/double(RAND_MAX));
	static unsigned int state=init_rand_state();
	state *= 1103515245;
	state += 12345;
	return (high*(state>>16))>>16;
}


	// Vector version of fastAtan2Deg
	inline __attribute__((always_inline)) void vAtan2Deg(float* a, const float* y, const float* x, size_t n) {
#ifdef __ARM_NEON__DOESNT_WORK_ANYMORE
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
					 "vldmia %[y]!, {d2,d3}\n\t"// q1=y.
					 "vldmia %[x]!, {d4,d5}\n\t"// q2=x.
					 "vmov.f32 q0, #0.0\n\t"	// q0=0.
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
					 "vdup.f32 q1, d13[0]\n\t"	// q1=135
					 "vmul.f32 q0, q2, q6\n\t"	// q0=r*q6
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
			a[i  ]=fastAtan2Deg(y[i  ], x[i  ]);
			a[i+1]=fastAtan2Deg(y[i+1], x[i+1]);
			a[i+2]=fastAtan2Deg(y[i+2], x[i+2]);
			a[i+3]=fastAtan2Deg(y[i+3], x[i+3]);
		}
		for (size_t i(n&~3); i<n; ++i) a[i]=fastAtan2Deg(y[i], x[i]);
#endif
	}

#ifdef DOESNT_WORK_AT_THE_MOMENT
	// Interleaved version of vAtan2Deg()
	inline __attribute__((always_inline)) void vAtan2DegI(float* a, const float* xy, size_t n) {
#ifdef __ARM_NEON__DOESNT_WORK_ANYMORE
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
					 "vld2.32 {d0,d1}, [%[xy]]!\n\t"// q2=x.
					 "vmov.32 d4, d0\n\t"
					 "vmov.32 d2, d1\n\t"
					 "vld2.32 {d0,d1}, [%[xy]]!\n\t"// q2=x.
					 "vmov.32 d5, d0\n\t"
					 "vmov.32 d3, d1\n\t"
					 "vmov.f32 q0, #0.0\n\t"	// q0=0.
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
					 "vdup.f32 q1, d13[0]\n\t"	// q1=135
					 "vmul.f32 q0, q2, q6\n\t"	// q0=r*q6
					 "vdup.f32 q2, d13[1]\n\t"	// q2=45
					 "vand.f32 q5, q1, q4\n\t"	// q5=135 if cnd1
					 "vbif.f32 q5, q2, q4\n\t"	// q5=45 if !cnd1
					 "vadd.f32 q0, q0, q5\n\t"	// q0+=q5
					 "vneg.f32 q1, q0\n\t"		// q1=-q0
					 "vbit.f32 q0, q1, q3\n\t"	// q0=-q0 if cnd0					 
					 "vstmia %[a]!, {q0}\n\t"
					 "subs r1, r1, #4\n\t"
					 "bne 2b\n\t"
					 : [a] "+r" (a), [xy] "+r" (xy)
					 : [n] "r" (n), [constants] "r" (constants)
					 : "memory", "cc", "r1", "q0", "q1", "q2", "q3", "q4", "q5", "q6"
					 );
		while (m) {
			const float& x(*xy); ++xy;
			const float& y(*xy); ++xy;
			*a=fastAtan2Deg(y, x); ++a;
			--m;
		}
#else
#error not implemented
#endif
	}
#endif
	
	// Long vector squared euclidean distance. n must be a multiple of 4.
	inline __attribute__((always_inline)) void vSquaredDistance(float& result, const float* a, const float* b, size_t n) {
#ifdef __ARM_NEON__DOESNT_WORK_ANYMORE
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
		for (size_t i(0); i<n; ++i) {
			const float tmp(a[i] - b[i]);
			result+=tmp*tmp;
		}
#endif
	}

	// Long vector dot product. n must be a multiple of 4.
	inline __attribute__((always_inline)) void vDot(float& result, const float* a, const float* b, size_t n) {
#ifdef __ARM_NEON__DOESNT_WORK_ANYMORE
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
		for (size_t i(0); i<n; ++i) {
			result+=a[i]*b[i];
		}
#endif
	}
	
	// Long vector squared magnitude. n must be a multiple of 4.
	inline __attribute__((always_inline)) void vSquareMagnitude(float& result, const float* v, size_t n) {
#ifdef __ARM_NEON__DOESNT_WORK_ANYMORE
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
		for (size_t i(0); i<n; i+=4) {
			result+=v[i  ]*v[i  ];
			result+=v[i+1]*v[i+1];
			result+=v[i+2]*v[i+2];
			result+=v[i+3]*v[i+3];
		}
#endif
	}
	
	// Scale a vector by a scalar
	inline __attribute__((always_inline)) void vScale(float* v, float s, size_t n) {
#ifdef __ARM_NEON__DOESNT_WORK_ANYMORE
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
		for (size_t i(0); i<n; i+=4) {
			v[i  ]*=s;
			v[i+1]*=s;
			v[i+2]*=s;
			v[i+3]*=s;
		}
#endif
	}

	// Copy a vector
	inline __attribute__((always_inline)) void MemCopy(void* dst, const void* src, size_t n) {
		if ((n & 15) == 0) {
#ifdef __ARM_NEON__DOESNT_WORK_ANYMORE
			asm volatile ("3:\n\t"
						  "vldmia %[src]!, {q7}\n\t"
						  "vstmia %[dst]!, {q7}\n\t"
						  "subs %[n], %[n], #16\n\t"
						  "bne 3b\n\t"
						  : [dst] "+r" (dst), [src] "+r" (src), [n] "+r" (n)
						  :
						  : "memory", "cc", "q7"
						  );
#else
			unsigned int* d = (unsigned int*)dst;
			unsigned int* s = (unsigned int*)src;
			for (size_t i(0); i<n; i+=4) {
				const size_t j(i>>2);
				d[j  ]=s[j  ];
				d[j+1]=s[j+1];
				d[j+2]=s[j+2];
				d[j+3]=s[j+3];
			}
#endif
		}
		else memcpy(dst, src, n);
	}
	
	
	
	// Clear a vector… a.k.a "faster memset".
	inline __attribute__((always_inline)) void vClear(void* v, size_t n) {
		size_t m(n&3);
		size_t size(n&~3);
		n=size;
		unsigned int* w = (unsigned int*)v;
#ifdef __ARM_NEON__DOESNT_WORK_ANYMORE
		asm volatile ("vmov.u32 q0, #0\n\t"
					  "3:\n\t"
					  "vstmia %[v]!, {q0}\n\t"
					  "subs %[n], %[n], #4\n\t"
					  "bne 3b\n\t"
					  : [v] "+r" (v), [n] "+r" (n)
					  : 
					  : "memory", "cc", "q0"
					  );
#else
		for (size_t i(0); i<n; i+=4) {
			w[i  ]=0;
			w[i+1]=0;
			w[i+2]=0;
			w[i+3]=0;
		}
#endif
		while(m) {
			w[size] = 0;
			++size;
			--m;
		}
	}
	
	
	
	// Long vector normalize. n must be a multiple of 4.
	inline __attribute__((always_inline)) void vNormalize(float* v, size_t n) {
#ifdef __ARM_NEON__DOESNT_WORK_ANYMORE
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
		for (size_t i(0); i<n; i+=4) {
			tmp+=v[i  ]*v[i  ];
			tmp+=v[i+1]*v[i+1];
			tmp+=v[i+2]*v[i+2];
			tmp+=v[i+3]*v[i+3];
		}
		tmp=1.f/sqrtf(tmp);
		for (size_t i(0); i<n; i+=4) {
			v[i  ]*=tmp;
			v[i+1]*=tmp;
			v[i+2]*=tmp;
			v[i+3]*=tmp;
		}
#endif
	}
	
	
	
	
	/** find_homography computes a homography from the four points
	 * defined in <i>from</i> into the four points defined into <i>to</i>.
	 * The homography is calculated using QR factorization.
	 * @param from array of eight floats defining the source points
	 * @param to array of eight floats defining the destination points
	 * @returns array of nine floats defining the homography in row-major
	 **/
	static inline bool find_homography_4_pairs(float* __restrict__ H,
											   const float* __restrict__ from, 
											   const float* __restrict__ to,
											   const unsigned int* __restrict__ indices) {
		__CLPK_integer m(9);
		__CLPK_integer n(8);
		__CLPK_integer lwork(8*64);
		float A[9*9]; /* this needs to be m*m to hold the final Q matrix */
		float tau[8];
		float work[8*64];
		// Set-up A matrix to solve Ax=0, i.e. find the nullspace of A.
		// A is 8x9, which wont work for nullspace extraction from
		// QR factorization (wont work with SVD neither...),
		// therefore we assume A to be transposed:
		// Define it using row-major but tell BLAS/LAPACK
		// column-major (prefered format anyways).
		// We have to transpose the result as well then.
		//
		// Refer to Hartley and Zisserman, "Multiple View Geometry"
		// (Section 4.1, the DLT algorithm) for an explanation and
		// deduction of matrix A.
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
		
		// compute QR factorization using LAPACK, then extract Q
		__CLPK_integer info1, info2;
		sgeqrf_(&m, &n, A, &m, tau, work, &lwork, &info1);
		sorgqr_(&m, &m, &n, A, &m, tau, work, &lwork, &info2);
		
		// The last column of Q consists of the nullspace of
		// A, which is the solution and thus the desired
		// homography. However, the result now is transposed
		// as well (see above) but
		// still column-major; therefore it is already
		// the correct result in row-major.
		// We need to normalize but then we are done.
		const float* Res(&A[9*8]);
		const float norm(1.f/Res[8]);
		for(size_t i(0); i<9; ++i) H[i] = Res[i]*norm;
		return (!info1 && !info2);
	}
	
#ifdef __ARM_NEON__DOESNT_WORK_ANYMORE
static inline float dot4_neon(float v0[4], float v1[4]) {
	float r;
	asm volatile (
				  "vld1.32                {d2, d3}, [%1] \n\t"   //d2={x0,y0}, d3={z0, w0}
				  "vld1.32                {d4, d5}, [%2] \n\t"   //d4={x1,y1}, d5={z1, w1}
				  "vmul.f32               d0, d2, d4     \n\t"   //d0= d2*d4
				  "vmla.f32               d0, d3, d5     \n\t"   //d0 = d0 + d3*d5
				  // 8-cycle stall because of vmla… better do 4 dots at once
				  "vpadd.f32              d0, d0         \n\t"   //d0 = d[0] + d[1]
				  "vmov.f32               %0, s0         \n\t"   //r0 = s0
				  :"=r"(r): "r"(v0), "r"(v1):
				  );      
	return r;
}
#endif

}
