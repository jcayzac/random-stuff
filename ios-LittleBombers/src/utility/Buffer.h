#if !defined(UUID_003000D0B9C5438098923968EF391FC6)
#define UUID_003000D0B9C5438098923968EF391FC6

#include <string>
#include <boost/noncopyable.hpp>

class Buffer: boost::noncopyable {
public:
	enum EFreeMethod {
		NORMAL_FREE=0,
		MMAP_FREE
	};

	Buffer(uintptr_t _address, size_t _size, EFreeMethod _free=NORMAL_FREE)
	: mAddress(_address), mSize(_size), mFreeMethod(_free)
	{ }

	~Buffer();
	
	static Buffer* createFromReadOnlyFile(const std::string& path, bool prefetch=true);

	uintptr_t address() const { return mAddress; }
	size_t size() const { return mSize; }
private:
	uintptr_t mAddress;
	size_t    mSize;
	int       mFreeMethod;
};



#endif // UUID_003000D0B9C5438098923968EF391FC6

