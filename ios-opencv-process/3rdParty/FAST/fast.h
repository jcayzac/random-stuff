#pragma once
#include <map>
#include <vector>

typedef struct { int x, y; } xy;
typedef unsigned char byte;

struct FASTMem {
	enum {
		SLOT_DETECT,
		SLOT_SCORE,
		SLOT_NONMAX_1,
		SLOT_NONMAX_2
	};
	std::map<int, std::vector<xy> > xys;
	std::map<int, std::vector<int> > ints;
	void* setSizeXY(int slot, size_t size_elems) {
		std::vector<xy>& vec = xys[slot];
		if (size_elems>vec.size())
			vec.resize(size_elems);
		return (void*) &vec[0];
	}
	void* setSizeInt(int slot, size_t size_elems) {
		std::vector<int>& vec = ints[slot];
		if (size_elems>vec.size())
			vec.resize(size_elems);
		return (void*) &vec[0];
	}
};
extern FASTMem gFASTMem;

int fast9_corner_score(const byte* p, const int pixel[], int bstart);
xy* fast9_detect(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
int* fast9_score(const byte* i, int stride, xy* corners, int num_corners, int b);
xy* fast9_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
xy* nonmax_suppression(const xy* corners, const int* scores, int num_corners, int* ret_num_nonmax);
