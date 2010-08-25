struct sphere {
	float o[3];
	float radius;
};

struct cone {
	float apex[3];
	float direction[3];
	float cos, sin;
};

int sphereConeTest(const sphere& s, const cone& c) {
	// Returns:
	//     -1 if sphere is out of the cone
	//      1 if sphere is totally included
	//      0 if it's partially included
	float delta[3];
	float proj=.0f, distance2=.0f;
	for (int i=0; i<3; ++i) {
		delta[i]   = (s.o[i] - c.apex[i]);    // delta = s.o - c.apex
		proj      += delta[i]*c.direction[i]; // proj = dot(delta, c.direction)
		distance2 += delta[i]*delta[i];       // distance2 = dot(delta,delta)
	}
	const float b(proj*c.sin), d(c.cos*sqrtf(distance2 - proj*proj));
	const float e(d-b);
	const int z(((d-b)>.0f)?-1:1);
	return (fabsf(e)>s.radius)?z:0;
}
