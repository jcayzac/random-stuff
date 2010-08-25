#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <iostream>
#include <set>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#include <AudioToolbox/AudioToolbox.h>
#endif
#include "Platform.h"

// ★★★★★★★★★★★★★ VIBRATE ★★★★★★★★★★★★

void Platform::vibrate() {
	DEBUG_LOG("★★★ vibrate ★★★");
#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
	AudioServicesPlaySystemSound (kSystemSoundID_Vibrate);
#endif
}

// ★★★★★★★★★★★★★★ TIME ★★★★★★★★★★★★★★

static long long gTimePrev  = -1;
static long long gTimeAccum = 0;

float Platform::timeAsSeconds() {
	struct timeval now;
	gettimeofday(&now,0);
	long long nowl = now.tv_sec*1e6+now.tv_usec;
	if (gTimePrev<0) gTimePrev=nowl;
	gTimeAccum+=nowl-gTimePrev;
	gTimePrev=nowl;
	return 0.000001f*(float)gTimeAccum;
}

float Platform::timeElapsedInSeconds(const float& reference) {
	return timeAsSeconds()-reference;
}

