#if !defined(RESOURCE_FILE_T_INCLUDED)
#define RESOURCE_FILE_T_INCLUDED

#include <string>
#include <map>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "utility/Buffer.h"

class ResourceFileT {
private:
	boost::shared_ptr<Buffer> mBuffer;

public:
	ResourceFileT() { }
	~ResourceFileT() { }
	ResourceFileT(const ResourceFileT& o): mBuffer(o.mBuffer) { }
	ResourceFileT(const std::string& name);
	
	boost::shared_ptr<Buffer> buffer() const { return mBuffer; }

	uintptr_t address(size_t offset=0) const {
		return mBuffer?mBuffer->address()+offset:0;
	}
	operator bool() const { return size()!=0; }
	size_t size() const {
		return mBuffer?mBuffer->size():0;
	}
};

#endif // RESOURCE_FILE_T_INCLUDED


