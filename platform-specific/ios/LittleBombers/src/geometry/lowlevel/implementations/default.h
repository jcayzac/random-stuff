#if !defined(UUID_3B10B64678D4A0497D0D78647CCA5E2)
#define UUID_3B10B64678D4A0497D0D78647CCA5E2

#include <math.h>

namespace geometry {
namespace lowlevel {
	
	template<int granularity>
	struct copy_type { float v[granularity]; };

	template<int P, int count>
	struct sumer { float operator()(const float*v) { return (*v)+sumer<P, count-1>()(v+1); } };
	template<int P> struct sumer<P,0> { float operator()(const float*v) { return .0f; } };
	
	
	// ======================================================
	// All of the following can be specialized for a platform:
	// ======================================================
	template<int P>
	struct implementation {
		
		template<int granularity>
		static inline void copy(float* __restrict__ d, const float* __restrict__ s) {
			union { copy_type<granularity>* v; float* f; } dx; 
			union { const copy_type<granularity>* v; const float* f; } sx; 
			dx.f=d; sx.f=s;
			*dx.v=*sx.v;
		}
		
		static inline void zero_initializer(float* p, int n) {
#ifdef DEBUG
			do { *p = NAN; ++p; } while (--n);
#endif
		}
		
		
		static inline void sincos(float* sc, const float angle) {
			sc[0]=sinf(angle);
			sc[1]=cosf(angle);
		}
		static inline void sincos2(float* __restrict__ sc, const float* __restrict__ angles) {
			sincos(sc,angles[0]);
			sincos(sc+2,angles[1]);
		}
		
		
		template<int count>
		static inline float sum(const float* v) { return sumer<P, count>()(v); }
		
		static inline void v3_copy(float* __restrict__ r, const float* __restrict__ rhs) {
			copy<3>(r, rhs);
		}
	
		// v=op(v)
		static inline void v3_neg(float* v) {
			for (int i=0;i<3;++i) v[i]=-v[i];
		}
		
		// r=op(rhs)
		static inline void v3_neg(float* __restrict__ r, const float* __restrict__ rhs) {
			for (int i=0;i<3;++i) r[i]=-rhs[i];
		}
		
		static inline void v3_sum(float& r, const float* __restrict__ rhs) {
			r=sum<3>(&rhs[0]);
		}
		
		static inline void v3_square(float* __restrict__ r, const float* __restrict__ rhs) {
			for (int i=0;i<3;++i) r[i]=rhs[i]*rhs[i];
		}
		
		// lhs = lhs op rhs(v3)
		static inline void v3_add (float* __restrict__ lhs, const float* __restrict__ rhs) {
			for (int i=0;i<3;++i) lhs[i]+=rhs[i];
		}
		static inline void v3_sub (float* __restrict__ lhs, const float* __restrict__ rhs) {
			for (int i=0;i<3;++i) lhs[i]-=rhs[i];
		}
		static inline void v3_mul (float* __restrict__ lhs, const float* __restrict__ rhs) {
			for (int i=0;i<3;++i) lhs[i]*=rhs[i];
		}
		static inline void v3_div (float* __restrict__ lhs, const float* __restrict__ rhs) {
			for (int i=0;i<3;++i) lhs[i]/=rhs[i];
		}
		
