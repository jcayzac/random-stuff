/******************************************************************************

 @File         PVRTTextureAPI.cpp

 @Title        OGLES/PVRTTextureAPI

 @Copyright    Copyright (C)  Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  OGLES texture loading.

******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//#include "PVRTContext.h"
#include "PVRTglesExt.h"
#include "PVRTTexture.h"
#include "PVRTTextureAPI.h"
#include "PVRTDecompress.h"
//#include "PVRTFixedPoint.h"
//#include "PVRTMatrix.h"
//#include "PVRTMisc.h"
//#include "PVRTResourceFile.h"

/*****************************************************************************
** Functions
****************************************************************************/


/*!***************************************************************************
 @Function		PVRTTextureTile
 @Modified		pOut		The tiled texture in system memory
 @Input			pIn			The source texture
 @Input			nRepeatCnt	Number of times to repeat the source texture
 @Description	Allocates and fills, in system memory, a texture large enough
 to repeat the source texture specified number of times.
 *****************************************************************************/
void PVRTTextureTile(PVR_Texture_Header**      pOut,
					 const PVR_Texture_Header* const pIn,
					 const int				  nRepeatCnt) {
	unsigned int	nFormat = 0, nType = 0, nBPP, nSize, nElW = 0, nElH = 0;
	PVRTuint8		*pMmSrc, *pMmDst;
	unsigned int		nLevel;
	PVR_Texture_Header	*psTexHeaderNew;
	
	assert(pIn->dwWidth);
	assert(pIn->dwWidth == pIn->dwHeight);
	assert(nRepeatCnt > 1);
	
	switch(pIn->dwpfFlags & PVRTEX_PIXELTYPE)
	{
		case OGL_RGBA_5551:
			nFormat		= GL_UNSIGNED_SHORT_5_5_5_1;
			nType		= GL_RGBA;
			nElW		= 1;
			nElH		= 1;
			break;
		case OGL_RGBA_8888:
			nFormat		= GL_UNSIGNED_BYTE;
			nType		= GL_RGBA;
			nElW		= 1;
			nElH		= 1;
			break;
		case OGL_PVRTC2:
			nFormat		= GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
			nType		= 0;
			nElW		= 8;
			nElH		= 4;
			break;
		case OGL_PVRTC4:
			nFormat		= GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
			nType		= 0;
			nElW		= 4;
			nElH		= 4;
			break;
	}
	
	nBPP = PVRTTextureFormatGetBPP(nFormat, nType);
	nSize = pIn->dwWidth * nRepeatCnt;
	
	psTexHeaderNew	= PVRTTextureCreate(nSize, nSize, nElW, nElH, nBPP, true);
	*psTexHeaderNew	= *pIn;
	pMmDst	= (PVRTuint8*)psTexHeaderNew + sizeof(*psTexHeaderNew);
	pMmSrc	= (PVRTuint8*)pIn + sizeof(*pIn);
	
	for(nLevel = 0; ((unsigned int)1 << nLevel) < nSize; ++nLevel)
	{
		int nBlocksDstW = PVRT_MAX((unsigned int)1, (nSize >> nLevel) / nElW);
		int nBlocksDstH = PVRT_MAX((unsigned int)1, (nSize >> nLevel) / nElH);
		int nBlocksSrcW = PVRT_MAX((unsigned int)1, (pIn->dwWidth >> nLevel) / nElW);
		int nBlocksSrcH = PVRT_MAX((unsigned int)1, (pIn->dwHeight >> nLevel) / nElH);
		int nBlocksS	= nBPP * nElW * nElH / 8;
		
		PVRTTextureLoadTiled(
							 pMmDst,
							 nBlocksDstW,
							 nBlocksDstH,
							 pMmSrc,
							 nBlocksSrcW,
							 nBlocksSrcH,
							 nBlocksS,
							 (pIn->dwpfFlags & PVRTEX_TWIDDLE) ? true : false);
		
		pMmDst += nBlocksDstW * nBlocksDstH * nBlocksS;
		pMmSrc += nBlocksSrcW * nBlocksSrcH * nBlocksS;
	}
	
	psTexHeaderNew->dwWidth = nSize;
	psTexHeaderNew->dwHeight = nSize;
	psTexHeaderNew->dwMipMapCount = nLevel;
	*pOut = psTexHeaderNew;
}

