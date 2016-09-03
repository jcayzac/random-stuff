/*
 *  SystemPaths.h
 *  Planar Image Finder
 *
 *  Created by Cayzac Julien on 9/24/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#pragma once
#include <string>

class SystemPaths {
public:
	enum PATH_NAME {
		EXECUTABLE,
		BUNDLE,
		APPLICATION_ROOT,
		CACHES,
		DOCUMENTS,
		TEMP
	};
	static const std::string& get(PATH_NAME name);
	static void init(const char* const path);
};
