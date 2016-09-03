/*
 *  Object.h
 *  Planar Image Finder
 *
 *  Created by Cayzac Julien on 9/27/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#pragma once

class Object {
public:
	Object(const char* programPath);
	virtual ~Object();
	virtual void draw() { }
protected:
	unsigned int mProgram;
};