#include <sstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ResourceFile.h"
#include "FileSystem.h"

ResourceFileT::ResourceFileT(const std::string& name) {
	
	static std::map<
		std::string,
		boost::weak_ptr<Buffer>
	> gResourceLibrary;
	
	try {
		std::map<
			std::string,
			boost::weak_ptr<Buffer>
		>::iterator it = gResourceLibrary.find(name);
		if (it!=gResourceLibrary.end()) {
			// Found
			mBuffer = it->second.lock();
			if (mBuffer)
				return;
		}
		// Not found, alloc
		mBuffer.reset(Buffer::createFromReadOnlyFile(name));
		if (mBuffer) {
			gResourceLibrary[name]=mBuffer;
			DEBUG_LOG("[Resource]  Mapped « " << name << " » at 0x" << std::hex << mBuffer->address());
		}
		else DEBUG_LOG ("[Resource] Error: returned pointer for « " << name << " » is null...");
	}
	catch(...) {
		DEBUG_LOG("[Resource] Error while getting « " << name << " »!");
		mBuffer.reset();
	}
}
