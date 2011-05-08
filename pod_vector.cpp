#include <vector>
#include <stdint.h>


// Fast vector-like template.
// T's copy constructor, assignment operator and destructor
// are completely ignored, so this template should only
// be used with POD types
template <typename T>
class fast_vector {
public:
	typedef T*       iterator;
	typedef const T* const_iterator;

	fast_vector() throw()
	: mData(0)
	, mSize(0)
	, mError(false) { }

	fast_vector(const size_t& sz) throw()
	: mData((T*) __builtin_malloc(storage_size_for(sz)))
	, mSize(mData?sz:0)
	, mError(!mData) {
	}

	fast_vector(const fast_vector& o) throw()
	: mData((T*) __builtin_malloc(storage_size_for(o.size())))
	, mSize(mData?o.size():0)
	, mError(!mData) {
		if (mData) __builtin_memcpy(mData, o.mData, mSize*sizeof(T));
	}

	fast_vector& operator=(const fast_vector& o) throw() {
		if (resize(o.size())) {
			#if defined(__ARM_NEON__)
			// copy 64 bytes at a time
			void* __restrict__ dst(mData);
			void* __restrict__ src(o.mData);
			size_t sz(((o.size()*sizeof(T))+63)>>6);
			asm volatile(
				"1:\t"
				"pld [%[src], #0xC0]\n\t"
				"vldm %[src]!,{d0-d7}\n\t"
				"vstm %[dst]!,{d0-d7}\n\t"
				"subs %[sz],%[sz],#0x40\n\t"
				"bge 1b"
				: [dst] "+r" (dst), [src] "+r" (src), [sz] "+r" (sz)
				:
				: "cc", "memory"
			);
			#elif defined(__arm__)
			// copy 32 bytes at a time
			void* __restrict__ dst(mData);
			void* __restrict__ src(o.mData);
			size_t sz(((o.size()*sizeof(T))+31)>>5);
			asm volatile(
				"push {r3-r10}\n\t"
				"1:\t"
				"ldmia %[src]!, {r3-r10}\n\t"
				"stmia %[dst]!, {r3-r10}\n\t"
				"subs %[sz], %[sz], #0x20\n\t"
				"bge 1b\n\t"
				"pop {r3-r10}"
				: [dst] "+r" (dst), [src] "+r" (src), [sz] "+r" (sz)
				:
				: "cc", "memory"
			);
			#else
			__builtin_memcpy(mData,o.mData,o.size()*sizeof(T));
			#endif
		}
		else {
			// mError must be assigned after clear(),
			// as clear() sets it to false
			clear(true);
			mError=true;
		}
		return *this;
	}

	~fast_vector() throw() {
		clear(true);
	}

	void swap(fast_vector& o) throw() {
		std::swap(mData,  o.mData);
		std::swap(mSize,  o.mSize);
		std::swap(mError, o.mError);
	}

	void clear(bool deallocate=false) throw() {
		mSize=0;
		if ((mError || deallocate) && mData) {
			__builtin_free(mData);
		}
		mError=false;
	}

	size_t size() const throw() {
		return mSize;
	}

	bool resize(const size_t& sz) throw() {
		const bool res(reserve(sz));
		mSize=res?sz:mSize;
		return res;
	}

	size_t capacity() const throw() {
		return mError?0:storage_size_for(mSize);
	}

	bool reserve(const size_t& sz) throw() {
		bool res(true);
		if (mError || (storage_size_for(sz) > storage_size_for(mSize))) {
			T* p((T*) __builtin_realloc(mData, storage_size_for(sz)));
			res=(bool)p;
			mData=p?p:mData;
		}
		return res;
	}

	bool push_back(const T& value) throw() {
		const bool res(resize(mSize+1));
		T tmp;
		T& dst(res?back():tmp);
		dst = value;
		return res;
	}

	void pop_back() throw() {
		mSize=mSize?mSize-1:0;
	}

	operator bool() const throw() {
		return !mError;
	}

	// no check for error is done
	operator T*() throw() {
		return mData;
	}

	operator const T*() const throw() {
		return mData;
	}

	T* begin() throw() {
		return mData;
	}

	const T* begin() const throw() {
		return mData;
	}

	T* end() throw() {
		return mData+mSize;
	}

	const T* end() const throw() {
		return mData+mSize;
	}

	T& front() throw() {
		return mData[0];
	}

	const T& front() const throw() {
		return mData[0];
	}

	T& back() throw() {
		return mData[mSize-1];
	}

	const T& back() const throw() {
		return mData[mSize-1];
	}

private:
	// Return enough pages to hold sz objects of type T.
	static size_t storage_size_for(size_t sz) {
		sz*=sizeof(T);
		return ((sz+4095) & ~4095);
	}
private:
	T* __restrict__ mData;
	size_t mSize;
	bool   mError;
};

extern void test0(std::vector<int>& z, std::vector<int>& w) {
	for (int i(0); i<200; ++i) z.push_back(i);
	w=z;
	w[100]=0;
}

extern void test(fast_vector<int>& z, fast_vector<int>& w) {
	for (int i(0); i<200; ++i) z.push_back(i);
	w=z;
	w[100]=0;
}