		// lhs = lhs op rhs(scalar)
		static inline void v3_add(float* lhs, const float rhs)  {
			for(int i=0;i<3;++i)  lhs[i]+=rhs;
		}
		static inline void v3_sub(float* lhs, const float rhs)  {
			for(int i=0;i<3;++i)  lhs[i]-=rhs;
		}
		static inline void v3_mul(float* lhs, const float rhs)  {
			for(int i=0;i<3;++i)  lhs[i]*=rhs;
		}
		static inline void v3_div(float* lhs, const float rhs)  {
			v3_mul(lhs,1.f/rhs);
		}
		// r = lhs op rhs(v3)
		static inline void v3_add(float* __restrict__ r, const float* __restrict__ lhs, const float* __restrict__ rhs) {
			for (int i=0;i<3;++i) r[i]=lhs[i]+rhs[i];
		}
		static inline void v3_sub(float* __restrict__ r, const float* __restrict__ lhs, const float* __restrict__ rhs) {
			for (int i=0;i<3;++i) r[i]=lhs[i]-rhs[i];
		}
		static inline void v3_mul(float* __restrict__ r, const float* __restrict__ lhs, const float* __restrict__ rhs) {
			for (int i=0;i<3;++i) r[i]=lhs[i]*rhs[i];
		}
		static inline void v3_div(float* __restrict__ r, const float* __restrict__ lhs, const float* __restrict__ rhs) {
			for (int i=0;i<3;++i) r[i]=lhs[i]/rhs[i];
		}
		// r = lhs op rhs(scalar)
		static inline void v3_add(float* __restrict__ r, const float* __restrict__ lhs, const float rhs) {
			for (int i=0;i<3;++i) r[i]=lhs[i]+rhs;
		}
		static inline void v3_sub(float* __restrict__ r, const float* __restrict__ lhs, const float rhs) {
			for (int i=0;i<3;++i) r[i]=lhs[i]-rhs;
		}
		static inline void v3_mul(float* __restrict__ r, const float* __restrict__ lhs, const float rhs) {
			for (int i=0;i<3;++i) r[i]=lhs[i]*rhs;
		}
		static inline void v3_div(float* __restrict__ r, const float* __restrict__ lhs, const float rhs) {
			v3_mul(r,lhs,1.f/rhs);
		}
		
		static inline void v3_dot(float& r, const float* __restrict__ lhs, const float* __restrict__ rhs) {
			float tmp[3];
			v3_mul(tmp,lhs,rhs);
			v3_sum(r,tmp);
		}
		
		static inline void v3_cross(float* __restrict__ r, const float* __restrict__ lhs, const float* __restrict__ rhs) {
			r[0]=lhs[1]*rhs[2]-lhs[2]*rhs[1];
			r[1]=lhs[2]*rhs[0]-lhs[0]*rhs[2];
			r[2]=lhs[0]*rhs[1]-lhs[1]*rhs[0];
		}
		
		static inline void v3_length2(float& r, const float* rhs) {
			float tmp[3];
			v3_square(tmp,rhs);
			v3_sum(r,tmp);
		}
		
		static inline void v3_normalize(float* r) {
			float tmp;
			v3_length2(tmp,r);
			v3_mul(r,1.f/sqrtf(tmp));
		}
		
		// »»»»»»»»»»»»»» Matrix operations »»»»»»»»»»»»»»»»»»
		static inline void m_copy(float* __restrict__ r, const float* __restrict__ rhs) {
			copy<16>(r, rhs);
		}
		static inline void m_build_identity(float* r) {
			static const float identity[] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
			m_copy(r,identity);
		}
		static inline void m_build_translation(float* __restrict__ r, const float* __restrict__ d) {
			m_build_identity(r);
			r[12]=d[0];
			r[13]=d[1];
			r[14]=d[2];
		}
		static inline void m_build_scale(float* r, const float scale) {
			m_build_identity(r);
			r[ 0]=scale;
			r[ 5]=scale;
			r[10]=scale;
		}
		static inline void m_build_rotation_x(float* r, const float angle) {
			float sc[2];
			m_build_identity(r);
			sincos(sc,angle);
			r[ 5] = sc[1];
			r[ 6] =-sc[0];
			r[ 9] = sc[0];
			r[10] = sc[1];
		}
		static inline void m_build_rotation_y(float* r, const float angle) {
			float sc[2];
			m_build_identity(r);
			sincos(sc,angle);
			r[ 0] = sc[1];
			r[ 2] = sc[0];
			r[ 8] =-sc[0];
			r[10] = sc[1];
		}
		static inline void m_build_rotation_z(float* r, const float angle) {
			float sc[2];
			m_build_identity(r);
			sincos(sc,angle);
			r[ 0] = sc[1];
			r[ 1] =-sc[0];
			r[ 4] = sc[0];
			r[ 5] = sc[1];
		}
		static inline void m_build_rotation(float* __restrict__ r, const float* __restrict__ v, const float angle) {
			struct is_normalized {
				bool operator()(const float* v) {
					float tmp;
					implementation<P>::v3_length2(tmp,v);
					return (fabsf(tmp-1.f)<.05f);
				}
			};
			assert(is_normalized()(v)); // ← will build the actual functor only in Debug
			
			float sc[2];
			sincos(sc,angle);
			
			for (int i=0; i<3; ++i) {
				int j=i<<2;
				r[  j] = v[0]*v[i]*(1-sc[1]);
				r[1+j] = v[1]*v[i]*(1-sc[1]);
				r[2+j] = v[2]*v[i]*(1-sc[1]);
				r[3+j] = 0;
			}
			r[ 0] += sc[1];
			r[ 1] += v[2]*sc[0];
			r[ 2] -= v[1]*sc[0];
			r[ 4] -= v[2]*sc[0];
			r[ 5] += sc[1];
			r[ 6] += v[0]*sc[0];
			r[ 8] += v[1]*sc[0];
			r[ 9] -= v[0]*sc[0];
			r[10] += sc[1];
			static const float last_column[] = { 0,0,0,1 };
			copy<4>(&r[12],last_column);
		}
		
