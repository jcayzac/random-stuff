#pragma once
#include <sstream>
#include <fstream>
#include <algorithm>

namespace internal {

template<typename T>
static void saveAsGreyPGM(const std::string& filename,
						  const T* data,
						  size_t w,
						  size_t h,
						  size_t stride) {
	std::ofstream pic;
	pic.open(filename.c_str());
	T max=0;
	for (size_t y(0); y<h; ++y) {
		for (unsigned x(0); x<w; ++x) max=std::max(max, data[y*stride + x]);
	}
	if (pic) {
		pic << "P2\n" << w << " " << h << "\n" << (int) max << "\n";
		for (size_t y(0); y<h; ++y) {
			for (size_t x(0); x<w; ++x) {
				pic << (int) data[y*stride + x] << " ";
			}
			pic << "\n";
		}
		pic.close();
	}
}

}

#define saveAsGreyPGM_U8  internal::saveAsGreyPGM<unsigned char>
#define saveAsGreyPGM_S32 internal::saveAsGreyPGM<int>

