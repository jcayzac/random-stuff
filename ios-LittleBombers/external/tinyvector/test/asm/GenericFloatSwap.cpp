#define TV_DONT_VECTORIZE
#include "tv/tv"

void swap(TV::tiny::vector<float, 12> & x , TV::tiny::vector<float, 12> & y) {
	TV::tiny::vector<float, 12> t(y);
	y = x;
	x = t;
}
