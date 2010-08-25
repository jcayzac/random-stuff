#include "Test.h"




void TestKL() {
	KillerMoves km;
	KillerMoveList kml(5);
	
	kml.Consider(0,15);
	kml.Consider(1,1);
	kml.Consider(2,20);
	kml.Consider(3,15);
	kml.Consider(4,15);
	kml.Consider(5,15);
	kml.Consider(2,15);
	kml.Consider(3,15);
	kml.Consider(6,15);
	kml.Consider(0,17);
	kml.Consider(0,1);
	kml.Consider(8,150);

	kml.dump();
}
