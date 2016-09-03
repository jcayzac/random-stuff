/*
 *  VideoQuad.cpp
 *  Planar Image Finder
 *
 *  Created by Cayzac Julien on 9/27/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "VideoQuad.h"
#include <iostream>
#include <assert.h>

VideoQuad::VideoQuad()
: Object("VideoQuad.program")
, texture(0)
, uniformTexture(glGetUniformLocation(mProgram, "texture"))
, attribVertex(glGetAttribLocation(mProgram, "position"))
, attribUV(glGetAttribLocation(mProgram, "uv"))
{
	// Create video texture
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	unsigned int* data = new unsigned int[1024*512];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 512, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (unsigned char*)data);
	delete[] data;
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, 0);	
}

void VideoQuad::draw() {
	static const GLfloat vertices[] = {-1,-1,0,1,-1,0,-1,1,0,1,1,0};
	static const GLfloat uv[] = {640.f/1024.f,480.f/512.f,640.f/1024.f,0,0,480.f/512.f,0,0};
	glUseProgram(mProgram);
	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, 0, 0, vertices);
	glVertexAttribPointer(attribUV, 2, GL_FLOAT, 0, 0, uv);
	glEnableVertexAttribArray(attribVertex);
	glEnableVertexAttribArray(attribUV);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(uniformTexture, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(attribVertex);
	glDisableVertexAttribArray(attribUV);	
}

void VideoQuad::updateTexture(const unsigned char* data) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 640, 480, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
}
