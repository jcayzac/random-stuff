#if !defined(UUID_75D437020ECC4E81A8CBBB4912132334)
#define UUID_75D437020ECC4E81A8CBBB4912132334
#include "codecs/audio/AudioBase.h"
namespace codecs {
namespace audio {

	class Vorbis: public AudioBase {
	public:
		Vorbis(): AudioBase("Vorbis") { registerCodec(this); }
		int accept(const std::string& path) const;
		void* decodeAudioImpl(const std::string& path, int& size, int& bits, int& channels, int& rate);
		Buffer* codecDecode(const Buffer* original, const std::string& path);
	private:
		static Vorbis SELF;
	};
	
}
}

#endif // UUID_75D437020ECC4E81A8CBBB4912132334

