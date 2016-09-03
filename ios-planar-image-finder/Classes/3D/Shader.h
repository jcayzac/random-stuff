/*
 *  Shader.h
 *  Planar Image Finder
 *
 *  Created by Cayzac Julien on 9/24/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */
#include <OpenGLES/ES2/gl.h>

class Shader {
public:
	static bool compile(GLuint& shader, GLenum type, const char* source);
	static bool link(GLuint program);
	static bool compileAndLink(GLuint& program, const char* vsh, const char* fsh);
	static bool validate(GLuint program);
private:
};