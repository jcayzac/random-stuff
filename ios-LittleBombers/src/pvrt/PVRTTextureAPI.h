/******************************************************************************

 @File         PVRTTextureAPI.h

 @Title        OGLES/PVRTTextureAPI

 @Copyright    Copyright (C)  Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  OGLES texture loading.

******************************************************************************/
#ifndef _PVRTTEXTUREAPI_H_
#define _PVRTTEXTUREAPI_H_

#if defined(__APPLE__)
#import <OpenGLES/ES1/gl.h>
#else
#include <GLES/egl.h>
#endif

#include "PVRTTexture.h"

/****************************************************************************
** Functions
****************************************************************************/

/*!***************************************************************************
 @Function		PVRTTextureLoadFromPointer
 @Input			pointer				Pointer to header-texture's structure
 @Modified		texName				the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader		Pointer to a PVR_Texture_Header struct. Modified to
									contain the header data of the returned texture Ignored if NULL.
 @Input			nLoadFromLevel		Which mipmap level to start loading from (0=all)
 @Input			texPtr				If null, texture follows header, else texture is here.
 @Return		PVR_SUCCESS on success
 @Description	Allows textures to be stored in C header files and loaded in. Can load parts of a
				mipmaped texture (ie skipping the highest detailed levels).
				Sets the texture MIN/MAG filter to GL_LINEAR_MIPMAP_NEAREST/GL_LINEAR
				if mipmaps are present, GL_LINEAR/GL_LINEAR otherwise.
*****************************************************************************/
bool PVRTTextureLoadFromPointer(const PVR_Texture_Header* pointer,
								GLuint *const texName,
								PVR_Texture_Header *psTextureHeader=NULL,
								const int nLoadFromLevel=0,
								const void * const texPtr=0);


/*!***************************************************************************
 @Function			PVRTTextureFormatGetBPP
 @Input				nFormat
 @Input				nType
 @Description		Returns the bits per pixel (BPP) of the format.
*****************************************************************************/
unsigned int PVRTTextureFormatGetBPP(const GLuint nFormat, const GLuint nType);

#endif /* _PVRTTEXTUREAPI_H_ */

/*****************************************************************************
 End of file (PVRTTextureAPI.h)
*****************************************************************************/
