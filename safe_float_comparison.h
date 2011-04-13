
#include <algorithm>

static inline bool floats_are_different(const float& a, const float& b) {
	union { float f; int i; } ua, ub;
	ua.f = a;
	ub.f = b;
	// ieee-754 are in lexical order, so ua.i-ub.i should
	// produce "0" for normal numbers if they're equal.
	// I remove the last two bits of the mantissa to account for
	// the lack of precision, and test that the absolute value
	// is not zero.
	return bool((ua.i-ub.i)&0x7FFFFFFC);
}

