#include "tv/tv"

float swap(TV::tiny::vector<float, 12> const & x , TV::tiny::vector<float, 12> const & y) {
	return TV::tiny::dot(x, y);
}
