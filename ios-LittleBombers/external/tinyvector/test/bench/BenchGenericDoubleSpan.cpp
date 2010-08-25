#define TV_DONT_VECTORIZE
#include "tv/tv"

#include <QTime>
#include <iostream>

// Random double
inline double rand() {
	return (1.0e0*qrand())/qrand();
}

// Return elapsed time
int BenchGenericDoubleSpan() {
	// Data
	TV::tiny::vector<double, 100> v1, v2, v3, v4;
	double a1, a2, a3, a4;
	
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
	a1 = rand();
	a2 = rand();
	a3 = rand();
	a4 = rand();
	
	// Measuring
	t.start();
	
	for (unsigned int i = 0; i < 10000000; ++i)
		v1 = v1 * a1 + v2 * a2 + v3 * a3 + v4 * a4;
	
	elaps = t.elapsed();
	
	return elaps;
}

int main(int argc, char **argv) {
	std::cout << "BenchGenericDoubleSpan: " << BenchGenericDoubleSpan() << "ms" << std::endl;
	return 0;
}