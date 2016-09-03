/*
 *  SystemPaths.cpp
 *  Planar Image Finder
 *
 *  Created by Cayzac Julien on 9/24/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#include <sstream>
#include "SystemPaths.h"

std::string pathToExecutable;
std::string pathToBundle;
std::string pathToRoot;
std::string pathToCache;
std::string pathToDocuments;
std::string pathToTmp;

const std::string& SystemPaths::get(PATH_NAME name) {
	switch (name) {
		case EXECUTABLE: return pathToExecutable;
		case BUNDLE: return pathToBundle;
		case APPLICATION_ROOT: return pathToRoot;
		case CACHES: return pathToCache;
		case DOCUMENTS: return pathToDocuments;
		case TEMP: return pathToTmp;
	}
	return pathToTmp; // never reached, but gcc complains
}

void SystemPaths::init(const char* const path) {
	pathToExecutable=path;
	pathToBundle = pathToExecutable.substr(0,pathToExecutable.find_last_of("/\\"));
	pathToRoot = pathToBundle.substr(0,pathToBundle.find_last_of("/\\"));
	{
		std::stringstream x;
		x << pathToRoot << "/Library/Caches";
		pathToCache = x.str();
	}
	{
		std::stringstream x;
		x << pathToRoot << "/Documents";
		pathToDocuments = x.str();
	}
	{
		std::stringstream x;
		x << pathToRoot << "/tmp";
		pathToTmp = x.str();
	}
}
