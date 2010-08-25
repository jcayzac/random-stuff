#include <math.h>
#include "noisysphere.h"

CNoisySphere::CNoisySphere(unsigned tessel,float amplitude):
CSphere(tessel) {
	srand(1000);
	for (index v=0; v<nb_verts; v++) {
		float v_amp = sqrt(1.0f-amplitude*(((float)(rand()%10000))/10000.0f));
		vertices[v].x *= v_amp;
		vertices[v].y *= v_amp;
		vertices[v].z *= v_amp;
	}
	// normales aux faces
	compute_normals();
	// normales aux vertices
	smoothen(true);
}

CNoisySphere::~CNoisySphere(void) {

}

