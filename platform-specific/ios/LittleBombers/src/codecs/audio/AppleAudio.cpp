#ifdef __APPLE__

/*
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>
#include "AppleAudio.h"
#include "FileSystem.h"
#include <sstream>
static std::string getFileURL(const std::string& path) {
	if (path.find("file://")==0) return path;
	if (path.empty()) return path;
	
	// changes ' ' to '+'
	// could be more efficient, using std::count and a functor
	std::string escaped(path);
	size_t found=escaped.find(' ');
	while (found!=escaped.npos)
		found=escaped.replace(found,1,"%20").find(' ',found+3);
	
	std::ostringstream tmp;
	tmp << "file://localhost" << escaped;
	return tmp.str();
}

namespace codecs {
	namespace audio {
		AppleAudio AppleAudio::SELF;
		int AppleAudio::accept(const std::string& path) const {
			if (path.find(".mp4")==path.length()-4) return 1000;
			if (path.find(".caf")==path.length()-4) return 1000;
			if (path.find(".m4a")==path.length()-4) return 1000;
			if (path.find(".mp3")==path.length()-4) return 1000;
			if (path.find(".aac")==path.length()-4) return 1000;
			if (path.find(".wav")==path.length()-4) return 1000;
			if (path.find(".pcm")==path.length()-4) return 1000;
			return 0;
		}
		
		Buffer* Vorbis::codecDecode(const Buffer* original, const std::string& path) {
		}
		
		
		void* AppleAudio::decodeAudioImpl(const std::string& path, int& size, int& bits, int& channels, int& rate) {
			DEBUG_LOG("[AppleAudio] Opening file « " << path << " »");
			std::string url(getFileURL(path));
			
			// Build a fucking URL since the framework won't accept a file name
			CFStringRef cfpath = CFStringCreateWithCString(0, url.c_str(), kCFStringEncodingUTF8);
			if (!cfpath) {
				DEBUG_LOG("[AppleAudio] Error at point ① (๏ັ‥̲๏ັ⟱");
				return 0;
			}
			
			CFURLRef inFileURL = CFURLCreateWithString(0, cfpath, 0);
			CFRelease(cfpath);
			if (!inFileURL) {
				DEBUG_LOG("[AppleAudio] Error at point ② (๏ັ‥̲๏ັ⟱");
				return 0;
			}
			// Open URL with ExtAudioFileOpen()
			ExtAudioFileRef extRef = 0;
			OSStatus err = ExtAudioFileOpenURL(inFileURL, &extRef);
			CFRelease(inFileURL);
			if(err || !extRef) {
				DEBUG_LOG("[AppleAudio] Error opening file.");
				return 0;
			}
			
			// Get the audio data format
			AudioStreamBasicDescription inFormat;
			UInt32 propSize = sizeof(inFormat);
			err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &propSize, &inFormat);
			if(err) {
				DEBUG_LOG("[AppleAudio] Error parsing enveloppe (๏ັ‥̲๏ັ⟱");
				ExtAudioFileDispose(extRef);
				return 0;
			}
			
			// Set the client format to 16 bit signed integer (native-endian) data
			// Maintain the channel count and sample rate of the original source format
			AudioStreamBasicDescription outFormat;
			outFormat.mSampleRate       = inFormat.mSampleRate;
			outFormat.mChannelsPerFrame = inFormat.mChannelsPerFrame;
			outFormat.mFormatID         = kAudioFormatLinearPCM;
			outFormat.mBytesPerPacket   = 2 * inFormat.mChannelsPerFrame;
			outFormat.mFramesPerPacket  = 1;
			outFormat.mBytesPerFrame    = 2 * inFormat.mChannelsPerFrame;
			outFormat.mBitsPerChannel   = 16;
			outFormat.mFormatFlags      = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
			
			// Set the desired client (output) data format
			err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(outFormat), &outFormat);
			if(err) {
				DEBUG_LOG("[AppleAudio] WTF? ① (๏ັ‥̲๏ັ⟱");
				ExtAudioFileDispose(extRef);
				return 0;
			}
			
			// Get the total frame count
			SInt64 frameCount = 0;
			propSize = sizeof(frameCount);
			err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &propSize, &frameCount);
			if(err) {
				DEBUG_LOG("[AppleAudio] WTF? ② (๏ັ‥̲๏ັ⟱");
				ExtAudioFileDispose(extRef);
				return 0;
			}
			
			// Read all the data into memory
			UInt32 dataSize = (UInt32)(frameCount * outFormat.mBytesPerFrame);
			void* data = malloc(dataSize);
			if (data) {
				AudioBufferList buf;
				buf.mNumberBuffers = 1;
				buf.mBuffers[0].mDataByteSize = dataSize;
				buf.mBuffers[0].mNumberChannels = outFormat.mChannelsPerFrame;
				buf.mBuffers[0].mData = data;
				
				// Read the data into an AudioBufferList
				err = ExtAudioFileRead(extRef, (UInt32*)&frameCount, &buf);
				if(err) {
					// failure
					free (data);
					data = 0;
					DEBUG_LOG("[AppleAudio] Error reading file (๏ັ‥̲๏ັ⟱");
				}
				else {
					// success
					size = (int)dataSize;
					channels = (int) outFormat.mChannelsPerFrame;
					bits = 16;
					rate = (int)outFormat.mSampleRate;
				}
			}
			else {
				DEBUG_LOG("[AppleAudio] Not enough memory! (๏ັ‥̲๏ັ⟱");
			}
			
			// Dispose the ExtAudioFileRef, it is no longer needed
			if (extRef) ExtAudioFileDispose(extRef);
			return data;
		}
	}
}
 
 */
#endif