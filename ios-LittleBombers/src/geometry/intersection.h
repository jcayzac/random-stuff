#if !defined(UUID_988AA6357A10CEF0D)
#define UUID_988AA6357A10CEF0D

#include "geometry/lowlevel/implementation.h"
#include "geometry/sphere.h"
#include "geometry/cone.h"
#include "geometry/vec3f.h"
#include "utility/tribool.h"

namespace geometry {
	
	
	template<typename A, typename B>
	struct intersect {
		tribool operator()(const A& lhs, const B& rhs) { return false; }
	};
	
	template<>
	struct intersect<sphere,cone> {
		tribool operator()(const sphere& lhs, const cone& rhs) {
			const vec3f delta(lhs.center()-rhs.apex());
			const float distance2(delta.squareLength());
			const float proj(delta.dot(rhs.direction()));
			const float e(rhs.sincos()[1]*sqrtf(distance2 - proj*proj)-proj*rhs.sincos()[0]);
			return (fabsf(e)>lhs.radius())?(e<.0f):maybe;
		}
	};
	
	template<>
	struct intersect<cone,sphere> {
		tribool operator()(const cone& lhs, const sphere& rhs) {
			intersect<sphere,cone> tmp;
			return tmp(rhs,lhs);
		}
	};

	template<>
	struct intersect<sphere,sphere> {
		tribool operator()(const sphere& lhs, const sphere& rhs) {
			const float distance2((lhs.center()-rhs.center()).squareLength()),
			diffRadii((lhs.radius() - rhs.radius()) * (lhs.radius() - rhs.radius())),
			grandRadius((lhs.radius() + rhs.radius()) * (lhs.radius() + rhs.radius()));
			return (distance2<=diffRadii)?true:((distance2>grandRadius)?false:maybe);
		}
	};

	template<>
	struct intersect<sphere,vec3f> {
		tribool operator()(const sphere& lhs, const vec3f& rhs) {
			return ((rhs-lhs.center()).squareLength()<lhs.radius()*lhs.radius())?true:false;
		}
	};

	template<>
	struct intersect<vec3f,sphere> {
		tribool operator()(const vec3f& lhs, const sphere& rhs) {
			intersect<sphere,vec3f> tmp;
			return tmp(rhs,lhs);
		}
	};
	
	
}; // geometry

#endif // UUID_988AA6357A10CEF0D

