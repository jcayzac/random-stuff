#define TV_DONT_VECTORIZE
#include "tv/tv"

#include <QTime>
#include <iostream>

// Random float
inline float rand() {
	return (1.0e0F*qrand())/qrand();
}

// Return elapsed time
int BenchGenericFloatSum() {
	// Data
	TV::tiny::vector<float, 100> v1, v2, v3, v4;
	float a1, a2, a3, a4;
	
	// Time
	QTime t;
	int elaps;
	
	// Generate Random data
	for(unsigned int i = 0; i < 100; ++i) {
		v1(i) = rand();
		v2(i) = rand();
		v3(i) = rand();
		v4(i) = rand();
	}
	
	// Measuring
	t.start();
	
	for (unsigned int i = 0; i < 10000000; ++i)
		v1 = v1 + v2 + v3 + v4;
	
	elaps = t.elapsed();
	
	return elaps;
}

int main(int argc, char **argv) {
	std::cout << "BenchGenericFloatSum: " << BenchGenericFloatSum() << "ms" << std::endl;
	return 0;
}