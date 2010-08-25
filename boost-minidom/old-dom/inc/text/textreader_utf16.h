// $Id: textreader_utf16.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(TEXTREADER_UTF16_INCLUDED)
#define TEXTREADER_UTF16_INCLUDED

#include <llsdk/inc/text/textreader_ucs2.h>
#include <llsdk/inc/memory/memory.h>

namespace text {

class TUtf16Reader: public TUcs2Reader {
public:
	TUtf16Reader(bool isLittleEndian): TUcs2Reader(isLittleEndian) { }
	virtual mobilesdk_char read(const unsigned char*& address, unsigned long& size, bool& malformed) const {
		mobilesdk_char result = TUcs2Reader::read(address,size,malformed);
		// We do not support surrogates!!!
		if (result>=0xd800 && result<=0xdfff) {
			if (size>=2) {
				size-=2; // skip pair's second
				address+=2;
			}
			else malformed=true;
		}
		return malformed?0:result;
	}
	virtual TEncoding encoding() const {
		return m_bIsLittleEndian?ENCODING_UTF16LE:ENCODING_UTF16BE;
	}
};

} // namespace text

#include <llsdk/inc/memory/memory_end.h>
#endif // TEXTREADER_UTF16_INCLUDED
