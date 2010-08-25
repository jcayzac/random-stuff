#ifdef __APPLE__
#include <OpenGLES/ES1/gl.h>
#else
#include <GLES/gl.h>
#endif
#include "Platform.h"
#include "FileSystem.h"
#include "Model.h"

struct MD2Header {
	int identifier;
	int version;
	int skinWidth;
	int skinHeight;
	int _,__;
	int vertexCount;
	int textureCoordinateCount;
	int triangleCount;
	int ___;
	int frameCount;
	int unusedC;
	int textureCoordinatesOffset;
	int trianglesOffset;
	int framesOffset;
};

typedef float MD2Vector[3];

typedef short MD2TexCoords[2];

struct MD2Vertex {
	unsigned char xyz[3];
	unsigned char lightnormalindex;
};

struct MD2Triangle {
	short vertexIndices[3];
	short textureCoordinateIndices[3];
};

struct MD2Frame {
	float     scale[3];
	float     translate[3];
	char      name[16];
	MD2Vertex firstVertex;
};

#define MD2_MAGIC_NUMBER	844121161
#define MD2_VERSION			8

/// @brief Array of 162 normals encoded as [x*127, y*127, z*127].
static const char normals[] = "\
\xbd\x00\x6c\xc8\x1e\x6e\xdb\x00\x79\xd9\x40\x67\xeb\x21\x79\x00\x00\x7f\x00\x6c\x43\xed\x5b\x57\
\x13\x5b\x57\x00\x43\x6c\x27\x40\x67\x43\x00\x6c\x25\x00\x79\x38\x1e\x6e\x15\x21\x79\xa9\x13\x5b\
\x99\x27\x40\xb5\x36\x57\x94\x43\x00\x92\x38\x1e\xa5\x57\x13\xa9\x4b\x36\xc0\x67\x27\xe2\x6e\x38\
\xca\x57\x4b\xa5\x57\xed\xc0\x67\xd9\xbd\x6c\x00\x00\x6c\xbd\xe2\x6e\xc8\x00\x79\xdb\xdf\x79\xeb\
\x00\x7f\x00\x00\x79\x25\xdf\x79\x15\x1e\x6e\x38\x21\x79\x15\x40\x67\x27\x1e\x6e\xc8\x21\x79\xeb\
\x40\x67\xd9\x6c\x43\x00\x5b\x57\x13\x5b\x57\xed\x43\x6c\x00\x36\x57\x4b\x6e\x38\x1e\x57\x4b\x36\
\x67\x27\x40\x57\x13\x5b\x4b\x36\x57\x79\x25\x00\x7f\x00\x00\x79\x15\x21\x6c\xbd\x00\x79\xdb\x00\
\x6e\xc8\x1e\x79\xeb\x21\x67\xd9\x40\x57\xed\x5b\x6c\x00\x43\x6e\x38\xe2\x67\x27\xc0\x79\x15\xdf\
\x43\x00\x94\x57\x13\xa5\x57\xed\xa5\x6c\x00\xbd\x67\xd9\xc0\x6e\xc8\xe2\x79\xeb\xdf\x13\x5b\xa9\
\x27\x40\x99\x36\x57\xb5\x38\x1e\x92\x4b\x36\xa9\x57\x4b\xca\xed\x5b\xa9\xd9\x40\x99\x00\x43\x94\
\xbd\x00\x94\xc8\x1e\x92\xdb\x00\x87\xeb\x21\x87\x00\x00\x81\x25\x00\x87\x15\x21\x87\xc8\xe2\x92\
\xd9\xc0\x99\xeb\xdf\x87\x00\x94\xbd\xed\xa5\xa9\x13\xa5\xa9\x00\xbd\x94\x27\xc0\x99\x38\xe2\x92\
\x15\xdf\x87\x1e\x92\xc8\x40\x99\xd9\x36\xa9\xb5\x5b\xa9\xed\x57\xb5\xca\x4b\xca\xa9\x00\x87\xdb\
\x00\x81\x00\x21\x87\xeb\x00\x94\x43\x00\x87\x25\x1e\x92\x38\x21\x87\x15\x40\x99\x27\x5b\xa9\x13\
\x43\x94\x00\xe2\x92\xc8\xc0\x99\xd9\xdf\x87\xeb\x94\xbd\x00\xa5\xa9\xed\xa5\xa9\x13\xbd\x94\x00\
\xc0\x99\x27\xe2\x92\x38\xdf\x87\x15\x92\xc8\x1e\x99\xd9\x40\xa9\xb5\x36\xa9\xed\x5b\xc8\xe2\x6e\
\xb5\xca\x57\xd9\xc0\x67\xed\xa5\x57\xca\xa9\x4b\xeb\xdf\x79\x38\xe2\x6e\x15\xdf\x79\x27\xc0\x67\
\x13\xa5\x57\x00\xbd\x6c\x36\xa9\x4b\x4b\xca\x57\x57\xb5\x36\x87\x25\x00\x87\x15\x21\x81\x00\x00\
\x94\x00\x43\x87\xdb\x00\x87\xeb\x21\x92\x38\xe2\x87\x15\xdf\x99\x27\xc0\x92\xc8\xe2\x87\xeb\xdf\
\x99\xd9\xc0\xa9\x13\xa5\xa9\xed\xa5\x94\x00\xbd\xa9\x4b\xca\xb5\x36\xa9\xca\x57\xb5\xca\xa9\xb5\
\xb5\xca\xa9\xa9\xb5\xca";

