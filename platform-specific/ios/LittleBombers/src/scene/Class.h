#if !defined(UUID_416DEE979E014B8E927970468D5C5FD3)
#define UUID_416DEE979E014B8E927970468D5C5FD3

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include "geometry/vec3f.h"
#include "geometry/sphere.h"
#include "utility/countable_list.h"

struct Vertex {
	vec3f position;
	vec3f normal;
	short uv[2];
};

struct Packet {
	std::vector<Vertex>         mVertices;
	std::vector<unsigned short> mIndices;
	sphere mBoundingVolume;
};


class Object;
class Class: boost::noncopyable {
private:
	Class(const std::string& path, int flags);
public:
	enum EKind { STATIC, DYNAMIC };
public:
	static boost::shared_ptr<Class> fromFile(const std::string& path, EKind kind=STATIC);
	static boost::shared_ptr<Class> fromObject(const boost::shared_ptr<Object>& object);
	
	int packetCount() const { return mPackets.size(); }
private:
	countable_list<Packet> mPackets;
};

#endif // UUID_416DEE979E014B8E927970468D5C5FD3

