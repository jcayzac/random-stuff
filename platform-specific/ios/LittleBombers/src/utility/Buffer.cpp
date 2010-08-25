#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Buffer.h"

Buffer::~Buffer() {
	if (mFreeMethod&MMAP_FREE) munmap((void*)mAddress, mSize);
	else free((void*)mAddress);
}

Buffer* Buffer::createFromReadOnlyFile(const std::string& path, bool prefetch) {
	Buffer* buffer = 0;
	int fileDescriptor = ::open( path.c_str(), O_RDONLY, 0 );
	if(fileDescriptor>=0) {
		struct stat statInfo;
		if(fstat(fileDescriptor, &statInfo)==0) {
			void* ptr = ::mmap(0,(size_t)statInfo.st_size,PROT_READ,MAP_PRIVATE,fileDescriptor,0);
			if(ptr != MAP_FAILED) {
				uintptr_t address = (uintptr_t) ptr;
				size_t    size    = (size_t)statInfo.st_size;
				buffer = new Buffer(address, size, MMAP_FREE);
				if (prefetch) {
					volatile long zzz = 0;
					for (uintptr_t p=address; p<address+size-sizeof(long); p+=16) {
						zzz^=*((const long* const)p);
					}
				}
			}
		}
		::close(fileDescriptor);
	}
	return buffer;
}



