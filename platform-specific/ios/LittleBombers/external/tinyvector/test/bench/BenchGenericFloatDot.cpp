#define TV_DONT_VECTORIZE
#include "tv/tv"

#include <QTime>
#include <iostream>

// Random float
inline float rand() {
	return (1.0e0F*qrand())/qrand();
}

// Return elapsed time
int BenchGenericFloatDot() {
	// Data
	TV::tiny::vector<float, 100> x, y;
	float a, b;
	
	// Time
	QTime t;
	int elaps;
	
	// Generate Random data
	for(unsigned int i = 0; i < 100; ++i) {
		x(i) = rand();
		y(i) = rand();
	}
	a = rand();
	
	// Measuring
	t.start();
	
	for (unsigned int i = 0; i < 10000000; ++i)
		y = TV::tiny::dot(x, y);
	
	elaps = t.elapsed();
	
	return elaps;
}

int main(int argc, char **argv) {
	std::cout << "BenchGenericFloatDot: " << BenchGenericFloatDot() << "ms" << std::endl;
	return 0;
}