// Computes compute an approximation of sqrt(x*x+y*y) for distance
// 5% max relative error, with no sqrt, no multiply & no branch
// Algorithm by Rafael Baptista, made branchless
// Reference: http://www.flipcode.com/articles/article_fastdistance.shtml
long approx_distance(long dx, long dy) {
	const unsigned long xx(std::max(dx,-dx));
	const unsigned long yy(std::max(dy,-dy));
	const unsigned long min(std::min(xx, yy));
	const unsigned long max(std::max(xx, yy));
   // coefficients equivalent to ( 123/128 * max ) and ( 51/128 * min )
	return (((max<<8)+(max<<3)-(max<<4)-(max<<1)+(min<<7)-(min<<5)+(min<<3)-(min<<1))>>8);
}
