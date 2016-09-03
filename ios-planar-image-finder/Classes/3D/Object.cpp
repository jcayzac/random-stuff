/*
 *  Object.cpp
 *  Planar Image Finder
 *
 *  Created by Cayzac Julien on 9/27/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include "Object.h"
#include "Shader.h"
#include "SystemPaths.h"

static const char vertex_prefix[] = "[vertex";
static const char fragment_prefix[] = "[fragment";

Object::Object(const char* programPath): mProgram(0) {
	std::string fullPath;
	{
		std::stringstream x;
		x << SystemPaths::get(SystemPaths::BUNDLE) << "/" << programPath;
		fullPath = x.str();
	}
	
	std::ifstream ifs;
	ifs.open(fullPath.c_str());
	if (!ifs) {
		std::stringstream what;
		what << "Can't open \"" << fullPath << "\"!";
		throw std::runtime_error(what.str());
	}
	
	std::stringstream vertex_shader, fragment_shader;
	std::stringstream* currentStream = 0;
	char line[256];
	while(!ifs.eof()) {
		ifs.getline(line, 256);
		if (strncmp(line, vertex_prefix, strlen(vertex_prefix))==0) {
			currentStream=&vertex_shader;
			continue;
		}
		if (strncmp(line, fragment_prefix, strlen(fragment_prefix))==0) {
			currentStream=&fragment_shader;
			continue;
		}
		if (currentStream) {
			std::stringstream& current(*currentStream);
			current << line << "\n";
		}
	}
	
	ifs.close();
	
	if (vertex_shader.str().empty() || fragment_shader.str().empty()) {
		std::stringstream what;
		what << "Empty shader!\n\nVertex Shader: [" << vertex_shader.str() << "]\nFragment Shader: [" << fragment_shader.str() << "]\n";
		throw std::runtime_error(what.str());
	}
	
	if (!Shader::compileAndLink(mProgram, vertex_shader.str().c_str(), fragment_shader.str().c_str())) {
		std::stringstream what;
		what << "Could not compile/link program with\n\nVertex Shader: [" << vertex_shader.str() << "]\nFragment Shader: [" << fragment_shader.str() << "]\n";
		throw std::runtime_error(what.str());
	}
}

Object::~Object() {
	if (mProgram) {
		glDeleteProgram(mProgram);
		mProgram=0;
	}
}
