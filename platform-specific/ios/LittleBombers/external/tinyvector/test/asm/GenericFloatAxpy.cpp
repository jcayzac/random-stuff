#define TV_DONT_VECTORIZE
#include "tv/tv"

void swap(float const & a, TV::tiny::vector<float, 12> const & x, TV::tiny::vector<float, 12> & y) {
	y = x * a + y;
}
