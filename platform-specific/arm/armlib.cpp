#include <math.h>

#define sincosf(s,c,theta)        { s=sinf(theta); c=cosf(theta) }
#define floatn_add( res, a, b, n) for (int i=0; i<n; ++i) { res[i] = a[i]+b[i]; }
#define floatn_sub( res, a, b, n) for (int i=0; i<n; ++i) { res[i] = a[i]-b[i]; }
#define floatn_opposite(res,b, n) for (int i=0; i<n; ++i) { res[i] = -b[i]; }
#define floatn_mul( res, a, b, n) for (int i=0; i<n; ++i) { res[i] = a[i]*b[i]; }
#define floatn_div( res, a, b, n) for (int i=0; i<n; ++i) { res[i] = a[i]/b[i]; }
#define floatn_muls(res, a, b, n) for (int i=0; i<n; ++i) { res[i] = a[i]*b; } // b=scalar!
#define floatn_divs(res, a, b, n) for (int i=0; i<n; ++i) { res[i] = a[i]/b; } // b=scalar!
#define floatn_dot( res, a, b, n) { res=.0f; for (int i=0; i<n; ++i) { res += a[i]*b[i]; }; } // res=scalar!
#define floatn_equal(res,a, b, n) { res=false; for(int i=0; i<n; ++i) { res|=(a[i]!=b[i]); }; res=!res; } // res=bool!
#define floatn_x(   res, a, b, n) for (int i=0; i<n; ++i) { int j=(i+1)%n,k=(i+2)%n; res[i]=a[j]*b[k] - a[k]*b[j]; }
#define floatn_length2(res,a,  n) floatn_dot(res,a,a,n) // res=scalar!
#define floatn_length(res,a,   n) { floatn_length2(res,a,n); res=(float)sqrtf(res); } // res=scalar!
#define floatn_norm(res, a,    n) { float d; floatn_length(d,a,n); d=1.f/d; floatn_muls(res, a, d, n); }
#define floatn_set( res, a,    n) for (int i=0; i<n; ++i) { res[i] = a[i]; }
#define floatn_sets(res, a,    n) for (int i=0; i<n; ++i) { res[i] = a; } // a=scalar!


