/*
 *  SaveImageAsPng.mm
 *  Process
 *
 *  Created by Cayzac Julien on 9/7/10.
 *  Copyright 2010 -. All rights reserved.
 *
 */

#include "SaveImageAsPng.h"
#import <CoreMedia/CoreMedia.h>

bool SaveImageAsPNG::operator()(const std::string& filename,
								const unsigned char* const data,
								const unsigned int width,
								const unsigned int height,
								const unsigned int channels,
								const unsigned int bytesPerRow,
								const SaveImageAsPNG::ColorSpace colorSpace
								) const {
	CGColorSpaceRef cs = (colorSpace==COLORSPACE_RGB)?CGColorSpaceCreateDeviceRGB():CGColorSpaceCreateDeviceGray();
	
	NSData *tmp = [NSData dataWithBytes:data length:height*bytesPerRow];
	CGDataProviderRef provider = CGDataProviderCreateWithCFData((CFDataRef)tmp);
	CGImageRef imageRef = CGImageCreate(width, height,8,8*channels, bytesPerRow, cs, kCGImageAlphaNone|kCGBitmapByteOrderDefault,provider, NULL, false, kCGRenderingIntentDefault);
	UIImage *ret = [UIImage imageWithCGImage:imageRef];
	CGImageRelease(imageRef);
	CGDataProviderRelease(provider);
	CGColorSpaceRelease(cs);
	NSData* png = UIImagePNGRepresentation(ret);
	[ret release];
	[tmp release];
	NSString* s = [[NSString alloc] initWithUTF8String:filename.c_str()];
	[png writeToFile:s atomically:YES];
	[s release];
	[png release];
	return true;
}