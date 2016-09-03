/*
 *  SaveImageAsPng.h
 *  Process
 *
 *  Created by Cayzac Julien on 9/7/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */
#pragma once
#include <string>

struct SaveImageAsPNG {
	enum ColorSpace {
		COLORSPACE_RGB,
		COLORSPACE_GREY
	};
	bool operator()(const std::string& filename,
					const unsigned char* const data,
					const unsigned int width,
					const unsigned int height,
					const unsigned int channels,
					const unsigned int bytesPerRow,
					const ColorSpace colorSpace
	) const;
};
