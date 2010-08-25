#if !defined(SOUND_H_INCLUDED)
#define SOUND_H_INCLUDED
#include <boost/shared_ptr.hpp>
#include <string>

class SoundFX {
public:
	static const uint32_t ALL_INSTANCES = 0xffffffff;
public:
	SoundFX() {}
	~SoundFX();
	SoundFX(const SoundFX& o): mOpaque(o.mOpaque) { }
	
	bool init(const std::string& path, bool async=false);
	operator bool() const;
	
	uint32_t play(bool looping=false, float initialVolume=1.f);
	void stop(uint32_t instance=ALL_INSTANCES);
	float volume(uint32_t instance=ALL_INSTANCES) const;
	void setVolume(float value, uint32_t instance=ALL_INSTANCES);
private:
	struct opaque_t;
	boost::shared_ptr<opaque_t> mOpaque;
};




#endif // SOUND_H_INCLUDED

