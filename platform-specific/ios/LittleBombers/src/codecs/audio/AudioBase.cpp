#include "AudioBase.h"

namespace codecs {
	namespace audio {


		AudioEnveloppe* AudioBase::createEnveloppe(long samples, long channels, long bits, long rate) {
			long size = (bits>>3)*channels*samples;
			AudioEnveloppe* x = (AudioEnveloppe*) malloc(sizeof(AudioEnveloppe)+size);
			x->family       = Codec::SOUND;
			x->offsetToData = sizeof(AudioEnveloppe);
			x->dataSize     = size;
			x->channels     = channels;
			x->samples      = samples;
			x->bits         = bits;
			x->rate         = rate;
			return x;
		}
		
	}
}