#include <OpenAL/alc.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AudioToolbox/ExtendedAudioFile.h>
#include <TargetConditionals.h>
#include "OpenAL.h"
#include "IPod.h"

static void interruptionListener(void* inClientData, UInt32 inInterruptionState) {
	if (inInterruptionState == kAudioSessionBeginInterruption)
		alcMakeContextCurrent(0);
	else if (inInterruptionState == kAudioSessionEndInterruption) {
		AudioSessionSetActive(true);
		if (OpenAL::makeContextCurrent()) {
			// FIXME: restart sounds?
		}
    }
}

bool isIPodPlaying() {
	UInt32 x;
	UInt32 size = sizeof(x);
	OSStatus result = AudioSessionGetProperty(kAudioSessionProperty_OtherAudioIsPlaying, &size, &x);
	if (result) {
		DEBUG_LOG("[IPod] Error querying iPod status: " << (int)result);
		return false;
	}
	return (bool)x;
}

bool initIPod() {
	OSStatus result = AudioSessionInitialize(0, 0, interruptionListener, 0);
	if (result) {
		DEBUG_LOG("[IPod] Error initializing: " << (int)result);
		return false;
	}
	
	// Setting the category in the simulator results in error (-50).
#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
	bool ipod = isIPodPlaying();
	// if the iPod is playing, use the ambient category to mix with it
	// otherwise, use solo ambient to get the hardware for playing the app background track
	UInt32 category = (ipod) ? kAudioSessionCategory_AmbientSound : kAudioSessionCategory_SoloAmbientSound;
	result = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);
	if (result) {
		DEBUG_LOG("[IPod] Error setting audio session category: " << (int)result);
		return false;
	}
	if (ipod)
		DEBUG_LOG("[IPod] Mixing with current music.");
#endif
	
	result = AudioSessionSetActive(true);
	if (result) {
		DEBUG_LOG("[IPod] Error setting audio session active: " << (int)result);
		return false;
	}
	return true;
}
