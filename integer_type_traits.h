/**
    Cross-platform, cross-compiler, cross-whatever integer limits

    http://github.com/jcayzac
*/

namespace {
	template<typename T>
	struct type_traits {
		enum {
			sign_value  = (T(-1)<T(0)),
			bits_value  = sizeof(T)*8,
			max_value   = (T)(sign_value?(((T)~T(0)) &~ (T(1)<<(bits_value-1))):((T)~T(0))),
			min_value   = (T)(sign_value?(-max_value-1):0)
		};
		static T max() { return T(max_value); }
		static T min() { return T(min_value); }
		static bool is_signed() { return bool(sign_value); }
	};
}


/////////// test program //////////////
#include <iostream>
template<typename T>
void test(const char* name) {
	std::cerr << name << ":\n"
	<< "\tmin  = " << type_traits<T>::min() << "\n"
	<< "\tmax  = " << type_traits<T>::max() << "\n"
	;
}

#define TEST(x) test<x>(#x); test<signed x>("signed " #x); test<unsigned x>("unsigned " #x);

int main() {
	TEST(char);
	TEST(wchar_t);
	TEST(short);
	TEST(int);
	TEST(long);
	TEST(long long);
	return 0;
}

