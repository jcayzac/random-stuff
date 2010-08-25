// $Id: textreader_latin9.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(TEXTREADER_LATIN9_INCLUDED)
#define TEXTREADER_LATIN9_INCLUDED

#include <llsdk/inc/text/textreader_latin1.h>
#include <llsdk/inc/memory/memory.h>

namespace text {

class TLatin9Reader: public TLatin1Reader {
public:
	virtual mobilesdk_char read(const unsigned char*& address, unsigned long& size, bool& malformed) const;
	virtual TEncoding encoding() const {
		return ENCODING_LATIN9;
	}
	static void translateLatin1(mobilesdk_char* address, unsigned long size);
};

} // namespace text

#include <llsdk/inc/memory/memory_end.h>
#endif // TEXTREADER_LATIN9_INCLUDED
