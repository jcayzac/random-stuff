#import <Foundation/Foundation.h> // NSData
#import <AVFoundation/AVAudioPlayer.h> // AVAudioPlayer

#include "FileSystem.h"
#include "Sound.h"
#include "BackgroundMusic.h"

// Define an object we can pass to [AVAudioPlayer setDelegate:]
@interface MyAudioPlayerDelegate: NSObject<AVAudioPlayerDelegate> {
	BackgroundMusic* object;
	bool notify;
}
@property (nonatomic) bool notify;
@end
@implementation MyAudioPlayerDelegate
@synthesize notify;
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag {
	if (self.notify)
		object->playbackEndedSignal();
}
- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error {
	assert(false);
}
- (id) initWithMusic:(BackgroundMusic*)m {
	if( self = [super init]) {
		object = m;
		notify = true;
	}
	return self;
}
@end

struct BackgroundMusic::opaque_t {
	MyAudioPlayerDelegate* mapd;
	AVAudioPlayer* av;
	bool playing;
	opaque_t(): av(0), mapd(0) { }
	~opaque_t() {
		if (av) [av release];
		if (mapd) [mapd release];
	}
};

bool BackgroundMusic::init(const std::string& path) {
	mOpaque.reset(new (nothrow) opaque_t);
	if (mOpaque) {
		std::string realPath(FileSystem::buildPath(FileSystem::RES_DIR, path));
		opaque_t& o = *mOpaque;
		o.playing=false;
		o.mapd = [[MyAudioPlayerDelegate alloc] initWithMusic:this];
		if (o.mapd) {
			NSData* data = [[NSData alloc] initWithContentsOfMappedFile:[NSString stringWithUTF8String:realPath.c_str()]];
			NSError* error;
			o.av=[[AVAudioPlayer alloc] initWithData:data error:&error];
			if (o.av) {
				[o.av setDelegate:(o.mapd)];
				[o.mapd retain];
				[o.av retain];
				return true;
			}
		}
		mOpaque.reset();
	}
	return false;
}

BackgroundMusic::~BackgroundMusic() {
	if (mOpaque) {
		opaque_t& o = *mOpaque;
		[o.mapd setNotify:false];
	}
	stop(false);
}

void BackgroundMusic::play(int loops, float initialVolume) {
	if (!mOpaque) return;
	opaque_t& o = *mOpaque;
	[o.av setVolume:initialVolume];
	[o.av setNumberOfLoops:loops];
	[o.av play];
	o.playing=true;
}
void BackgroundMusic::stop(bool notifyDelegate) {
	if (!mOpaque) return;
	opaque_t& o = *mOpaque;
	o.playing=false;
	[o.av stop];
}
float BackgroundMusic::volume() const {
	if (!mOpaque) return 0;
	opaque_t& o = *mOpaque;
	return [o.av volume];
}
void BackgroundMusic::setVolume(float value) {
	if (!mOpaque) return;
	opaque_t& o = *mOpaque;
	[o.av setVolume:value];
}

bool BackgroundMusic::isPlaying() const {
	if (!mOpaque) return false;
	opaque_t& o = *mOpaque;
	return o.playing;
}