/*!***************************************************************************
 @Function		PVRTTextureLoadFromPointer
 @Input			pointer				Pointer to header-texture's structure
 @Modified		texName				the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader		Pointer to a PVR_Texture_Header struct. Modified to
 contain the header data of the returned texture Ignored if NULL.
 @Input			nLoadFromLevel		Which mipmap level to start loading from (0=all)
 @Input			texPtr				If null, texture follows header, else texture is here.
 @Return		true on success
 @Description	Allows textures to be stored in C header files and loaded in. Can load parts of a
 mipmaped texture (ie skipping the highest detailed levels). In OpenGL Cube Map, each
 texture's up direction is defined as next (view direction, up direction),
 (+x,-y)(-x,-y)(+y,+z)(-y,-z)(+z,-y)(-z,-y).
 Sets the texture MIN/MAG filter to GL_LINEAR_MIPMAP_NEAREST/GL_LINEAR
 if mipmaps are present, GL_LINEAR/GL_LINEAR otherwise.
 *****************************************************************************/
bool PVRTTextureLoadFromPointer(const PVR_Texture_Header* pointer,
								GLuint* const texName,
								PVR_Texture_Header* psTextureHeader,
								const int nLoadFromLevel,
								const void* const texPtr
) {
	PVR_Texture_Header PVRHeaderBE;
	const PVR_Texture_Header* psPVRHeader=pointer;
	PVRTuint32* pData;
	
	// convert header to little endian if required
	if(!PVRTIsLittleEndian())
	{
		memcpy(&PVRHeaderBE, pointer, sizeof(PVR_Texture_Header));
		pData = (PVRTuint32*) &PVRHeaderBE;
		unsigned int ui32HeaderSize = sizeof(PVRHeaderBE) / sizeof(PVRTuint32);
		for(unsigned int i = 0; i < ui32HeaderSize; ++i)
			PVRTByteSwap((PVRTuint8*) &pData[i], sizeof(PVRTuint32));
		psPVRHeader = &PVRHeaderBE;
	}
	
	//  check that this data is cube map data or not.
	assert(!(psPVRHeader->dwpfFlags & PVRTEX_CUBEMAP));
	
	pData = (PVRTuint32*) psPVRHeader;
	unsigned int u32NumSurfs;
	
	// perform checks for old PVR psPVRHeader
	if(psPVRHeader->dwHeaderSize!=sizeof(PVR_Texture_Header))
	{	// Header V1
		assert(psPVRHeader->dwHeaderSize==PVRTEX_V1_HEADER_SIZE);
		u32NumSurfs = 1;
	}
	else
	{	// Header V2
		if(psPVRHeader->dwNumSurfs<1)
		{	// encoded with old version of PVRTexTool before zero numsurfs bug found.
			u32NumSurfs = 1;
		}
		else
		{
			u32NumSurfs = psPVRHeader->dwNumSurfs;
		}
	}
	
	GLuint textureName;
	GLenum textureFormat = 0;
	GLenum textureType = GL_RGB;
	
	bool IsPVRTCSupported = CPVRTglesExt::IsGLExtensionSupported("GL_IMG_texture_compression_pvrtc");
	bool IsBGRA8888Supported  = CPVRTglesExt::IsGLExtensionSupported("GL_IMG_texture_format_BGRA8888");
	
	*texName = 0;	// install warning value
	bool IsCompressedFormatSupported = false, IsCompressedFormat = false;
	/* Only accept untwiddled data UNLESS texture format is PVRTC */
	assert ( !(((psPVRHeader->dwpfFlags & PVRTEX_TWIDDLE) == PVRTEX_TWIDDLE)
			&& ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)!=OGL_PVRTC2)
			&& ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)!=OGL_PVRTC4)) );
	
	PVRTuint16 *pTmpShortData;
	bool bFreeData = false;
	bool bConvertEndianness = false;
	
	switch(psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)
	{
		case OGL_RGBA_4444:
			textureFormat = GL_UNSIGNED_SHORT_4_4_4_4;
			textureType = GL_RGBA;
			bConvertEndianness = !PVRTIsLittleEndian();
			break;
			
		case OGL_RGBA_5551:
			textureFormat = GL_UNSIGNED_SHORT_5_5_5_1;
			textureType = GL_RGBA;
			bConvertEndianness = !PVRTIsLittleEndian();
			break;
			
		case OGL_RGBA_8888:
			textureFormat = GL_UNSIGNED_BYTE;
			textureType = GL_RGBA;
			break;
			
			// New OGL Specific Formats Added
			
		case OGL_RGB_565:
			textureFormat = GL_UNSIGNED_SHORT_5_6_5;
			textureType = GL_RGB;
			bConvertEndianness = !PVRTIsLittleEndian();
			break;
			
		case OGL_RGB_555:
			assert(false);
			break;
			
		case OGL_RGB_888:
			textureFormat = GL_UNSIGNED_BYTE;
			textureType = GL_RGB;
			break;
			
		case OGL_I_8:
			textureFormat = GL_UNSIGNED_BYTE;
			textureType = GL_LUMINANCE;
			break;
			
		case OGL_AI_88:
			textureFormat = GL_UNSIGNED_BYTE;
			textureType = GL_LUMINANCE_ALPHA;
			break;
			
		case MGLPT_PVRTC2:
		case OGL_PVRTC2:
			if(IsPVRTCSupported)
			{
				IsCompressedFormatSupported = IsCompressedFormat = true;
				textureFormat = psPVRHeader->dwAlphaBitMask==0 ? GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG ;	// PVRTC2
			}
			else
			{
				IsCompressedFormatSupported = false;
				IsCompressedFormat = true;
				textureFormat = GL_UNSIGNED_BYTE;
				textureType = GL_RGBA;
			}
			break;
		case MGLPT_PVRTC4:
		case OGL_PVRTC4:
			if(IsPVRTCSupported)
			{
				IsCompressedFormatSupported = IsCompressedFormat = true;
				textureFormat = psPVRHeader->dwAlphaBitMask==0 ? GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG ;	// PVRTC4
			}
			else
			{
				IsCompressedFormatSupported = false;
				IsCompressedFormat = true;
				textureFormat = GL_UNSIGNED_BYTE;
				textureType = GL_RGBA;
			}
			break;
		case OGL_BGRA_8888:
			if(IsBGRA8888Supported)
			{
				textureFormat = GL_UNSIGNED_BYTE;
#ifndef __APPLE__
				textureType   = GL_BGRA;
#else
				textureType = GL_RGBA;
#endif
				break;
			}
			else
			{
				// Unable to load GL_BGRA texture as extension GL_IMG_texture_format_BGRA8888 is unsupported.
				return false;
			}
		default:
			// pixel type not supported.\n");
			return false;
	}
	
	if(bConvertEndianness)
	{
		pData = (PVRTuint32*) malloc(psPVRHeader->dwHeaderSize + psPVRHeader->dwTextureDataSize);
		
		memcpy(pData, pointer, psPVRHeader->dwHeaderSize + psPVRHeader->dwTextureDataSize);
		
		pTmpShortData = (PVRTuint16*) (((PVRTuint8*) pData) + psPVRHeader->dwHeaderSize);
		
		for(unsigned int i = 0; i <  psPVRHeader->dwTextureDataSize / sizeof(PVRTuint16); ++i)
			PVRTByteSwap((PVRTuint8*) &pTmpShortData[i], sizeof(PVRTuint16));
		
		bFreeData = true;
	}
	
	while (glGetError());
	// load the texture up
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);				// Never have row-aligned in psPVRHeaders
	
	glGenTextures(1, &textureName);
	glBindTexture(GL_TEXTURE_2D, textureName);
	GLenum err=glGetError();
	if(err!=GL_NO_ERROR) {
		DEBUG_LOG("[PVRT] Error while loading texture: « 0x" << std::hex << err << " »");
		if(bFreeData)
			free(pData);
		return false;
	}
	
	int nTextureLevelsNeeded = (psPVRHeader->dwpfFlags & PVRTEX_MIPMAP)? psPVRHeader->dwMipMapCount : 0;
	int nSoftwareOffset = (nTextureLevelsNeeded>0)?1:0;
	
	for(unsigned int i=0; i<u32NumSurfs; i++)
	{
		PVRTuint8 *theTexturePtr = (texPtr? (PVRTuint8*)texPtr :  (PVRTuint8*)pData + psPVRHeader->dwHeaderSize) + psPVRHeader->dwTextureDataSize * i;
		PVRTuint8 *theTextureToLoad = 0;
		int		nMIPMapLevel;
		unsigned int		nSizeX= psPVRHeader->dwWidth, nSizeY = psPVRHeader->dwHeight;
		unsigned int		CompressedImageSize = 0;
		
		for(nMIPMapLevel = 0; nMIPMapLevel <= nTextureLevelsNeeded; nSizeX = PVRT_MAX(nSizeX/2, (unsigned int)1), nSizeY = PVRT_MAX(nSizeY/2, (unsigned int)1), nMIPMapLevel++)
		{
			// Do Alpha-swap if needed
			
			theTextureToLoad = theTexturePtr;
			
			// Load the Texture
			/* If the texture is PVRTC then use GLCompressedTexImage2D */
			if(IsCompressedFormat)
			{
				/* Calculate how many bytes this MIP level occupies */
				if ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)==OGL_PVRTC2)
				{
					CompressedImageSize = ( PVRT_MAX(nSizeX, PVRTC2_MIN_TEXWIDTH) * PVRT_MAX(nSizeY, PVRTC2_MIN_TEXHEIGHT) * psPVRHeader->dwBitCount + 7) / 8;
				}
				else
				{// PVRTC4 case
					CompressedImageSize = ( PVRT_MAX(nSizeX, PVRTC4_MIN_TEXWIDTH) * PVRT_MAX(nSizeY, PVRTC4_MIN_TEXHEIGHT) * psPVRHeader->dwBitCount + 7) / 8;
				}
				
				if(IsCompressedFormatSupported)
				{
					if(((signed int)nMIPMapLevel - (signed int)nLoadFromLevel) >= 0)
					{
						/* Load compressed texture data at selected MIP level */
						glCompressedTexImage2D(GL_TEXTURE_2D, nMIPMapLevel-nLoadFromLevel, textureFormat, nSizeX, nSizeY, 0,
											   CompressedImageSize, theTextureToLoad);
					}
				}
				else
				{
					if(((signed int)nMIPMapLevel - (signed int)(nLoadFromLevel+nSoftwareOffset)) >= 0)
					{
						// Convert PVRTC to 32-bit
						PVRTuint8 *u8TempTexture = (PVRTuint8*)malloc(nSizeX*nSizeY*4);
						if ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)==OGL_PVRTC2)
						{
							PVRTDecompressPVRTC(theTextureToLoad, 1, nSizeX, nSizeY, u8TempTexture);
						}
						else
						{// PVRTC4 case
							PVRTDecompressPVRTC(theTextureToLoad, 0, nSizeX, nSizeY, u8TempTexture);
						}
						
						// Upload the texture as 32-bits
						glTexImage2D(GL_TEXTURE_2D,nMIPMapLevel-nLoadFromLevel,GL_RGBA,
									 nSizeX,nSizeY,0, GL_RGBA,GL_UNSIGNED_BYTE,u8TempTexture);
						FREE(u8TempTexture);
					}
				}
			}
			else
			{
				if(((signed int)nMIPMapLevel - (signed int)nLoadFromLevel) >= 0)
				{
					/* Load uncompressed texture data at selected MIP level */
					glTexImage2D(GL_TEXTURE_2D,nMIPMapLevel-nLoadFromLevel,textureType,nSizeX,nSizeY,0, textureType,textureFormat,theTextureToLoad);
				}
			}
			
			
			
			if(glGetError())
			{
				//PVRTErrorOutputDebug("PVRTTextureLoadPartialFromPointer failed: glBindTexture() failed.\n");
				
				if(bFreeData)
					free(pData);
				
				return false;
			}
			
			// offset the texture pointer by one mip-map level
			/* PVRTC case */
			if ( IsCompressedFormat )
			{
				theTexturePtr += CompressedImageSize;
			}
			else
			{
				/* New formula that takes into account bit counts inferior to 8 (e.g. 1 bpp) */
				theTexturePtr += (nSizeX * nSizeY * psPVRHeader->dwBitCount + 7) / 8;
			}
		}
	}
	
	*texName = textureName;
	
	if(psTextureHeader)
	{
		*(PVR_Texture_Header*)psTextureHeader = *psPVRHeader;
		((PVR_Texture_Header*)psTextureHeader)->dwPVR = PVRTEX_IDENTIFIER;
		((PVR_Texture_Header*)psTextureHeader)->dwNumSurfs = u32NumSurfs;
	}
	
	if(!psPVRHeader->dwMipMapCount)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	if(bFreeData)
		free(pData);
	
	return true;
}


/*!***************************************************************************
 @Function			PVRTTextureFormatGetBPP
 @Input				nFormat
 @Input				nType
 @Description		Returns the bits per pixel (BPP) of the format.
*****************************************************************************/
unsigned int PVRTTextureFormatGetBPP(const GLuint nFormat, const GLuint nType)
{
	switch(nFormat)
	{
	case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
	case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
		return 2;
	case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
	case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
		return 4;
	case GL_UNSIGNED_BYTE:
		switch(nType)
		{
		case GL_RGBA:
		case GL_BGRA:
			return 32;
		}
	case GL_UNSIGNED_SHORT_5_5_5_1:
		switch(nType)
		{
		case GL_RGBA:
			return 16;
		}
	}

	return 0xFFFFFFFF;
}


/*****************************************************************************
 End of file (PVRTTextureAPI.cpp)
*****************************************************************************/
