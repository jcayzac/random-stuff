/*
   compile-time and runtime hash functions,
   http://github/jcayzac

   USAGE:

     static_string_hash("some string")
         Produces a compile-time hash constant

     static_wstring_hash(L"some string")
         Produces the exact same constant (strings are character type agnostic)

     static_strlen("some string")
         Produces the compile-time constant 11

     std::string s("some string");
     string_hash(s)
         Produces the same value, but it's computed at runtime (use it with
         non static const strings)

     static_string_hash2(" string", static_string_hash("some"))
         Produces still the same value, as the hash of "some" is used to seed
         the hash of " string".

     std::string s(" string");
     string_hash(s, static_string_hash("some"))
         Produces the same value, except the prefix is computed at compile-time
         while the end result is computed at runtime.

*/

namespace {

template <typename T, int N> char(&static_strlen_helper(T(&_)[N]))[N];
#define static_strlen(x) (sizeof(static_strlen_helper(x))-1)

static inline unsigned int _combining_function(const unsigned int& v, const unsigned int& s) {
	return v + 0x9e3779b9U + (s<<6) + (s>>2);
}

template<typename T, int N, unsigned S=0>
struct _static_string_hash {
	static unsigned int hash(const T* p) {
		return _combining_function(p[N-1], _static_string_hash<T, N-1, S>::hash(p));
	}
};

template<typename T, unsigned int S>
struct _static_string_hash<T,0,S> {
	static unsigned int hash(const T* p) { return S; }
};

template<typename T>
static unsigned int string_hash(T p, unsigned seed=0) {
	unsigned int res(seed);
	unsigned int i(0);
	while(p[i]) {
		res = _combining_function(p[i], res);
		++i;
	}
	return res;
}

#define static_string_hash(x)  _static_string_hash<char,    static_strlen(x)>::hash(x)
#define static_wstring_hash(x) _static_string_hash<wchar_t, static_strlen(x)>::hash(x)

#define static_string_hash2(x, seed) \
_static_string_hash< \
    char, \
    static_strlen(x), \
    seed \
>::hash(x)

#define static_wstring_hash2(x, seed) \
_static_string_hash< \
    wchar_t, \
    static_strlen(x), \
    seed \
>::hash(x)

} // namespace






// test program
#include <iostream>

// If you want to look at the assembly, search for "bla" :)
static const unsigned int bla1(static_string_hash("bla"));
static const char bla_as_fixed_size_array[] = "bla"; // you can't use const char*
static const unsigned int bla2 = static_string_hash(bla_as_fixed_size_array);
unsigned int bla3() { return static_string_hash("bla"); }
unsigned int bla4() { return static_string_hash(bla_as_fixed_size_array); }

int main() {
	std::cout <<
		// This is computed at runtime, and will process each character in turn.
		string_hash("bla") << "\n" <<
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

	// Use a compile-time string hash as a prefix for a runtime one:
	static const unsigned int prefix(static_string_hash("bla"));
	std::cout << string_hash("blo",prefix)
	          << " == "
              << string_hash(L"blablo") << "\n";
	return 0;
}

