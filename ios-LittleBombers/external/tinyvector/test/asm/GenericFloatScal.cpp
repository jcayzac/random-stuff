#define TV_DONT_VECTORIZE
#include "tv/tv"

void swap(float const & a , TV::tiny::vector<float, 12> & x) {
	x = x * a;
}
