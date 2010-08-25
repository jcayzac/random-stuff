/// @brief solves the intersection problem for a given segment and a circle.
/// @param Start  Segment's starting point.
/// @param End    Segment's ending point.
/// @param Center Circle's center point.
/// @param Radius Circle's radius.
/// @param P1 [out] First intersection.
/// @param P2 [out] Second intersection.
/// @return true if an intersection was detected, false if no intersection exists.
/// @sa msdk::point2
///
/// @note The segment is specified with a starting point and an ending point because the resulting intersection
/// are implicitely sorted in the same order. I.e. P1 will alway be closest to Start than P2, and P2 will be the intersection
/// that is closest to End.
///
/// Total float operations used:<ul>
/// <li> When intersections exist:                      10 sub, 10 add, 14 mul, 1 div, 2 squareroots</li>
/// <li> When the segment lies too far from the circle:  9 sub,  5 add, 10 mul, 1 div, 1 squareroot</li>
/// <li> When the given circle is off segment's bounds: 10 sub,  6 add, 10 mul, 1 div, 2 squareroots</li>
/// </ul>
inline bool solveCircleSegmentIntersection(const point2& Start, const point2& End, const point2& Center, float Radius, point2& P1, point2& P2) {
	// here, A = Start, B = End, O = Center
	point2 AO(Center-Start); // 2 sub
	point2 BO(Center-End); // 2 sub
	point2 AB(End-Start); // 2 sub
	// Length of segment [AB]:
	float length=sqrtf(AB.dotProduct(AB)); // 1 sqrt, 1 add, 2 mul
	if (length<.00000001f) return false;
	// Unit vector over line (AB):
	point2 U(AB*(1.f/length)); // 1 div, 2 mul
	// Offset of O's projection on line (AB) relative to A:
	float Ioff=AO.dotProduct(U); // 1 add, 2 mul
	// Projection of O on line (AB):
	point2 I(Start+U*Ioff); // 2 mul, 2 add
	// Squared distance between O and line (AB):
	point2 Z(I-Center); // 2 sub
	float Z2 = Z.dotProduct(Z); // 1 add, 2 mul
	// Squared radius of circle:
	float R2 = Radius*Radius; // 1 sub

	// The distance between circle's center and line (AB) is greater than
	// circle's radius => no intersection!
	if (Z2>R2) return false;

	// We search the intersection lying at offset x on line (AB),
	// relatively to I, so that P-O has length R.
	// It's a simple triangle problem, solved using Pythagore law:
	// R² = x² + Z2.
	// Hence, x = squareroot_of( R² - Z2)
	float x = sqrtf(R2-Z2);  // 1 sqrt
	
	// The two intersection of the given circle with line (AB)
	// lie at offset Ioff-x and Ioff+x
	float x1 = Ioff-x; // 1 sub
	float x2 = Ioff+x; // 1 add
	
	// The circle is beyond A: no intersection!
	if (x1<.0f && x2<.0f) return false;
	
	// The circle is beyond B: no intersection!
	if (x1>length && x2>length) return false;
	
	// Clip the intersections to segment [AB], so that even if the segment lies inside
	// the given circle we return valid segment points.
	x1=(x1<.0f)?.0f:x1;
	x2=(x2<.0f)?.0f:x2;
	x1=(x1>length)?length:x1;
	x2=(x2>length)?length:x2;
	
	// Give result points
	P1 = Start + U*x1; // 2 mul, 2 add
	P2 = Start + U*x2; // 2 mul, 2 add
	return true;
}
