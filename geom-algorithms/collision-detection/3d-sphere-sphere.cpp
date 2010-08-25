struct sphere {
	float o[3];
	float radius;
};

int sphereSphereTest(const sphere& a, const sphere& b) {
	// Returns:
	//     -1 if a if out of b
	//      1 if a is totally included in b
	//      0 if it's partially included in b
	float distance2=.0f;
	for (int i=0; i<3; ++i) {
		const float delta(a.o[i] - b.o[i]);
		distance2 += delta*delta;
	}
	const float diffRadii((a.radius - b.radius) * (a.radius - b.radius));
	const float grandRadius((a.radius + b.radius) * (a.radius + b.radius));
	const int z((distance2>grandRadius)?-1:0);
	return (distance2<=diffRadii)?1:z;
}
