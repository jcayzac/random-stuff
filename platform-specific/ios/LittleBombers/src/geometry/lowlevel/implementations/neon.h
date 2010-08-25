#if !defined(UUID_7C26F387D42E478F8B7D02A20D1F73EB)
#define UUID_7C26F387D42E478F8B7D02A20D1F73EB
#ifdef __ARM_NEON__

#include "geometry/lowlevel/implementations/default.h"

namespace geometry {
namespace lowlevel {
	enum { platform_neon = 1 };
	
#define spec(t) template<> inline t implementation<platform_neon>

	spec(void)::m_copy(float* __restrict__ r, const float* __restrict__ rhs) {
		asm volatile
		(
		"vldmia %1, { q0-q3 }\n\t"
		"vstmia %0, { q0-q3 }\n\t"
		:: "r"(r), "r"(rhs)
		:  "memory","q0","q1","q2","q3"
		);
	}
	
	spec(void)::sincos2(float* __restrict__ sc, const float* __restrict__ angles) {
		static const float constants[]  = { 
			/* q1 */  0,                M_PI_2,           0,                M_PI_2,
			/* q2 */  M_PI,             M_PI,             M_PI,             M_PI,
			/* q3 */  4.f/M_PI,         4.f/M_PI,         4.f/M_PI,         4.f/M_PI,
			/* q4 */ -4.f/(M_PI*M_PI), -4.f/(M_PI*M_PI), -4.f/(M_PI*M_PI), -4.f/(M_PI*M_PI),
			/* q5 */  2.f,              2.f,              2.f,              2.f,
			/* q6 */  .225f,            .225f,            .225f,            .225f
		};  
		asm volatile(
					 // Load q0 with [angle1,angle1,angle2,angle2]
					 "vldmia %1, { d3 }\n\t"
					 "vdup.f32 d0, d3[0]\n\t"
					 "vdup.f32 d1, d3[1]\n\t"
					 // Load q1-q6 with constants
					 "vldmia %2, { q1-q6 }\n\t"
					 // Cos(x) = Sin(x+PI/2), so
					 // q0 = [angle1, angle1+PI/2, angle2, angle2+PI/2]
					 "vadd.f32 q0,q0,q1\n\t"
					 // if angle1+PI/2>PI, substract 2*PI
					 // q0-=(q0>PI)?2*PI:0
					 "vcge.f32 q1,q0,q2\n\t"
					 "vand.f32 q1,q1,q2\n\t"
					 "vmls.f32 q0,q1,q5\n\t"
					 // q0=(4/PI)*q0 - q0*abs(q0)*4/(PI*PI)
					 "vabs.f32 q1,q0\n\t"
					 "vmul.f32 q1,q0,q1\n\t"
					 "vmul.f32 q0,q0,q3\n\t"
					 "vmul.f32 q1,q1,q4\n\t"
					 "vadd.f32 q0,q0,q1\n\t"
					 // q0+=.225*(q0*abs(q0) - q0)
					 "vabs.f32 q1,q0\n\t"
					 "vmul.f32 q1,q0,q1\n\t"
					 "vsub.f32 q1,q0\n\t"
					 "vmla.f32 q0,q1,q6\n\t"
					 "vstmia %0, { q0 }\n\t"
					 :: "r"(sc), "r"(angles), "r"(constants)
					 : "memory","cc","q0","q1","q2","q3","q4","q5","q6"
					 );  
	}
	
	spec(void)::m_mul (float* __restrict__ r, const float* __restrict__ lhs, const float* __restrict__ rhs) {
		asm volatile 
		(
		 "vldmia %1, { q0-q3 }\n\t"	// Store A & B leaving room for q4-q7, which should be preserved
		 "vldmia %2, { q8-q11 }\n\t"
		 
		 "vmul.f32 q12, q8, d0[0]\n\t"	// result = first column of B x first row of A
		 "vmul.f32 q13, q8, d2[0]\n\t"
		 "vmul.f32 q14, q8, d4[0]\n\t"
		 "vmul.f32 q15, q8, d6[0]\n\t"
		 
		 "vmla.f32 q12, q9, d0[1]\n\t"	// result += second column of B x second row of A
		 "vmla.f32 q13, q9, d2[1]\n\t"
		 "vmla.f32 q14, q9, d4[1]\n\t"
		 "vmla.f32 q15, q9, d6[1]\n\t"
		 
		 "vmla.f32 q12, q10, d1[0]\n\t"	// result += third column of B x third row of A
		 "vmla.f32 q13, q10, d3[0]\n\t"
		 "vmla.f32 q14, q10, d5[0]\n\t"
		 "vmla.f32 q15, q10, d7[0]\n\t"
		 
		 "vmla.f32 q12, q11, d1[1]\n\t"	// result += last column of B x last row of A
		 "vmla.f32 q13, q11, d3[1]\n\t"
		 "vmla.f32 q14, q11, d5[1]\n\t"
		 "vmla.f32 q15, q11, d7[1]\n\t"
		 
		 "vstmia %0, { q12-q15 }\n\t"   // output = result registers
		 ::"r"(r),"r"(lhs),"r"(rhs)
		 :"memory","q0","q1","q2","q3","q8","q9","q10","q11","q12","q13","q14","q15"
		 );
	}
	
#undef spec(t)

}; // lowlevel
}; // geometry

#endif // __ARM_NEON__
#endif // UUID_7C26F387D42E478F8B7D02A20D1F73EB