extern "C" {

// float3
typedef float float3[3];
void float3_default(                 float3& self)                                       { floatn_sets(self, .0f, 3); }
void float3_copy(                    float3& self, const float3& o)                      { floatn_set(self, o, 3); }
void float3_operator_plus(           float3& tmp,  const float3& lhs, const float3& rhs) { floatn_add(tmp, lhs, rhs, 3); }
void float3_operator_plusequal(      float3& self, const float3& rhs)                    { floatn_add(self, self, rhs, 3); }
void float3_operator_minus(          float3& tmp,  const float3& lhs, const float3& rhs) { floatn_sub(tmp, lhs, rhs, 3); }
void float3_operator_unary_minus(    float3& tmp,  const float3& rhs)                    { floatn_opposite(tmp, rhs, 3); }
void float3_operator_minusequal(     float3& self, const float3& rhs)                    { floatn_sub(self, self, rhs, 3); }
void float3_operator_mul(            float3& tmp,  const float3& lhs, const float3& rhs) { floatn_mul(tmp, lhs, rhs, 3); }
void float3_operator_mul_scalar(     float3& tmp,  const float3& lhs, float rhs)         { floatn_muls(tmp, lhs, rhs, 3); }
void float3_operator_mulequal(       float3& self, const float3& rhs)                    { floatn_mul(self, self, rhs, 3); }
void float3_operator_mulequal_scalar(float3& self, float rhs)                            { floatn_muls(self, self, rhs, 3); }
void float3_operator_div(            float3& tmp,  const float3& lhs, const float3& rhs) { floatn_div(tmp, lhs, rhs, 3); }
void float3_operator_div_scalar(     float3& tmp,  const float3& lhs, float rhs)         { floatn_divs(tmp, lhs, rhs, 3); }
void float3_operator_divequal(       float3& self, const float3& rhs)                    { floatn_div(self, self, rhs, 3); }
void float3_operator_divequal_scalar(float3& self, float rhs)                            { floatn_divs(self, self, rhs, 3); }
void float3_operator_equal(          bool& tmp,    const float3& lhs, const float3& rhs) { floatn_equal(tmp, lhs, rhs, 3); }
void float3_dot(                     float& tmp,   const float3& lhs, const float3& rhs) { floatn_dot(tmp, lhs, rhs, 3); }
void float3_normalize(               float3& self)                                       { floatn_norm(self, self, 3); }
void float3_get_normalized(          float3& self, const float3& rhs)                    { floatn_norm(self, rhs, 3); }
void float3_length(                  float& tmp,   const float3& rhs)                    { floatn_length(tmp, rhs, 3); }

// float4
typedef float float4[4];
void float4_default(                 float4& self)                                       { floatn_sets(self, .0f, 4); }
void float4_copy(                    float4& self, const float4& o)                      { floatn_set(self, o, 4); }
void float4_operator_plus(           float4& tmp,  const float4& lhs, const float4& rhs) { floatn_add(tmp, lhs, rhs, 4); }
void float4_operator_plusequal(      float4& self, const float4& rhs)                    { floatn_add(self, self, rhs, 4); }
void float4_operator_minus(          float4& tmp,  const float4& lhs, const float4& rhs) { floatn_sub(tmp, lhs, rhs, 4); }
void float4_operator_unary_minus(    float4& tmp,  const float4& rhs)                    { floatn_opposite(tmp, rhs, 4); }
void float4_operator_minusequal(     float4& self, const float4& rhs)                    { floatn_sub(self, self, rhs, 4); }
void float4_operator_mul(            float4& tmp,  const float4& lhs, const float4& rhs) { floatn_mul(tmp, lhs, rhs, 4); }
void float4_operator_mul_scalar(     float4& tmp,  const float4& lhs, float rhs)         { floatn_muls(tmp, lhs, rhs, 4); }
void float4_operator_mulequal(       float4& self, const float4& rhs)                    { floatn_mul(self, self, rhs, 4); }
void float4_operator_mulequal_scalar(float4& self, float rhs)                            { floatn_muls(self, self, rhs, 4); }
void float4_operator_div(            float4& tmp,  const float4& lhs, const float4& rhs) { floatn_div(tmp, lhs, rhs, 4); }
void float4_operator_div_scalar(     float4& tmp,  const float4& lhs, float rhs)         { floatn_divs(tmp, lhs, rhs, 4); }
void float4_operator_divequal(       float4& self, const float4& rhs)                    { floatn_div(self, self, rhs, 4); }
void float4_operator_divequal_scalar(float4& self, float rhs)                            { floatn_divs(self, self, rhs, 4); }
void float4_operator_equal(          bool& tmp,    const float4& lhs, const float4& rhs) { floatn_equal(tmp, lhs, rhs, 4); }
void float4_dot(                     float& tmp,   const float4& lhs, const float4& rhs) { floatn_dot(tmp, lhs, rhs, 4); }
void float4_normalize(               float4& self)                                       { floatn_norm(self, self, 4); }
void float4_get_normalized(          float4& self, const float4& rhs)                    { floatn_norm(self, rhs, 4); }
void float4_length(                  float& tmp,   const float4& rhs)                    { floatn_length(tmp, rhs, 4); }
void float4_cross(                   float4& tmp,  const float4& lhs, const float4& rhs) { floatn_x(tmp, lhs, rhs, 4); }

// float3+float4
void float3_copy_float4(             float3& self, const float4& o)                      { floatn_set(self, o, 3); }
void float4_copy_float3(             float4& self, const float3& o)                      { floatn_set(self, o, 3); self[3]=.0f; }

// matrix
typedef float matrix[16];
void matrix_default(                 matrix& self)                                       { floatn_sets(self, .0f, 16); }
void matrix_identity(                matrix& self)                                       { for (int i=0;i<16;++i) self[i]=(i%5)?0:1; }
void matrix_translation(             matrix& self, const float x, const float y, const float z) {
	self[ 0]=1.0f; self[ 1]=0.0f; self[ 2]=0.0f; self[ 3]=0.0f;
	self[ 4]=0.0f; self[ 5]=1.0f; self[ 6]=0.0f; self[ 7]=0.0f;
	self[ 8]=0.0f; self[ 9]=0.0f; self[10]=1.0f; self[11]=0.0f;
	self[12]=x;    self[13]=y;    self[14]=z;    self[15]=1.0f;
}
void matrix_scaling(                 matrix& self, const float x, const float y, const float z) {
	self[ 0]=x;    self[ 1]=0.0f; self[ 2]=0.0f; self[ 3]=0.0f;
	self[ 4]=0.0f; self[ 5]=y;    self[ 6]=0.0f; self[ 7]=0.0f;
	self[ 8]=0.0f; self[ 9]=0.0f; self[10]=z;    self[11]=0.0f;
	self[12]=0.0f; self[13]=0.0f; self[14]=0.0f; self[15]=1.0f;
}
void matrix_rotation_axis(            matrix& self, const float3& axis, const float theta) {
	float3 axisn;
	float3_get_normalized(axisn, axis);
	float s = sinf(theta);
	float c = cosf(theta);
	
	self[ 0] = axisn[0] * axisn[0] * (1 - c) + c;
	self[ 4] = axisn[0] * axisn[1] * (1 - c) - (axisn[2] * s);
	self[ 8] = axisn[0] * axisn[2] * (1 - c) + (axisn[1] * s);
	self[12] = 0;
	
	self[ 1] = axisn[1] * axisn[0] * (1 - c) + (axisn[2] * s);
	self[ 5] = axisn[1] * axisn[1] * (1 - c) + c;
	self[ 9] = axisn[1] * axisn[2] * (1 - c) - (axisn[0] * s);
	self[13] = 0;
	
	self[ 2] = axisn[2] * axisn[0] * (1 - c) - (axisn[1] * s);
	self[ 6] = axisn[2] * axisn[1] * (1 - c) + (axisn[0] * s);
	self[10] = axisn[2] * axisn[2] * (1 - c) + c;
	self[14] = 0.0f;
	
	self[ 3] = 0.0f;
	self[ 7] = 0.0f;
	self[11] = 0.0f;
	self[15] = 1.0f;
}
void matrix_rotation_x(              matrix& self, const float theta) {
	float c = cosf(theta);
    float s = sinf(theta);
	self[ 0]=1.0f; self[ 1]=0.0f; self[ 2]=0.0f; self[ 3]=0.0f;
	self[ 4]=0.0f; self[ 5]=c;    self[ 6]=-s;   self[ 7]=0.0f;
	self[ 8]=0.0f; self[ 9]=s;    self[10]=c;    self[11]=0.0f;
	self[12]=0.0f; self[13]=0.0f; self[14]=0.0f; self[15]=1.0f;
}
void matrix_operator_mul_matrix(     matrix& tmp, const matrix& lhs,const matrix& rhs)   {
	for (int y=0; y<4; ++y) {
		const int i=y<<2;
		tmp[i  ] = lhs[i]*rhs[0] + lhs[i+1]*rhs[4] + lhs[i+2]*rhs[ 8] + lhs[i+3]*rhs[12];
		tmp[i+1] = lhs[i]*rhs[1] + lhs[i+1]*rhs[5] + lhs[i+2]*rhs[ 9] + lhs[i+3]*rhs[13];
		tmp[i+2] = lhs[i]*rhs[2] + lhs[i+1]*rhs[6] + lhs[i+2]*rhs[10] + lhs[i+3]*rhs[14];
		tmp[i+3] = lhs[i]*rhs[3] + lhs[i+1]*rhs[7] + lhs[i+2]*rhs[11] + lhs[i+3]*rhs[15];
	}
}
void matrix_operator_mul_float4(     float4& tmp, const float4& lhs, const matrix& rhs)  {
	tmp[0] = rhs[0] * lhs[0] + rhs[4] * lhs[1] + rhs[ 8] * lhs[2] + rhs[12] * lhs[3];
	tmp[1] = rhs[1] * lhs[0] + rhs[5] * lhs[1] + rhs[ 9] * lhs[2] + rhs[13] * lhs[3];
	tmp[2] = rhs[2] * lhs[0] + rhs[6] * lhs[1] + rhs[10] * lhs[2] + rhs[14] * lhs[3];
	tmp[3] = rhs[3] * lhs[0] + rhs[7] * lhs[1] + rhs[11] * lhs[2] + rhs[15] * lhs[3];
}
void matrix_operator_mul_float3(     float3& tmp, const float3& lhs, const matrix& rhs)  {
	tmp[0] = rhs[0] * lhs[0] + rhs[4] * lhs[1] + rhs[ 8] * lhs[2] + rhs[12];
	tmp[1] = rhs[1] * lhs[0] + rhs[5] * lhs[1] + rhs[ 9] * lhs[2] + rhs[13];
	tmp[2] = rhs[2] * lhs[0] + rhs[6] * lhs[1] + rhs[10] * lhs[2] + rhs[14];
}


}; // extern "C"
