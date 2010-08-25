#include <cmath>

#define TV_DONT_VECTORIZE
#include "tv/tv"

float nrm2(TV::tiny::vector<float, 12> const & x , TV::tiny::vector<float, 12> const & y) {
	return std::sqrt(TV::tiny::dot(x, y));
}
