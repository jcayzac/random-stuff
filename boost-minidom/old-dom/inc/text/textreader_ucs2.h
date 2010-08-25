// $Id: textreader_ucs2.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(TEXTREADER_UCS2_INCLUDED)
#define TEXTREADER_UCS2_INCLUDED

#include <llsdk/inc/text/textreader_latin1.h>
#include <llsdk/inc/memory/memory.h>

namespace text {

class TUcs2Reader: public TLatin1Reader {
public:
	TUcs2Reader(bool isLittleEndian): m_bIsLittleEndian(isLittleEndian) { }
	virtual mobilesdk_char read(const unsigned char*& address, unsigned long& size, bool& malformed) const {
		mobilesdk_char result = 0;
		if (size>=2) {
			result = ((const mobilesdk_char*)address)[0];
			address+=2;
			size-=2;
			if (!m_bIsLittleEndian) {
				result = ((result&0xff00)>>8)|((result&0x00ff)<<8);
			}
		}
		malformed=false;
		return result;
	}
	virtual TEncoding encoding() const {
		return m_bIsLittleEndian?ENCODING_UCS2LE:ENCODING_UCS2BE;
	}
protected:
	bool	m_bIsLittleEndian;
};

} // namespace text

#include <llsdk/inc/memory/memory_end.h>
#endif // TEXTREADER_UCS2_INCLUDED
