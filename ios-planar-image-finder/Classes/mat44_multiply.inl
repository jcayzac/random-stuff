#pragma once

/** Multiply two 4x4 matrices.
  * 
  * @param result An array of 16 floats that will receive the resulting matrix.
  * @param a      An array of 16 floats containing the first matrix.
  * @param b      An array of 16 floats containing the sencod matrix.
  *
  * This function has an undefined behavior if 'result' and any of the two input matrices overlap.
  */
static inline void mat44_multiply(float* __restrict__ result, const float* __restrict__ a, const float* __restrict__ b) {
	#define A(row,col)  a[(col<<2)+row]
	#define B(row,col)  b[(col<<2)+row]
	#define P(row,col)  result[(col<<2)+row]
	for (int i = 0; i < 4; ++i) {
		const float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
		P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
		P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
		P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
		P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
	}
	#undef A
	#undef B
	#undef P
}
