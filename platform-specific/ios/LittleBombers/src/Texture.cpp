#include <string.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGLES/ES1/gl.h>
#else
#include <GLES/gl.h>
#endif

#include "PVRTTextureAPI.h"
#include "Texture.h"
#include "ResourceFile.h"
#include "FileSystem.h"

bool TextureT::init(const std::string& path) {
	std::string realPath(FileSystem::buildPath(FileSystem::RES_DIR, path));
	ResourceFileT file(realPath);
	if (file.size()>sizeof(PVR_Texture_Header)) {
		PVR_Texture_Header header;
		return PVRTTextureLoadFromPointer(
			reinterpret_cast<const PVR_Texture_Header*>(file.address()),
			(GLuint* const) &mId,
			&header
		);
	}
	return false;
}

TextureT::~TextureT() {
	glDeleteTextures(1, &mId);
}

void TextureT::bind(int slot) const {
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture( GL_TEXTURE_2D, mId);
}