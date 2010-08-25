float approx_distance(float dx, float dy) {
	// 5% max relative error, with no sqrt, no multiply & no branch
	// Algorithm by Rafael Baptista, made branchless
	const long xx(((long)dx)&0x7fffffff), yy(((long)dy)&0x7fffffff);
	const unsigned long min((xx<yy)?xx:yy), max((xx<yy)?yy:xx);
	return (float) (((max<<8)+(max<<3)-(max<<4)-(max<<1)+(min<<7)-(min<<5)+(min<<3)-(min<<1))>>8);
}
