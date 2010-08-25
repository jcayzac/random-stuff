#include <set>
#include "utility/Buffer.h"
#include "codecs/Codec.h"
#include "FileSystem.h"
#include "Loader.h"
#include "Sound.h"
#include "OpenAL.h"

struct SoundFX::opaque_t {
	uint32_t buffer;
	std::set<uint32_t> instances;
	opaque_t(): buffer(0) { }
	void clear() {
		if (!instances.empty()) {
			std::set<uint32_t>::iterator it=instances.begin();
			while(it!=instances.end()) {
				uint32_t source = *it; ++it;
				OpenAL::releaseSource(source);
			}
			instances.clear();
		}
		if (buffer) {
			OpenAL::releaseBuffer(buffer);
			buffer=0;
		}
	}
	bool add(uint32_t source) {
		try {
			instances.insert(instances.empty()?instances.begin():--instances.end(), source);
		}
		catch(...) {
			return false;
		}
		return true;
	}
	~opaque_t() { clear(); }
};

static bool doInit(uint32_t& buffer, const std::string& p) {
	buffer = OpenAL::createBuffer(FileSystem::buildPath(FileSystem::RES_DIR, p));
	return (bool)(buffer);
}

bool SoundFX::init(const std::string& p, bool async) {
	boost::shared_ptr<Loader> loader = Loader::instance();
	if (!loader) return false;
	
	mOpaque.reset(new (nothrow) opaque_t);
	if (!mOpaque) return false;
	opaque_t& o = *mOpaque;
	if (async) return loader->enqueue(boost::bind(doInit, boost::ref(o.buffer), boost::ref(p)));
	else return doInit(o.buffer, p);
}

	
SoundFX::operator bool() const {
	if (!mOpaque) return false;
	opaque_t& o = *mOpaque;
	return o.buffer;
}

SoundFX::~SoundFX() {
	stop();
}

uint32_t SoundFX::play(bool looping, float initialVolume) {
	assert(mOpaque);
	opaque_t& o = *mOpaque;
	if (!o.buffer) return 0;
	// Find an instance who is not currently playing
	std::set<uint32_t>::iterator it=o.instances.begin();
	while(it!=o.instances.end()) {
		uint32_t source = *it; ++it;
		if (!OpenAL::isPlaying(source)) {
			OpenAL::play(source, initialVolume);
			return source;
		}
	}
	
	// Else, create a new one
	uint32_t source = OpenAL::createSource(looping);
	if (source) {
		if (o.add(source)) {
			if (OpenAL::bind(o.buffer, source)) {
				OpenAL::play(source, initialVolume);
				return source;
			}
			o.instances.erase(source);
		}
		OpenAL::releaseSource(source);
	}
	return 0;
}
void SoundFX::stop(uint32_t instance) {
	if (!instance) return;
	if (!mOpaque) return;
	opaque_t& o = *mOpaque;
	if (instance==ALL_INSTANCES) o.clear();
	else OpenAL::stop(instance);
}
float SoundFX::volume(uint32_t instance) const {
	if (!instance) return .0f;
	if (!mOpaque) return 0;
	if (instance==ALL_INSTANCES) return OpenAL::volume();
	return OpenAL::volume(instance);
}
void SoundFX::setVolume(float value, uint32_t instance) {
	if (!instance) return;
	if (!mOpaque) return;
	if (instance==ALL_INSTANCES) OpenAL::setVolume(value);
	OpenAL::setVolume(instance, value);
}


