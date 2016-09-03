/*
 *  ContourQuad.cpp
 *  Planar Image Finder
 *
 *  Created by Cayzac Julien on 9/30/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "ContourQuad.h"

ContourQuad::ContourQuad()
: Object("ContourQuad.program")
, attribVertex(glGetAttribLocation(mProgram, "position"))
, attribColor(glGetAttribLocation(mProgram, "color"))
{
	updateCorners(0,0,0,0,0,0,0,0);
}

void ContourQuad::draw() {
	static const GLubyte squareColors[] = {
		0,0,0,255,   // 4
		0,0,255,255, // 3
		255,0,0,255, // 1
		0,255,0,255  // 2
	};
	glUseProgram(mProgram);
	glVertexAttribPointer(attribVertex, 2, GL_FLOAT, 0, 0, mVertices);
	glVertexAttribPointer(attribColor, 4, GL_UNSIGNED_BYTE, 1, 0, squareColors);
	glEnableVertexAttribArray(attribVertex);
	glEnableVertexAttribArray(attribColor);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(attribVertex);
	glDisableVertexAttribArray(attribColor);	
}
