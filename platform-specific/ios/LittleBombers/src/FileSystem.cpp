#include <map>
#include <sstream>
#include <stdlib.h>
#include <sys/time.h>
#include "FileSystem.h"

std::map<FileSystem::EDirectory, std::string> gDirectories;

void FileSystem::set(const std::string& root,
					 const std::string& cache,
					 const std::string& temp,
					 const std::string& app,
					 const std::string& doc,
					 const std::string& res) {
	gDirectories[ROOT_DIR]=root;
	gDirectories[CACHE_DIR]=cache;
	gDirectories[TEMP_DIR]=temp;
	gDirectories[APP_DIR]=app;
	gDirectories[DOC_DIR]=doc;
	gDirectories[RES_DIR]=res;
}

std::string FileSystem::root() {
	return gDirectories[ROOT_DIR];
}
std::string FileSystem::cacheDir() {
	return gDirectories[CACHE_DIR];
}
std::string FileSystem::tempDir() {
	return gDirectories[TEMP_DIR];
}
std::string FileSystem::appDir() {
	return gDirectories[APP_DIR];
}
std::string FileSystem::docDir() {
	return gDirectories[DOC_DIR];
}
std::string FileSystem::resDir() {
	return gDirectories[RES_DIR];
}

std::string FileSystem::buildPath(EDirectory dir, const std::string& path) {
	std::ostringstream tmp;
	tmp << gDirectories[dir] << '/' << path;
	return tmp.str();
}

std::string FileSystem::buildTempName() {
	std::ostringstream tmp;
	tmp << tempDir() << "/";
	
	struct timeval now;
	gettimeofday(&now,0);
	long long nowl = now.tv_sec*1e6+now.tv_usec;

	for (int i=0; i<16; ++i) {
		static const char digits[] = "0123456789ABCDEF";
		tmp << digits[nowl&0xF];
		nowl >>=4;
	}
	tmp << ".tmp";
	return tmp.str();
}

