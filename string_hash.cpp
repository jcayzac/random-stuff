#include <iostream>
#include <string>
#include <boost/functional/hash.hpp>

#define declare_string(name_, value_)								\
struct name_ {														\
	static const std::string& name() {								\
		static std::string x(#name_);								\
		return x;													\
	}																\
	static const std::string& value() {								\
		static std::string x(value_);								\
		return x;													\
	}																\
	static std::size_t hash() {										\
		static std::size_t x(boost::hash<std::string>()(value()));	\
		return x;													\
	}																\
}

declare_string(a, "abcdef");
declare_string(b, "std::size_t _hash(boost::hash<std::string>()(string");

int main() {
	std::cout <<
		" name = [" << a::name() << "]\n" <<
		" hash = [" << a::hash() << "]\n" <<
		"value = [" << a::value() << "]\n";
	std::cout <<
		" name = [" << b::name() << "]\n" <<
		" hash = [" << b::hash() << "]\n" <<
		"value = [" << b::value() << "]\n";
	return 0;
}
