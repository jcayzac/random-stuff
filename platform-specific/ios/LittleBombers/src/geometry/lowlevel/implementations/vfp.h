#if !defined(UUID_4F2E52BDCCD24EDE9E60B519E5EC2E60)
#define UUID_4F2E52BDCCD24EDE9E60B519E5EC2E60
#ifdef __VFP_FP__
#include "geometry/lowlevel/implementations/default.h"

namespace geometry {
namespace lowlevel {
	
	enum { platform_vfp = 2 };
#define spec(t) template<> inline t implementation<platform_vfp>

	spec(void)::sincos(float* sc, const float angle) {
		static const float M_PI_65536 = ((float)M_PI/65536.f);
		static const float M_65536_PI = (65536.f/(float)M_PI);
		long scaled[2];
		scaled[0]=((long)(angle*M_65536_PI));
		scaled[1]=scaled[0]+32768; // cos(x) is sin(x+M_PI_2)
		sc[0]=((float)(scaled[0]&65535))*((scaled[0]&65536)?-M_PI_65536:M_PI_65536);
		sc[0]=(1.27323954473516268615f - .40528473456935108577f*fabsf(sc[0]))*sc[0];
		sc[1]=((float)(scaled[0]&65535))*((scaled[0]&65536)?-M_PI_65536:M_PI_65536);
		sc[1]=(1.27323954473516268615f - .40528473456935108577f*fabsf(sc[1]))*sc[1];
		sc[0]*=0.225f*fabsf(sc[0]) + 0.775f;
		sc[1]*=0.225f*fabsf(sc[1]) + 0.775f;
	}
	
	spec(void)::m_mul (float* __restrict__ r, const float* __restrict__ lhs, const float* __restrict__ rhs) {
		asm volatile
		(
		 "fmrx    r0, fpscr                         \n\t"
		 "bic     r0, r0, #0x00370000               \n\t"
		 "orr     r0, r0, #0x00030000               \n\t"
		 "fmxr    fpscr, r0                         \n\t"
		 // Interleaving loads and adds/muls for faster calculation.
		 // Let A:=src_ptr_1, B:=src_ptr_2, then
		 // function computes A*B as (B^T * A^T)^T.
		 
		 // Load the whole matrix into memory.
		 "fldmias  %1, {s8-s23}    \n\t"
		 // Load first column to scalar bank.
		 "fldmias  %2!, {s0-s3}    \n\t"
		 // First column times matrix.
		 "fmuls s24, s8, s0        \n\t"
		 "fmacs s24, s12, s1       \n\t"
		 
		 // Load second column to scalar bank.
		 "fldmias %2!,  {s4-s7}    \n\t"
		 
		 "fmacs s24, s16, s2       \n\t"
		 "fmacs s24, s20, s3       \n\t"
		 // Save first column.
		 "fstmias  %0!, {s24-s27}  \n\t" 
		 
		 // Second column times matrix.
		 "fmuls s28, s8, s4        \n\t"
		 "fmacs s28, s12, s5       \n\t"
		 
		 // Load third column to scalar bank.
		 "fldmias  %2!, {s0-s3}    \n\t"
		 
		 "fmacs s28, s16, s6       \n\t"
		 "fmacs s28, s20, s7       \n\t"
		 // Save second column.
		 "fstmias  %0!, {s28-s31}  \n\t" 
		 
		 // Third column times matrix.
		 "fmuls s24, s8, s0        \n\t"
		 "fmacs s24, s12, s1       \n\t"
		 
		 // Load fourth column to scalar bank.
		 "fldmias %2,  {s4-s7}    \n\t"
		 
		 "fmacs s24, s16, s2       \n\t"
		 "fmacs s24, s20, s3       \n\t"
		 // Save third column.
		 "fstmias  %0!, {s24-s27}  \n\t" 
		 
		 // Fourth column times matrix.
		 "fmuls s28, s8, s4        \n\t"
		 "fmacs s28, s12, s5       \n\t"
		 "fmacs s28, s16, s6       \n\t"
		 "fmacs s28, s20, s7       \n\t"
		 // Save fourth column.
		 "fstmias  %0!, {s28-s31}  \n\t" 
		 
		 "fmrx    r0, fpscr            \n\t"
		 "bic     r0, r0, #0x00370000  \n\t"
		 "fmxr    fpscr, r0            \n\t"
		 :: "r"(r), "r"(rhs), "r"(lhs)
		 : "r0", "cc", "memory",
		 "s0","s1","s2","s3","s4","s5","s6","s7","s8","s9","s10","s11","s12","s13","s14","s15",
		 "s16","s17","s18","s19","s20","s21","s22","s23","s24","s25","s26","s27","s28","s29","s30","s31"
		 );
		
	}
	
#undef spec(t)
}; // lowlevel
}; // geometry

#endif
#endif // UUID_4F2E52BDCCD24EDE9E60B519E5EC2E60

