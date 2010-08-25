#define TV_DONT_VECTORIZE
#include "tv/tv"

#include <QTime>
#include <iostream>

// Random double
inline double rand() {
	return (1.0e0*qrand())/qrand();
}

// Return elapsed time
int BenchGenericDoubleAxpy() {
	// Data
	TV::tiny::vector<double, 100> x, y;
	double a;
	
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
		y = x * a + y;
	
	elaps = t.elapsed();
	
	return elaps;
}

int main(int argc, char **argv) {
	std::cout << "BenchGenericDoubleAxpy: " << BenchGenericDoubleAxpy() << "ms" << std::endl;
	return 0;
}