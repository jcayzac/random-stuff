#if !defined(MODEL_H_INCLUDED)
#define MODEL_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include "Types.h"
#include "ResourceFile.h"
#include "Platform.h"

class Object;
class Class;

/*
 
 statique: utiliser un VBO avec GL_STATIC_DRAW
 dynamique: utiliser un VBO avec GL_DYNAMIC_DRAW
 transparent: no depth write, back to front
 opaque:      depth write, front to back
 
 Q=renderQueue
 For each object in frustum:
	R=Q.renderGroup(object.isOpaque?OPAQUE:TRANSPARENT)
	S=R.shaderGroup(object.shader) 
	object.viewMatrix=camera.transformMatrix*object.transformMatrix
	object.sortCrit=(object.viewMatrix*object.centroid).z + object.radius
	S.insert(object, object.isOpaque?more:less)
 
 B=renderBuffer
 For each shader in R=renderGroup(OPAQUE):
	setupShader(shader) // textures, vertex, fragment, etc
    S=R.shaderGroup(shader) 
    For each object in S:
		int rbSize=B.Size;
		B.resize(rbSize+object.vertices)
		transform(M, in=vertices, out=&S[currentVerticesInShaderGroup])
 
 Pour les particle systems, utiliser OES_point_sprite et OES_point_size_array si présentes !
 */


/*
 A full octree itself isn't required, just a value stored per vertex
 describing the location of the vertex in space in relation to other
 vertices. We use a byte per level of the tree with a single bit set for the
 node number - you can do some interesting things with codes created that way
 such as a bitwise AND of vertices to find the octree node which fully
 contains a set of vertices.
 
 It also means the post processed vertices have decent locality and easy
 tight batchability.
 */




/*
 
 
 	Sphere::Sphere()
	{
		radius = -1;
	}

	Sphere::Sphere(const Sphere &S)
	{
		radius = S.radius;
		center = S.center;
	}

	Sphere::Sphere(const Point &O)
	{
		radius = 0 + radiusEpsilon;
		center = O;
	}

	Sphere::Sphere(const Point &O, float R)
	{
		radius = R;
		center = O;
	}

	Sphere::Sphere(const Point &O, const Point &A)
	{
		Vector a = A - O;

		Vector o = 0.5f * a;

		radius = Vector::N(o) + radiusEpsilon;
		center = O + o;
	}

	Sphere::Sphere(const Point &O, const Point &A,  const Point &B)
	{
		Vector a = A - O;
		Vector b = B - O;

		float Denominator = 2.0f * ((a % b) * (a % b));

		Vector o = ((b^2) * ((a % b) % a) +
		            (a^2) * (b % (a % b))) / Denominator;

		radius = Vector::N(o) + radiusEpsilon;
		center = O + o;
	}

	Sphere::Sphere(const Point &O, const Point &A, const Point &B, const Point &C)
	{
		Vector a = A - O;
		Vector b = B - O;
		Vector c = C - O;

		float Denominator = 2.0f * Matrix::det(a.x, a.y, a.z,
		                                       b.x, b.y, b.z,
		                                       c.x, c.y, c.z);

		Vector o = ((c^2) * (a % b) +
		            (b^2) * (c % a) +
		            (a^2) * (b % c)) / Denominator;

		radius = Vector::N(o) + radiusEpsilon;
		center = O + o;
	}
 
 
Sphere Sphere::recurseMini(Point *P[], unsigned int p, unsigned int b = 0) {
	Sphere MB;
	switch(b) {
	case 0:
		MB = Sphere();
		break;
	case 1:
		MB = Sphere(*P[-1]);
		break;
	case 2:
		MB = Sphere(*P[-1], *P[-2]);
		break;
	case 3:
		MB = Sphere(*P[-1], *P[-2], *P[-3]);
		break;
	case 4:
		MB = Sphere(*P[-1], *P[-2], *P[-3], *P[-4]);
		return MB;
	}
	for(unsigned int i = 0; i < p; i++) {
		if(MB.d2(*P[i]) > 0) {  // Signed square distance to sphere
			for(unsigned int j = i; j > 0; j--) {
				Point *T = P[j];
				P[j] = P[j - 1];
				P[j - 1] = T;
			}
			MB = recurseMini(P + 1, i, b + 1);
		}
	}
	return MB;
}

Sphere Sphere::miniBall(Point P[], unsigned int p) {
	Point **L = new Point*[p];
	for(unsigned int i = 0; i < p; i++)
		L[i] = &P[i];
	Sphere MB = recurseMini(L, p);
	delete[] L;
	return MB;
}
 
 */

