#include "tv/tv"

#include <QTime>
#include <iostream>

// Random float
inline float rand() {
	return (1.0e0F*qrand())/qrand();
}

// Return elapsed time
int BenchVectorizedFloatAxpy() {
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
		y = y - x * a;
	
	elaps = t.elapsed();
	
	return elaps;
}

int main(int argc, char **argv) {
	std::cout << "BenchVectorizedFloatAxpy: " << BenchVectorizedFloatAxpy() << "ms" << std::endl;
	return 0;
}