/// @brief Array of the 21 standard MD2 animations, each encoded as [firstFrame, lastFrame, FPS] 
/// + an extra one with [0,0,1]
static const unsigned char animations[] = "\
\x00\x27\x09\x28\x2e\x0a\x2f\x35\x0a\x36\x39\x07\x3a\x3d\x07\x3e\x41\x07\x42\x47\x07\x48\x53\x07\
\x54\x5e\x07\x5f\x6f\x0a\x70\x7a\x07\x7b\x86\x06\x87\x99\x0a\x9a\x9f\x07\xa0\xa8\x0a\xc4\xac\x07\
\xad\xb1\x05\xb2\xb7\x07\xb8\xbd\x07\xbe\xc5\x07\xc6\xc6\x05\x00\x01";

bool ModelT::init(const std::string& path) {
	std::string realPath(FileSystem::buildPath(FileSystem::RES_DIR, path));
	mFile = ResourceFileT(realPath);
	if (mFile.size()>sizeof(MD2Header)) {
		const MD2Header* header=reinterpret_cast<const MD2Header*>(mFile.address());
		mIsAnimated=(header->frameCount>1);
		// Setup the texture coordinates, as they're constant between frames
		mRenderBuffer.resize(header->triangleCount * 3);
		const MD2TexCoords* textureCoordinates=reinterpret_cast<const MD2TexCoords*>(mFile.address()+header->textureCoordinatesOffset);
		const MD2Triangle* triangles=reinterpret_cast<const MD2Triangle*>(mFile.address()+header->trianglesOffset);
		int i=-1;
		for(int j=0; j<header->triangleCount; ++j) {
			for (int k=0; k<3; ++k) {
				++i;
				const MD2TexCoords& textureCoordinate = textureCoordinates[ triangles->textureCoordinateIndices[k]];
				mRenderBuffer[i].uv[0] = (32767L*textureCoordinate[0]) / header->skinWidth;
				mRenderBuffer[i].uv[1] = (32767L*textureCoordinate[1]) / header->skinHeight;
			}
			++triangles;
		}
		return true;
	}
	return false;
}

void ModelT::update(float elapsedTime) {
	if (!mIsAnimated) {
		if (mUpdatedOnce) return;
		mUpdatedOnce=true;
	}
	
	unsigned char firstFrame=animations[mAnimation*3];
	unsigned char lastFrame=animations[mAnimation*3+1];
	float fps = mFPSScale*animations[mAnimation*3+2];
	mTimeAccumulator+=elapsedTime;
	float lerp = mTimeAccumulator*fps;
	if (lerp>=1) {
		while (lerp>=1) {
			lerp-=1;
			mCurrentFrame=mNextFrame;
			mNextFrame=(++mNextFrame==lastFrame)?firstFrame:mNextFrame;
		}
		mTimeAccumulator=0;
	}
	interpolate(lerp);
}

