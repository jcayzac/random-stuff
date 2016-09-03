/*
 *  ContourQuad.h
 *  Planar Image Finder
 *
 *  Created by Cayzac Julien on 9/30/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#pragma once
#include "Object.h"

class ContourQuad: public Object {
public:
	ContourQuad();
	void draw();
	
	// order: topLeft, topRightm bottomRight, bottomLeft
	inline void updateCorners(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
		mVertices[0] = 1.f - 2.f*(y4/240.f);
		mVertices[1] = 1.f - 2.f*(x4/320.f);
		mVertices[2] = 1.f - 2.f*(y3/240.f);
		mVertices[3] = 1.f - 2.f*(x3/320.f);
		mVertices[4] = 1.f - 2.f*(y1/240.f);
		mVertices[5] = 1.f - 2.f*(x1/320.f);
		mVertices[6] = 1.f - 2.f*(y2/240.f);
		mVertices[7] = 1.f - 2.f*(x2/320.f);
	}
private:
	int attribVertex;
	int attribColor;
	float mVertices[8];
};


