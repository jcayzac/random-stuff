#include "tv/tv"

#include <QTime>
#include <iostream>

// Random double
inline double rand() {
	return (1.0e0*qrand())/qrand();
}

// Return elapsed time
int BenchVectorizedDoubleSum() {
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
	
	// Measuring
	t.start();
	
	for (unsigned int i = 0; i < 10000000; ++i)
		v1 = v1 + v2 + v3 + v4;
	
	elaps = t.elapsed();
	
	return elaps;
}

int main(int argc, char **argv) {
	std::cout << "BenchVectorizedDoubleSum: " << BenchVectorizedDoubleSum() << "ms" << std::endl;
	return 0;
}