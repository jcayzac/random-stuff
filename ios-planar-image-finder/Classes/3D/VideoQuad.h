/*
 *  VideoQuad.h
 *  Planar Image Finder
 *
 *  Created by Cayzac Julien on 9/27/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#pragma once
#include "Object.h"

class VideoQuad: public Object {
public:
	VideoQuad();
	void draw();
	void updateTexture(const unsigned char* data);
private:
	int uniformTexture;
	int attribVertex;
	int attribUV;
	unsigned int texture;
};
