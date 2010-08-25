#include <assert.h>
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <stdio.h>
#include <vector>

#include "OpenAL.h"
#include "Codec.h"

static const float gALListenerPos[] = {0.f, 0.f, 0.f};
static const float gALListenerOri[] = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}; 
static float gMasterGain = 1.f;
static bool gInitDone = false;

#define AL_ERROR(err) ((const char*)alGetString(err))

static ALvoid alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq) {
	typedef ALvoid AL_APIENTRY (*alBufferDataStaticProcPtr) (const ALint,ALenum,ALvoid*,ALsizei,ALsizei);
	static alBufferDataStaticProcPtr proc = 0;
	if (!proc) proc = (alBufferDataStaticProcPtr) alcGetProcAddress(0,(const ALCchar* const) "alBufferDataStatic");
	assert(proc);
    proc(bid, format, data, size, freq);
}

OpenAL::openal_instance_t::~openal_instance_t() {
	if (context) {
		DEBUG_LOG("[OpenAL] Closing...");
		ALCcontext* proper = (ALCcontext*)context;
		alcMakeContextCurrent(0);
		ALCdevice* device = alcGetContextsDevice(proper);
		ALuint dummy=0;
		alDeleteSources(1, &dummy);
		dummy=0;
		alDeleteBuffers(1, &dummy);
		alcDestroyContext(proper);
		alcCloseDevice(device);
		context=0;
	}
}

OpenAL::openal_instance_t* OpenAL::instance() {
	// ~openal_instance_t() will be called at exit
	static openal_instance_t x;
	return &x;
}

bool OpenAL::makeContextCurrent() {
	if (!gInitDone) if (!init()) return false;
	if (alcMakeContextCurrent((ALCcontext*)(instance()->context)))
		return true;
	DEBUG_LOG("[OpenAL] Failed to make context current");
	return false;
}


bool OpenAL::init() {
	ALCcontext* context = (ALCcontext*)(instance()->context);
	if (context) return true;
	else {
		ALCdevice* device = alcOpenDevice(0);
		if (device) {
			context = alcCreateContext(device, 0);
			if (context) {
				if (alcMakeContextCurrent(context)) {
					alListenerfv(AL_POSITION, gALListenerPos);
					alListenerfv(AL_ORIENTATION, gALListenerOri);
					instance()->context = context;
					gInitDone=true;
					return true;
				}
				alcDestroyContext(context);
			}
			alcCloseDevice(device);
		}
		alGetError();
	}
	return false;
}

uint32_t OpenAL::createBuffer(const std::string& path) {
	if (!gInitDone) if (!init()) return 0;
	
	// Generate buffer
	alGetError();
	ALuint buffer=0;
	alGenBuffers(1, &buffer);
	ALenum e = alGetError();
	if(e != AL_NO_ERROR) {
		DEBUG_LOG("[OpenAL] Error generating buffer for file '" << path << "': « " << AL_ERROR(e) << " »");
		return 0;
	}
	
	// Uncompress audio file
	int size,freq,bits,channels;
	void* data = Codec::decodeAudio(path,size,bits,channels,freq);
	if (!data) {
		DEBUG_LOG("[OpenAL] Error loading file '" << path << "'");
		alDeleteBuffers(1, &buffer);
		return 0;
	}
	
	int format=0;
	if (channels==1) format=(bits==8)?AL_FORMAT_MONO8:AL_FORMAT_MONO16;
	else if (channels==2) format=(bits==8)?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16;
	else {
		DEBUG_LOG("[OpenAL] Unsupported format: " << bits << " bits, " << channels << " channels.");
		free(data);
		data=0;
	}
	
	// Bind data to buffer
	alGetError();
	alBufferDataStaticProc(buffer, format, data, size, freq);
	e = alGetError();
	if (e != AL_NO_ERROR) {
		DEBUG_LOG("[OpenAL] Error binding data to buffer: « " << AL_ERROR(e) << " »");
		free(data);
		alDeleteBuffers(1, &buffer);
		return 0;
	}
	
	return buffer;
}

uint32_t OpenAL::createSource(bool looped) {
	if (!gInitDone) if (!init()) return 0;

	ALenum e;
	alGetError();
	ALuint source=0;
	alGenSources(1, &source);
	e = alGetError();
	if(e != AL_NO_ERROR) {
		DEBUG_LOG("[OpenAL] Error generating source: « " << AL_ERROR(e) << " »");
		return 0;
	}
	if (looped) alSourcei(source, AL_LOOPING, AL_TRUE);
	// alSourcef(source, AL_REFERENCE_DISTANCE, 20.f);
	return source;
}

bool OpenAL::bind(uint32_t buffer, uint32_t source) {
	if (!gInitDone) return false;
	
	ALenum e;
	alGetError();
	alSourcei(source, AL_BUFFER, buffer);
	e = alGetError();
	if(e != AL_NO_ERROR) {
		DEBUG_LOG("[OpenAL] Error binding source: « " << AL_ERROR(e) << " »");
		return false;
	}
	return true;
}

float OpenAL::volume(uint32_t source) {
	if (!gInitDone) .0f;
	float v=.0f;
	alGetError();
	alGetSourcef(source, AL_GAIN, &v);
	ALenum e = alGetError();
	if(e != AL_NO_ERROR) DEBUG_LOG("[OpenAL] Error querying volume: « " << AL_ERROR(e) << " »");
	return v;
}

void OpenAL::setVolume(uint32_t source, float gain) {
	if (!gInitDone) return;
	alSourcef(source, AL_GAIN, gain * gMasterGain);
}

float OpenAL::volume() { return gMasterGain; }
void OpenAL::setVolume(float gain) { gMasterGain=gain; }

void OpenAL::stop(uint32_t source) {
	if (!gInitDone) return;

	ALenum e;
	alGetError();
	alSourceStop(source);
	e = alGetError();
	if(e != AL_NO_ERROR) {
		DEBUG_LOG("[OpenAL] Error stopping source: « " << AL_ERROR(e) << " »");
	}
}

void OpenAL::play(uint32_t source, float gain, bool restart) {
	if (!gInitDone) return;
	
	ALenum e;
	alSourcefv(source, AL_POSITION, gALListenerPos);
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	if ((state == AL_PLAYING) && restart) {
		stop(source);
		alGetSourcei(source, AL_SOURCE_STATE, &state);
	}
	if (state != AL_PLAYING) {
		setVolume(source, gain);
		alGetError();
		alSourcePlay(source);
		e = alGetError();
		if(e != AL_NO_ERROR) DEBUG_LOG("[OpenAL] Error starting source: « " << AL_ERROR(e) << " »");
	}
}

bool OpenAL::isPlaying(uint32_t source) {
	ALint state=0;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);		
}

void OpenAL::releaseSource(uint32_t source) {
	if (!gInitDone) return;

	stop(source);
	bind(0, source);
	ALenum e;
	alGetError();
	alDeleteSources(1, &source);
	e = alGetError();
	if(e != AL_NO_ERROR) DEBUG_LOG("[OpenAL] Error deleting source: « " << AL_ERROR(e) << " »");
}
void OpenAL::releaseBuffer(uint32_t buffer) {
	if (!gInitDone) return;

	alGetError();
	alDeleteBuffers(1, &buffer);
	ALenum e = alGetError();
	if(e != AL_NO_ERROR) DEBUG_LOG("[OpenAL] Error deleting buffer: « " << AL_ERROR(e) << " »");	
}
