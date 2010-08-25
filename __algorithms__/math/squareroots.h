/// @brief Computes the reciprocal of x (1/x).
/// Empirical error evaluation:
/// * Maximum error: 1.6%
/// * Average error: 0.8%
/// Constraint: If x == 0, result is undefined.
static inline float Reciprocal(float x) {
	union {
		float float_val;
		int   int_val;
	} fi;
	fi.float_val = x;
	fi.int_val   = 2 * 0x3F800000 - fi.int_val;
	return fi.float_val* (2.0f - x*fi.float_val);
}

/// @brief Computes the reciprocal square root of x (1/sqrt(x)).
/// Empirical error evaluation:
/// * Maximum error: 4.1%
/// * Average error: 1.9%
/// Constraint: x must be positive
/// @sa https://playstation2-linux.com/download/p2lsd/fastrsqrt.pdf
static inline float ReciprocalSquareRoot(float x) {
	union {
		float value;
		unsigned word;
	} fw;
	fw.value=x;
	fw.value=(float)fw.word;
	fw.value*=-.5f;
	fw.value+=1597358720.f;
	fw.word=(unsigned)fw.value;
	return fw.value;
}

/// @brief Computes the square root of x.
/// Empirical error evaluation:
/// * Maximum error: 4.3%
/// * Average error: 1.9%
/// Constraint: x must be positive
static inline float SquareRoot(float x) {
	return Reciprocal(ReciprocalSquareRoot(x));
}

static inline unsigned int IntegerSquareRoot(unsigned int x) {
	unsigned int a = 0;
	unsigned int e = 0;
	unsigned int r = 0;
	unsigned int b = 0;
	for (int i=0;i<16;++i) {
		r <<= 2;
		r +=  x>>30;
		x <<= 2;
		a <<= 1;
		e  =  (a<<1) | 1;
		b = (r>=e)?1:0;
		r-=b?e:0;
		a+=b;
	}
	return a;
}

static inline unsigned long long IntegerSquareRoot(unsigned long long x) {
	unsigned long long a = 0;
	unsigned long long e = 0;
	unsigned long long r = 0;
	unsigned long long b = 0;
	for (int i=0;i<32;++i) {
		r <<= 2;
		r +=  x>>62;
		x <<= 2;
		a <<= 1;
		e  =  (a<<1) | 1;
		b = (r>=e)?1:0;
		r-=b?e:0;
		a+=b;
	}
	return a;
}
