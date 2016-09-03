/*
 *  Shader.cpp
 *  Planar Image Finder
 *
 *  Created by Cayzac Julien on 9/24/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#include <iostream>
#include "Shader.h"

bool Shader::compile(GLuint& shader, GLenum type, const char* source) {
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);

//#if defined(DEBUG)
	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar *log = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, &logLength, log);
		std::cerr << "Shader compile log:\n" << (const char*)log << "\n";
		delete[] log;
	}
//#endif
	GLint ok;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (!ok) glDeleteShader(shader);
    return (bool)ok;
}

bool Shader::link(GLuint program) {
	glLinkProgram(program);
	
#if defined(DEBUG)
	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar *log = new GLchar[logLength];
		glGetProgramInfoLog(program, logLength, &logLength, log);
		std::cerr << "Program link log:\n" << log << "\n";
		delete[] log;
	}
#endif
	
	GLint ok;
	glGetProgramiv(program, GL_LINK_STATUS, &ok);
	return (bool)ok;
}

bool Shader::compileAndLink(GLuint& program, const char* vsh, const char* fsh) {
	GLuint vertShader, fragShader;
	if (!compile(vertShader, GL_VERTEX_SHADER, vsh)) {
		return false;
	}
	if (!compile(fragShader, GL_FRAGMENT_SHADER, fsh)) {
		glDeleteShader(vertShader);
		return false;
	}
	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	
	if (!link(program)) {
		if (vertShader)	{ glDeleteShader(vertShader); vertShader=0; }
		if (fragShader)	{ glDeleteShader(fragShader); fragShader=0; }
		if (program)    { glDeleteProgram(program); program=0; }
		return false;
	}
	
	if (vertShader) { glDeleteShader(vertShader); vertShader=0; }
	if (fragShader) { glDeleteShader(fragShader); fragShader=0; }
	return true;
}

bool Shader::validate(GLuint program) {
	glValidateProgram(program);
	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar *log = new GLchar[logLength];
		glGetProgramInfoLog(program, logLength, &logLength, log);
		std::cerr << "Program validate log:\n" << log << "\n";
		delete[] log;
	}
	GLint ok;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &ok);
	return (bool)ok;
}