		static inline void m_mul (float* __restrict__ r, const float* __restrict__ lhs, const float* __restrict__ rhs) {
			for (int y=0; y<4; ++y) {
				const int i(y<<2);
				r[i  ] = lhs[i]*rhs[0] + lhs[i+1]*rhs[4] + lhs[i+2]*rhs[ 8] + lhs[i+3]*rhs[12];
				r[i+1] = lhs[i]*rhs[1] + lhs[i+1]*rhs[5] + lhs[i+2]*rhs[ 9] + lhs[i+3]*rhs[13];
				r[i+2] = lhs[i]*rhs[2] + lhs[i+1]*rhs[6] + lhs[i+2]*rhs[10] + lhs[i+3]*rhs[14];
				r[i+3] = lhs[i]*rhs[3] + lhs[i+1]*rhs[7] + lhs[i+2]*rhs[11] + lhs[i+3]*rhs[15];
			}
		}
		
		static inline void m_mul (float* __restrict__ lhs, const float* __restrict__ rhs) {
			float tmp[16];
			m_copy(tmp, lhs);
			m_mul(lhs,tmp,rhs);
		}
		
		static inline void m_inverse_transform(float* __restrict__ r, const float* __restrict__ rhs) {
			float f;
			r[ 0] = rhs[ 0];
			r[ 1] = rhs[ 4];
			r[ 2] = rhs[ 8];
			r[ 3] = 0.f;
			r[ 4] = rhs[ 1];
			r[ 5] = rhs[ 5];
			r[ 6] = rhs[ 9];
			r[ 7] = 0.f;
			r[ 8] = rhs[ 2];
			r[ 9] = rhs[ 6];
			r[10] = rhs[10];
			r[11] = 0.f;
			v3_dot(f,rhs,rhs+12);
			r[12] = -f;
			v3_dot(f,rhs+4,rhs+12);
			r[13] = -f;
			v3_dot(f,rhs+8,rhs+12);
			r[14] = -f;
			r[15] = 1.f;
		}
		
		static inline void m_mul_v3(float* __restrict__ dst,
							 const float* __restrict__ src,
							 const float* __restrict__ m,
							 float w) {
			dst[0] = m[0]*src[0] + m[4]*src[1] + m[ 8]*src[2] + m[12]*w;
			dst[1] = m[1]*src[0] + m[5]*src[1] + m[ 9]*src[2] + m[13]*w;
			dst[2] = m[2]*src[0] + m[6]*src[1] + m[10]*src[2] + m[14]*w;
		}
		
		static inline void m_mul_v3(float* __restrict__ dst,
							 int dstStride,
							 const float* __restrict__ src,
							 int srcStride,
							 const float* __restrict__ m,
							 int length,
							 float w) {
			do {
				m_mul_v3(dst,src,m,w);
				dst+=(dstStride>>2);
				src+=(dstStride>>2);
			} while (--length);
		}
		
	};
}; // lowlevel
}; // geometry

#endif // UUID_3B10B6E46784A0497D0D78647CCA5E2

