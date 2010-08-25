// $Id: textreader_ascii.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(TEXTREADER_ASCII_INCLUDED)
#define TEXTREADER_ASCII_INCLUDED

#include <llsdk/inc/text/textreader_latin1.h>
#include <llsdk/inc/memory/memory.h>

namespace text {

class TUSAsciiReader: public TLatin1Reader {
public:
	virtual mobilesdk_char read(const unsigned char*& address, unsigned long& size, bool& malformed) const {
		mobilesdk_char result = TLatin1Reader::read(address,size,malformed);
		malformed|=(result>0x007f);
		return malformed?0:result;
	}
	virtual TEncoding encoding() const {
		return ENCODING_US_ASCII;
	}
};

} // namespace text

#include <llsdk/inc/memory/memory_end.h>
#endif // TEXTREADER_ASCII_INCLUDED
