/******************************************************************************

 @File         PVRTglesExt.cpp

 @Title        OGLES/PVRTglesExt

 @Copyright    Copyright (C)  Imagination Technologies Limited.

 @Platform     Independant

 @Description  OGLES extensions.

******************************************************************************/
#include <string.h>
//#include "PVRTContext.h"
#include "PVRTglesExt.h"

/****************************************************************************
** Macros
****************************************************************************/

/****************************************************************************
** Defines
****************************************************************************/

/****************************************************************************
** Structures
****************************************************************************/

/****************************************************************************
** Declarations
****************************************************************************/

/****************************************************************************
** Constants
****************************************************************************/

/****************************************************************************
** Globals
****************************************************************************/

/****************************************************************************
** Local code
****************************************************************************/

/****************************************************************************
** Class: CPVRTglesExt
****************************************************************************/

/*!***************************************************************************
 @Function			LoadExtensions
 @Description		Initialises IMG extensions
*****************************************************************************/
void CPVRTglesExt::LoadExtensions()
{
	glCurrentPaletteMatrixOES = 0;
	glLoadPaletteFromModelViewMatrixOES	= 0;
	glMatrixIndexPointerOES = 0;
	glWeightPointerOES = 0;
	glClipPlanexIMG = 0;
	glClipPlanefIMG = 0;
	glVertexAttribPointerARB = 0;
	glEnableVertexAttribArrayARB = 0;
	glDisableVertexAttribArrayARB = 0;
	glProgramStringARB = 0;
	glBindProgramARB = 0;
	glDeleteProgramsARB = 0;
	glIsProgramARB = 0;
	glGenProgramsARB = 0;
	glVertexAttrib4fvARB = 0;
	glVertexAttrib4xvIMG = 0;
	glProgramLocalParameter4xIMG = 0;
	glProgramLocalParameter4xvIMG = 0;
	glProgramEnvParameter4xIMG = 0;
	glProgramEnvParameter4xvIMG = 0;
	glProgramEnvParameter4fARB = 0;
	glProgramEnvParameter4fvARB = 0;
	glProgramLocalParameter4fARB = 0;
	glProgramLocalParameter4fvARB = 0;
	glDrawTexiOES = 0;
	glDrawTexivOES = 0;
	glDrawTexsOES = 0;
	glDrawTexsvOES = 0;
	glDrawTexxOES = 0;
	glDrawTexxvOES = 0;
	glDrawTexfOES = 0;
	glDrawTexfvOES = 0;
	glGetTexStreamDeviceAttribivIMG = 0;
	glTexBindStreamIMG = 0;
	glGetTexStreamDeviceNameIMG = 0;
	glMultiDrawElementsEXT = 0;
	glMultiDrawArraysEXT = 0;
	glMapBufferOES = 0;
	glUnmapBufferOES = 0;
	glGetBufferPointervOES = 0;
	glIsRenderbufferOES = 0;
	glBindRenderbufferOES = 0;
	glDeleteRenderbuffersOES = 0;
	glGenRenderbuffersOES = 0;
	glRenderbufferStorageOES = 0;
	glGetRenderbufferParameterivOES = 0;
	glIsFramebufferOES = 0;
	glBindFramebufferOES = 0;
	glDeleteFramebuffersOES = 0;
	glGenFramebuffersOES = 0;
	glCheckFramebufferStatusOES = 0;
	glFramebufferTexture2DOES = 0;
	glFramebufferRenderbufferOES = 0;
	glGetFramebufferAttachmentParameterivOES = 0;
	glGenerateMipmapOES = 0;
	glPointSizePointerOES = 0;
		
	const GLubyte *pszGLExtensions;

	/* Retrieve GL extension string */
    pszGLExtensions = glGetString(GL_EXTENSIONS);

#ifndef __APPLE__
	/* GL_OES_matrix_palette */
	if (strstr((char *)pszGLExtensions, "GL_OES_matrix_palette"))
	{
		glCurrentPaletteMatrixOES			= (PFNGLCURRENTPALETTEMATRIXOES)eglGetProcAddress("glCurrentPaletteMatrixOES");
		glLoadPaletteFromModelViewMatrixOES	= (PFNGLLOADPALETTEFROMMODELVIEWMATRIXOES)eglGetProcAddress("glLoadPaletteFromModelViewMatrixOES");
		glMatrixIndexPointerOES				= (PFNGLMATRIXINDEXPOINTEROES)eglGetProcAddress("glMatrixIndexPointerOES");
		glWeightPointerOES					= (PFNGLWEIGHTPOINTEROES)eglGetProcAddress("glWeightPointerOES");
	}
	/* GL_IMG_user_clip_plane */
	if (strstr((char *)pszGLExtensions, "GL_IMG_user_clip_plane"))
	{
		/* glClipPlanexIMG and glClipPlanefIMG */
		glClipPlanexIMG = (PFNGLCLIPPLANEXIMG)eglGetProcAddress("glClipPlanexIMG");
		glClipPlanefIMG = (PFNGLCLIPPLANEFIMG)eglGetProcAddress("glClipPlanefIMG");
	}

	/* GL_IMG_vertex_program */
	if (strstr((char *)pszGLExtensions, "GL_IMG_vertex_program"))
	{
		glVertexAttribPointerARB = (PFNGLVERTEXATTRIBPOINTERARB)eglGetProcAddress("glVertexAttribPointerARB");
		glEnableVertexAttribArrayARB = (PFNGLENABLEVERTEXATTRIBARRAYARB)eglGetProcAddress("glEnableVertexAttribArrayARB");
		glDisableVertexAttribArrayARB = (PFNGLDISABLEVERTEXATTRIBARRAYARB)eglGetProcAddress("glDisableVertexAttribArrayARB");
		glProgramStringARB = (PFNGLPROGRAMSTRINGARB)eglGetProcAddress("glProgramStringARB");
		glBindProgramARB = (PFNGLBINDPROGRAMARB)eglGetProcAddress("glBindProgramARB");
		glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARB)eglGetProcAddress("glDeleteProgramsARB");
		glIsProgramARB = (PFNGLISPROGRAMARB)eglGetProcAddress("glIsProgramARB");
		glGenProgramsARB = (PFNGLGENPROGRAMSARB)eglGetProcAddress("glGenProgramsARB");
		glVertexAttrib4fvARB = (PFNGLVERTEXATTRIB4FVARB)eglGetProcAddress("glVertexAttrib4fvARB");
		glVertexAttrib4xvIMG = (PFNGLVERTEXATTRIB4XVIMG)eglGetProcAddress("glVertexAttrib4xvIMG");
		glProgramLocalParameter4xIMG = (PFNGLPROGRAMLOCALPARAMETER4XIMG)eglGetProcAddress("glProgramLocalParameter4xIMG");
		glProgramLocalParameter4xvIMG = (PFNGLPROGRAMLOCALPARAMETER4XVIMG)eglGetProcAddress("glProgramLocalParameter4xvIMG");
		glProgramEnvParameter4xIMG = (PFNGLPROGRAMENVPARAMETER4XIMG)eglGetProcAddress("glProgramEnvParameter4xIMG");
		glProgramEnvParameter4xvIMG = (PFNGLPROGRAMENVPARAMETER4XVIMG)eglGetProcAddress("glProgramEnvParameter4xvIMG");
		glProgramEnvParameter4fARB = (PFNGLPROGRAMENVPARAMETER4FARB)eglGetProcAddress("glProgramEnvParameter4fARB");
		glProgramEnvParameter4fvARB = (PFNGLPROGRAMENVPARAMETER4FVARB)eglGetProcAddress("glProgramEnvParameter4fvARB");
		glProgramLocalParameter4fARB = (PFNGLPROGRAMLOCALPARAMETER4FARB)eglGetProcAddress("glProgramLocalParameter4fARB");
		glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARB)eglGetProcAddress("glProgramLocalParameter4fvARB");
	}

	/* GL_OES_draw_texture */
	if (strstr((char *)pszGLExtensions, "GL_OES_draw_texture"))
	{
		glDrawTexiOES = (PFNGLDRAWTEXIOES)eglGetProcAddress("glDrawTexiOES");
		glDrawTexivOES = (PFNGLDRAWTEXIVOES)eglGetProcAddress("glDrawTexivOES");
		glDrawTexsOES = (PFNGLDRAWTEXSOES)eglGetProcAddress("glDrawTexsOES");
		glDrawTexsvOES = (PFNGLDRAWTEXSVOES)eglGetProcAddress("glDrawTexsvOES");
		glDrawTexxOES = (PFNGLDRAWTEXXOES)eglGetProcAddress("glDrawTexxOES");
		glDrawTexxvOES = (PFNGLDRAWTEXXVOES)eglGetProcAddress("glDrawTexxvOES");
		glDrawTexfOES = (PFNGLDRAWTEXFOES)eglGetProcAddress("glDrawTexfOES");
		glDrawTexfvOES = (PFNGLDRAWTEXFVOES)eglGetProcAddress("glDrawTexfvOES");
	}

	/* IMG_texture_stream */
	if (strstr((char *)pszGLExtensions, "GL_IMG_texture_stream"))
	{
		glGetTexStreamDeviceAttribivIMG = (PFNGLGETTEXSTREAMDEVICEATTRIBIVIMG)eglGetProcAddress("glGetTexStreamDeviceAttribivIMG");
		glTexBindStreamIMG = (PFNGLTEXBINDSTREAMIMG)eglGetProcAddress("glTexBindStreamIMG");
		glGetTexStreamDeviceNameIMG = (PFNGLGETTEXSTREAMDEVICENAMEIMG)eglGetProcAddress("glGetTexStreamDeviceNameIMG");
	}

	/* GL_EXT_multi_draw_arrays */
	if (strstr((char *)pszGLExtensions, "GL_EXT_multi_draw_arrays"))
	{
		glMultiDrawElementsEXT = (PFNGLMULTIDRAWELEMENTS)eglGetProcAddress("glMultiDrawElementsEXT");
		glMultiDrawArraysEXT = (PFNGLMULTIDRAWARRAYS)eglGetProcAddress("glMultiDrawArraysEXT");
	}

    /* GL_EXT_multi_draw_arrays */
	if (strstr((char *)pszGLExtensions, "GL_OES_mapbuffer"))
	{
        glMapBufferOES = (PFNGLMAPBUFFEROES)eglGetProcAddress("glMapBufferOES");
        glUnmapBufferOES = (PFNGLUNMAPBUFFEROES)eglGetProcAddress("glUnmapBufferOES");
        glGetBufferPointervOES = (PFNGLGETBUFFERPOINTERVOES)eglGetProcAddress("glGetBufferPointervOES");
	}

    /* GL_OES_Framebuffer_object*/
	if (strstr((char *)pszGLExtensions, "GL_OES_framebuffer_object"))
	{
        glIsRenderbufferOES = (PFNGLISRENDERBUFFEROES)eglGetProcAddress("glIsRenderbufferOES") ;
        glBindRenderbufferOES = (PFNGLBINDRENDERBUFFEROES)eglGetProcAddress("glBindRenderbufferOES");
        glDeleteRenderbuffersOES = (PFNGLDELETERENDERBUFFERSOES)eglGetProcAddress("glDeleteRenderbuffersOES");
        glGenRenderbuffersOES = (PFNGLGENRENDERBUFFERSOES)eglGetProcAddress("glGenRenderbuffersOES");
        glRenderbufferStorageOES = (PFNGLRENDERBUFFERSTORAGEOES)eglGetProcAddress("glRenderbufferStorageOES");
        glGetRenderbufferParameterivOES = (PFNGLGETRENDERBUFFERPARAMETERIVOES)eglGetProcAddress("glGetRenderbufferParameterivOES");
        glIsFramebufferOES = (PFNGLISFRAMEBUFFEROES)eglGetProcAddress("glIsFramebufferOES");
        glBindFramebufferOES = (PFNGLBINDFRAMEBUFFEROES)eglGetProcAddress("glBindFramebufferOES");
        glDeleteFramebuffersOES = (PFNGLDELETEFRAMEBUFFERSOES)eglGetProcAddress("glDeleteFramebuffersOES");
        glGenFramebuffersOES = (PFNGLGENFRAMEBUFFERSOES)eglGetProcAddress("glGenFramebuffersOES");
        glCheckFramebufferStatusOES = (PFNGLCHECKFRAMEBUFFERSTATUSOES)eglGetProcAddress("glCheckFramebufferStatusOES");
        glFramebufferTexture2DOES = (PFNGLFRAMEBUFFERTEXTURE2DOES)eglGetProcAddress("glFramebufferTexture2DOES");
        glFramebufferRenderbufferOES = (PFNGLFRAMEBUFFERRENDERBUFFEROES)eglGetProcAddress("glFramebufferRenderbufferOES");
        glGetFramebufferAttachmentParameterivOES = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES)eglGetProcAddress("glGetFramebufferAttachmentParameterivOES");
        glGenerateMipmapOES = (PFNGLGENERATEMIPMAPOES)eglGetProcAddress("glGenerateMipmapOES");
	}

	/* GL_OES_point_size_array */
	if (strstr((char *)pszGLExtensions, "GL_OES_point_size_array"))
	{
		glPointSizePointerOES = (PFNGLPOINTSIZEPOINTEROES)eglGetProcAddress("glPointSizePointerOES");
	}
	