void ModelT::setAnimation(EAnimation a) {
	if (!mIsAnimated)
		return;
	mAnimation=a;
	mNextFrame=animations[a*3];
}

void ModelT::setFrame(int frame) {
	const MD2Header* header=reinterpret_cast<const MD2Header*>(mFile.address());
	if (frame>=0 && frame<header->frameCount) {
		const MD2Triangle* triangles=reinterpret_cast<const MD2Triangle*>(mFile.address()+header->trianglesOffset);
		long frameSize=sizeof(MD2Frame)+(header->vertexCount-1)*sizeof(MD2Vertex);
		const MD2Frame* a = reinterpret_cast<const MD2Frame*>(mFile.address()+header->framesOffset+frame*frameSize);
		const MD2Vertex* va =(const MD2Vertex*)(&a->firstVertex);
		int i=-1;
		for(int j=0; j<header->triangleCount; ++j) {
			for (int k=0; k<3; ++k) {
				++i;
				const MD2Vertex& md2VertexA = va[ triangles->vertexIndices[k]];
				for (int z=0; z<3; ++z) {
					mRenderBuffer[i].xyz[z] = 65536.f*((a->scale[z] * md2VertexA.xyz[z]) + a->translate[z]);
					mRenderBuffer[i].norm[z] = normals[3*md2VertexA.lightnormalindex+z];
				}
			}
			++triangles;
		}
	}
}

void ModelT::interpolate(float lerp) {
	if (mCurrentFrame==mNextFrame || lerp<=0) setFrame(mCurrentFrame);
	else if (lerp>=1) setFrame(mNextFrame);
	else {
		const MD2Header* header=reinterpret_cast<const MD2Header*>(mFile.address());
		const MD2Triangle* triangles=reinterpret_cast<const MD2Triangle*>(mFile.address()+header->trianglesOffset);
		long frameSize=sizeof(MD2Frame)+(header->vertexCount-1)*sizeof(MD2Vertex);
		const MD2Frame* a = reinterpret_cast<const MD2Frame*>(mFile.address()+header->framesOffset+mCurrentFrame*frameSize);
		const MD2Frame* b = reinterpret_cast<const MD2Frame*>(mFile.address()+header->framesOffset+mNextFrame*frameSize);
		const MD2Vertex* va =(const MD2Vertex*)(&a->firstVertex);
		const MD2Vertex* vb =(const MD2Vertex*)(&b->firstVertex);
	
		
		
		
		
		
		VertexT* vertex = &mRenderBuffer[0];
		long fixedLerp6 = lerp*(1<<6);
		int i=-1;
		for(int j=0; j<header->triangleCount; ++j) {
			for(int k=0; k<3; ++k) {
				++i;
				const MD2Vertex& md2VertexA = va[ triangles->vertexIndices[k]];
				const MD2Vertex& md2VertexB = vb[ triangles->vertexIndices[k]];
				MD2Vector veca,vecb;
				VertexT current, next;
				for (int z=0; z<3; ++z) {
					veca[z]=md2VertexA.xyz[z]*a->scale[z] + a->translate[z];
					vecb[z]=md2VertexB.xyz[z]*b->scale[z] + b->translate[z];
					vertex->xyz[z]=(veca[z] + lerp*(vecb[z]-veca[z]))*65536.f;
					current.norm[z] = normals[3*md2VertexA.lightnormalindex+z];
					next.norm[z] = normals[3*md2VertexB.lightnormalindex+z];
					vertex->norm[z] = ((fixedLerp6 * (next.norm[z] - current.norm[z]))>>6) + current.norm[z];
				}
				++vertex;
			}
			++triangles;
		}
	}
}

void processRenderBuffer(const VertexBufferT& renderBuffer) {
	glVertexPointer( 3, GL_FIXED, sizeof(VertexT), renderBuffer.front().xyz);
	glTexCoordPointer( 2, GL_SHORT, sizeof(VertexT), renderBuffer.front().uv);
	glNormalPointer( GL_BYTE, sizeof(VertexT), renderBuffer.front().norm);
	glDrawArrays( GL_TRIANGLES, 0, renderBuffer.size());
}
