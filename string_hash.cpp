/*
   compile-time hash functions,
   http://github/jcayzac
*/

template <typename T, int N> char ( &_ArraySizeHelper( T (&array)[N] ))[N];
#define array_sizeof(x) sizeof(_ArraySizeHelper(x))
static inline unsigned int h(const unsigned int& v, const unsigned int& s) {
	return v + 0x9e3779b9U + (s<<6) + (s>>2);
}

namespace compile_time_hash {
	template<typename T, int N>
	struct H {
		static unsigned int exec(const T* p) {
			return h(p[N-1], H<T, N-1>::exec(p));
		}
	};

	template<typename T>
	struct H<T,0> {
		static unsigned int exec(const T* p) { return 0; }
	};
}

template<typename T>
struct runtime_hash {
	static unsigned int exec(const T* p, unsigned int N) {
		unsigned int res(0);
		for (unsigned int i(0); i<N; ++i) {
			res = h(p[i], res);
		}
		return res;
	}
};

#define static_string_hash(x)  compile_time_hash::H<char, array_sizeof(x)-1>::exec(x)
#define static_wstring_hash(x) compile_time_hash::H<wchar_t, array_sizeof(x)-1>::exec(x)

// test program
#include <iostream>

unsigned int test1() { return static_string_hash("bla"); }

int main() {
	std::cout <<
		// This is computed at runtime, and will process each character in turn.
		runtime_hash<char>::exec("bla",3) << "\n" <<
		// static_string_hash("bla") will produce the following asm instruction on my machine:
		//     movl    $3411065318, %esi
		// and on iPhone it will store the value in static storage:
		//           ldr r0, L12
		//           …
		//     L12:  .long -883901978
		// (signed -883901978 and unsigned 3411065318 are the same 32-bit constant)
		static_string_hash("bla") << "\n" <<
		// static_string_hash("123456789ABCD"):
		//     movl    $2657608752, %esi
		// and on iPhone it will store the value in static storage:
		//           ldr r0, L12
		//           …
		//     L12:  .long -1637358544
		// (signed -1637358544 and unsigned 2657608752 are the same 32-bit constant)
		static_string_hash("123456789ABCD") << "\n" <<
		// Same string, but with wide chars (produces the same result)
		static_wstring_hash(L"123456789ABCD") << "\n";
	return 0;
}

