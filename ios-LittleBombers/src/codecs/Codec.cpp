#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <map>
#include "Codec.h"



Buffer* Codec::decode(const Buffer* original, const std::string& path) {
	Buffer* result = 0;
	boost::shared_ptr<Codec> instance = Codec::root();
	assert((bool)instance);
	Codec& THIS = *instance;
	DEBUG_LOG("[Codec] Requesting codec for file « " << path << " »");
	if (THIS._children.empty()) {
		DEBUG_LOG("[Codec] Error: No codec has been registered yet! (๏ັ‥̲๏ັ⟱");
		return result;
	}
	// Collect the best codecs for this file
	std::multimap<int, codec_pointer> best_codecs;
	Collection::iterator it = THIS._children.begin();
	while (it!=THIS._children.end()) {
		codec_pointer codec = *(it); ++it;
		int pri = codec->accept(path);
		if (pri>0) best_codecs.insert(std::pair<int,codec_pointer>(pri,codec));
	}
	int count = best_codecs.size(); (void)count;
	if (count>1) { DEBUG_LOG("[Codec] " << count << " decoders accept this file type."); }
	else if (count==1) { DEBUG_LOG("[Codec] 1 decoder accepts this file type."); }

	// Try all the collected codecs until we get a valid pointer
	std::multimap<int,codec_pointer>::reverse_iterator it2 = best_codecs.rbegin();
	while (it2!=best_codecs.rend()) {
		codec_pointer codec = (*it2).second;
		DEBUG_LOG("[Codec] Trying decoder « " << codec->name() << " » (pri:" << (*it2).first << ")…");
		result=codec->codecDecode(original, path);
		if (result) {
			DEBUG_LOG("[Codec] " << codec->name() << " did great. Now returning uncompressed buffer to caller.");
			break;
		}
		DEBUG_LOG("[Codec] " << codec->name() << " could not decode the file.");
		++it2;
	}
	if (!result) DEBUG_LOG("[Codec] No codec could decode this file! ヾ(*｀Д´*)ﾉ\"");
	return result;	
}

void* Codec::decodeAudio(const std::string& path, int& size, int& bits, int& channels, int& rate) {
	boost::shared_ptr<Codec> instance = Codec::root();
	assert((bool)instance);
	Codec& THIS = *instance;
	DEBUG_LOG("[Codec] Requesting codec for file « " << path << " »");
	if (THIS._children.empty()) {
		DEBUG_LOG("[Codec] Error: No codec has been registered yet! (๏ັ‥̲๏ັ⟱");
		return 0;
	}
	
	// Collect the best codecs for this file
	std::multimap<int, codec_pointer> best_codecs;
	Collection::iterator it = THIS._children.begin();
	while (it!=THIS._children.end()) {
		codec_pointer codec = *(it); ++it;
		int pri = codec->accept(path);
		if (pri>0) best_codecs.insert(std::pair<int,codec_pointer>(pri,codec));
	}
	int count = best_codecs.size(); (void)count;
	if (count>1) { DEBUG_LOG("[Codec] " << count << " decoders accept this file type."); }
	else if (count==1) { DEBUG_LOG("[Codec] 1 decoder accepts this file type."); }

	// Try all the collected codecs until we get a valid pointer
	void* data=0;
	std::multimap<int,codec_pointer>::reverse_iterator it2 = best_codecs.rbegin();
	while (it2!=best_codecs.rend()) {
		codec_pointer codec = (*it2).second;
		DEBUG_LOG("[Codec] Trying decoder « " << codec->name() << " » (pri:" << (*it2).first << ")…");
		data=codec->decodeAudioImpl(path,size,bits,channels,rate);
		if (data) {
			DEBUG_LOG("[Codec] " << codec->name() << " did great. Now returning uncompressed buffer to caller.");
			break;
		}
		DEBUG_LOG("[Codec] " << codec->name() << " could not decode the file.");
		++it2;
	}
	if (!data) DEBUG_LOG("[Codec] No codec could decode this file! ヾ(*｀Д´*)ﾉ\"");
	return data;
}

