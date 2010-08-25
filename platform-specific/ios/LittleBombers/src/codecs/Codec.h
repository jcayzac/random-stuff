#if !defined(UUID_B9D8EE27E4244E0BAFC235F54F76FA58)
#define UUID_B9D8EE27E4244E0BAFC235F54F76FA58

/** @file Codecs.h
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
// How to implement a new codec named « FooBar »:
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

// FooBar.h:
class FooBar: CodecNode {
public:
	FooBar(): CodecNode("FooBar") { registerCodec(this); }
	int accept(const std::string& path) const;
	void* decodeAudioImpl(const std::string& path, int& size, int& bits, int& channels, int& rate);
private:
	static FooBar SELF;
};

// FooBar.cpp:

FooBar FooBar::SELF; // This will add our codec to the list of available ones

int FooBar::accept(const std::string& path) const {
	return 1;
}

void* FooBar::decodeAudioImpl(const std::string& path, int& size, int& bits, int& channels, int& rate) {
	return an_uncompressed_buffer;
}
*/

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include "utility/Buffer.h"

class Codec {
public:
	enum ECodecFamily {
		SOUND,
		IMAGE
	};
	struct Enveloppe {
		ECodecFamily family;
		long offsetToData;
		long dataSize;
	};
private:
	typedef Codec* codec_pointer;
	typedef std::vector<codec_pointer> Collection;
	Codec(): _name("root") {
		DEBUG_LOG("[Codec] Found new codec: « root » [should always be the 2nd codec found!!]");
	}
	const std::string& name() const { return _name; }
	const Collection& children() const { return _children; }
protected:
	Codec(const std::string& n): _name(n) { }
	void registerCodec(codec_pointer node) {
		DEBUG_LOG("[Codec] Found new codec: « " << node->name() << " »");
		root()->_children.push_back(node);
	}
	// If return value is 0, codec doesn't understand the file.
	// Otherwise it returns the priority of the codec
	virtual int accept(const std::string& path) const { return 0; }
	virtual void* decodeAudioImpl(const std::string& path, int& size, int& bits, int& channels, int& rate) { return 0; }
	
	virtual Buffer* codecDecode(const Buffer* original, const std::string& path) { return 0; }

	// the root node is also a singleton
	static boost::shared_ptr<Codec> root() {
		static boost::shared_ptr<Codec> instance(new (nothrow) Codec);
		return instance;
	};

public:
	virtual ~Codec() { DEBUG_LOG("[Codec] Freeing codec: « " << name() << " »"); }
	static void* decodeAudio(const std::string& path, int& size, int& bits, int& channels, int& rate);
	static Buffer* decode(const Buffer* original, const std::string& path);

private:
	std::string _name;
	Collection _children;
};

#endif // UUID_B9D8EE27E4244E0BAFC235F54F76FA58

