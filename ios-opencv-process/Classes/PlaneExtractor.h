/*
 *  PlaneExtractor.h
 *  Process
 *
 *  Created by Cayzac Julien on 9/7/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#pragma once

class PlaneExtractor {
public:
	PlaneExtractor();
	~PlaneExtractor();
	void addImage(unsigned int width, unsigned int height, unsigned int bytesPerRow, unsigned int bytesPerPixel, unsigned char* data);
private:
	PlaneExtractor(const PlaneExtractor&);
	PlaneExtractor& operator=(const PlaneExtractor&);
private:
	struct PImp;
	PImp* mPimp;
};
