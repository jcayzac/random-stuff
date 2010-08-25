#if !defined(UUID_196464216E7A48A9B7194A44B42A1A27)
#define UUID_196464216E7A48A9B7194A44B42A1A27

#include "Codec.h"
namespace codecs {
	namespace audio {
		
		struct AudioEnveloppe: public Codec::Enveloppe {
			long channels;
			long samples;
			long bits;
			long rate;
			inline uintptr_t data() { return ((uintptr_t)this)+sizeof(*this); }
		};
		
		class AudioBase: protected Codec {
		public:
			static AudioEnveloppe* createEnveloppe(long samples, long channels, long bits, long rate);
		protected:
			AudioBase(const std::string& n): Codec(n) { }
		};
		
	}
}

#endif // UUID_196464216E7A48A9B7194A44B42A1A27