#else
	// the extensions supported on the iPhone are treated as core functions of gl
	// so assign the function pointers in this class appropriately.

	
	/* GL_OES_matrix_palette */
	if (strstr((char *)pszGLExtensions, "GL_OES_matrix_palette"))
	{
		glCurrentPaletteMatrixOES				= ::glCurrentPaletteMatrixOES;
		glLoadPaletteFromModelViewMatrixOES	= ::glLoadPaletteFromModelViewMatrixOES;
		glMatrixIndexPointerOES				= ::glMatrixIndexPointerOES;
		glWeightPointerOES					= ::glWeightPointerOES;
	}

	/* GL_OES_draw_texture */
	if (strstr((char *)pszGLExtensions, "GL_OES_draw_texture"))
	{
		glDrawTexiOES = ::glDrawTexiOES;
		glDrawTexivOES = ::glDrawTexivOES;
		glDrawTexsOES = ::glDrawTexsOES;
		glDrawTexsvOES = ::glDrawTexsvOES;
		glDrawTexxOES = ::glDrawTexxOES;
		glDrawTexxvOES = ::glDrawTexxvOES;
		glDrawTexfOES = ::glDrawTexfOES;
		glDrawTexfvOES = ::glDrawTexfvOES;
	}

	/* GL_EXT_multi_draw_arrays */
	if (strstr((char *)pszGLExtensions, "GL_OES_mapbuffer"))
	{
        glMapBufferOES = ::glMapBufferOES;
        glUnmapBufferOES = ::glUnmapBufferOES;
        glGetBufferPointervOES = ::glGetBufferPointervOES;
	}
	
    /* GL_OES_Framebuffer_object*/
	if (strstr((char *)pszGLExtensions, "GL_OES_framebuffer_object"))
	{
        glIsRenderbufferOES = ::glIsRenderbufferOES;
        glBindRenderbufferOES = ::glBindRenderbufferOES;
        glDeleteRenderbuffersOES = ::glDeleteRenderbuffersOES;
        glGenRenderbuffersOES = ::glGenRenderbuffersOES;
        glRenderbufferStorageOES = ::glRenderbufferStorageOES;
        glGetRenderbufferParameterivOES = ::glGetRenderbufferParameterivOES;
        glIsFramebufferOES = ::glIsFramebufferOES;
        glBindFramebufferOES = ::glBindFramebufferOES;
        glDeleteFramebuffersOES = ::glDeleteFramebuffersOES;
        glGenFramebuffersOES = ::glGenFramebuffersOES;
        glCheckFramebufferStatusOES = ::glCheckFramebufferStatusOES;
        glFramebufferTexture2DOES = ::glFramebufferTexture2DOES;
        glFramebufferRenderbufferOES = ::glFramebufferRenderbufferOES;
        glGetFramebufferAttachmentParameterivOES = ::glGetFramebufferAttachmentParameterivOES;
        glGenerateMipmapOES = ::glGenerateMipmapOES;
	}
	
	/* GL_OES_point_size_array */
	if (strstr((char *)pszGLExtensions, "GL_OES_point_size_array"))
	{
		glPointSizePointerOES = ::glPointSizePointerOES;
	}
	
#endif
}

/*!***********************************************************************
@Function			IsGLExtensionSupported
@Input				extension extension to query for
@Returns			True if the extension is supported
@Description		Queries for support of an extension
*************************************************************************/
bool CPVRTglesExt::IsGLExtensionSupported(const char *extension)
{
	// The recommended technique for querying OpenGL extensions;
	// from http://opengl.org/resources/features/OGLextensions/
    const GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;

    /* Extension names should not have spaces. */
    where = (GLubyte *) strchr(extension, ' ');
    if (where || *extension == '\0')
        return 0;

    extensions = glGetString(GL_EXTENSIONS);

    /* It takes a bit of care to be fool-proof about parsing the
    OpenGL extensions string. Don't be fooled by sub-strings, etc. */
    start = extensions;
    for (;;) {
        where = (GLubyte *) strstr((const char *) start, extension);
        if (!where)
            break;
        terminator = where + strlen(extension);
        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return true;
        start = terminator;
    }
    return false;
}

/*****************************************************************************
 End of file (PVRTglesExt.cpp)
*****************************************************************************/
