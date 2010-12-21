
template <typename T, int N> char ( &_ArraySizeHelper( T (&array)[N] ))[N];
#define typed_sizeof(x) sizeof(_ArraySizeHelper(x))

namespace compile_time_hash {
	static inline unsigned int h(const unsigned int& v, const unsigned int& s) {
		return v + 0x9e3779b9U + (s<<6) + (s>>2);
	}

	template<int N>
	struct H {
		static unsigned int exec(const char* p) {
			return h(p[N-1], H<N-1>::exec(p));
		}
	};

	template<>
	struct H<0> {
		static unsigned int exec(const char* p) { return 0; }
	};
}

#define static_string_hash(x) compile_time_hash::H<typed_sizeof(x)>::exec(x)

// test program
#include <iostream>
int main() {
	std::cout <<
		// static_string_hash("bla") will produce the following asm instruction on my machine:
		// movl    $3411065318, %esi
		static_string_hash("bla") << "\n" <<
		// static_string_hash("123456789ABCD"):
		// movl    $2657608752, %esi
		static_string_hash("123456789ABCD") << "\n";
	return 0;
}

