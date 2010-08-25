// $Id: textreader_latin1.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(TEXTREADER_LATIN1_INCLUDED)
#define TEXTREADER_LATIN1_INCLUDED

#include <llsdk/inc/text/textreader.h>
#include <llsdk/inc/memory/memory.h>

namespace text {

class TLatin1Reader: public ICharacterReader {
public:
	virtual mobilesdk_char read(const unsigned char*& address, unsigned long& size, bool& malformed) const {
		mobilesdk_char result = 0;
		if (size) {
			result = (mobilesdk_char) *address;
			++address;
			--size;
		}
		malformed=false;
		return result;
	}
	virtual TEncoding encoding() const {
		return ENCODING_LATIN1;
	}
};

} // namespace text

#include <llsdk/inc/memory/memory_end.h>
#endif // TEXTREADER_LATIN1_INCLUDED