// Offline:
// — convert vertices to platform format (see Base.h/interleaved_arrays_t)

class Object: public boost::noncopyable {
private:
	Object();
	bool init(const boost::shared_ptr<Class>& clazz);
public:
	static boost::shared_ptr<Object> fromClass(const boost::shared_ptr<Class>& clazz);
	static boost::shared_ptr<Object> fromPath(const std::string& path);
	const boost::shared_ptr<Class>& theClass() const { return mClass; }
private:
	boost::shared_ptr<Class> mClass;
};

class Class: public boost::noncopyable {
private:
	Class();
	bool init(std::string& path);
public:
	static boost::shared_ptr<Class> fromObject(const boost::shared_ptr<Object>& object);
	static boost::shared_ptr<Class> fromPath(const std::string& path);
	
private:
};


extern void processRenderBuffer(const VertexBufferT& renderBuffer);

class ModelT {
public:
	enum EAnimation {
		ANIM_STAND,    ANIM_MOVE,     ANIM_ATTACK,    ANIM_PAIN_1,
		ANIM_PAIN_2,   ANIM_PAIN_3,     ANIM_JUMP,      ANIM_FLIP,
		ANIM_SALUTE,   ANIM_FALL_BACK,  ANIM_WAVE,     ANIM_POINT,
		ANIM_CROUCH_STAND,  ANIM_CROUCH_MOVE,  ANIM_CROUCH_ATTACK,
		ANIM_CROUCH_PAIN, ANIM_CROUCH_DEATH, ANIM_DEATH_FALL_BACK,
		ANIM_DEATH_FALL_FORWARD, ANIM_DEATH_FALL_BACK_SLOW,
		ANIM_BOOM, ANIM_FREEZE
	};
	ModelT()
	: mIsAnimated(false)
	, mUpdatedOnce(false)
	, mAnimation(ANIM_FREEZE)
	, mCurrentFrame(0)
	, mNextFrame(0)
	, mTimeAccumulator(0)
	, mFPSScale(1)
	{ }
	ModelT(const ModelT& o)
	: mIsAnimated(o.mIsAnimated)
	, mUpdatedOnce(false)
	, mAnimation(o.mAnimation)
	, mCurrentFrame(o.mCurrentFrame)
	, mNextFrame(o.mNextFrame)
	, mTimeAccumulator(o.mTimeAccumulator)
	, mFPSScale(o.mFPSScale)
	, mFile(o.mFile)
	{ }
	~ModelT() { }
	bool init(const std::string& path);
	
	void update(float elapsedTime);
	const VertexBufferT& renderBuffer() const { return mRenderBuffer; }
	
	EAnimation animation() const { return mAnimation; }
	void setAnimation(EAnimation);
	
	float fpsScale() const { return mFPSScale; }
	void setFpsScale(float v) { mFPSScale=v; }
private:
	void setFrame(int frame);
	void interpolate(float lerp);
private:
	ResourceFileT mFile;
	VertexBufferT mRenderBuffer;
	
	// Animations
	bool mIsAnimated,mUpdatedOnce;
	EAnimation mAnimation;
	int mCurrentFrame, mNextFrame;
	float mTimeAccumulator, mFPSScale;
};

#endif // MODEL_H_INCLUDED
