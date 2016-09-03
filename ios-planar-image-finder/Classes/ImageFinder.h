#pragma once
#include <stdexcept>
#include "accel.h"

class ImageFinder {
public:
	struct point_t {
		float x;
		float y;
		point_t(): x(.0f), y(.0f) { }
		point_t(float _x, float _y): x(_x), y(_y) { }
		point_t(const point_t& o): x(o.x), y(o.y) { }
		point_t& operator=(const point_t& o) { x=o.x; y=o.y; return *this; }
	};
	typedef point_t corners_t[4];
	typedef float transformation_matrix_t[16];
private:
	// ImageFinder is NonCopyable
	ImageFinder(const ImageFinder&);
	ImageFinder& operator=(const ImageFinder&);
public:
	ImageFinder();
	~ImageFinder();
	
	void setMarker(void* data, size_t width, size_t height, size_t bytesPerRow, size_t bytesPerPixel);
	void addVideoFrame(void* data, size_t width, size_t height, size_t bytesPerRow, const transformation_matrix_t& pose);
	bool pollCorners(corners_t& corners, ImageFinder::transformation_matrix_t& pose);

private:
	class PImpl;
	friend class PImpl;
	PImpl* mImp;
};