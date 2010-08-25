#if !defined(UUID_729B21280BA84CF9A9EC4F896447D7F0)
#define UUID_729B21280BA84CF9A9EC4F896447D7F0


class FreeLookCamera {
public:
	FreeLookCamera();
	FreeLookCamera(const Camera);
	
	
	const vec3f&  position() const  { return mPosition;  }
	const vec3f&  direction() const { return mLookAt;    }
	const matrix& worldView() const { return mWorldView; }
private:
	void recompute() {
		// since it's a freelook camera, 
	}
private:
	vec3f  mPosition;
	vec3f  mDirection;
	matrix mWorldView;
	matrix mTransposedWorldView;
};



#endif // UUID_729B21280BA84CF9A9EC4F896447D7F0

