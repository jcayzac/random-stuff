#include <map>
#include <sstream>
#include "Class.h"

typedef std::map<std::string,boost::shared_ptr<Class> > library_t; 
typedef library_t::iterator class_iterator_t;
static library_t library;
/*
boost::shared_ptr<Class> Class::fromFile(const std::string& path, int flags) {
	std::ostringstream key_builder;
	key_builder << path << ":" << flags;
	
	std::string key(key_builder.str());
	
	class_iterator_t it = library.find(key);
	if (it!=library.end()) return it->second;
	
	boost::shared_ptr<Class> newClass(new (nothrow) Class(path, flags));
	if (newClass) library[key] = newClass;
	
	return newClass;
}
*/
