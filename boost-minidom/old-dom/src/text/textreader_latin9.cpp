// $Id: textreader_latin9.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/text/textreader_latin9.h>
#include <llsdk/inc/memory/memory.h>

namespace text {

static const mobilesdk_char TRANSLATION[] = {
	0x00a4,0x20ac,	// CURRENCY			=> EURO SIGN
	0x00a6,0x0160,	// BROKEN BAR		=> CAPITAL S WITH HACEK
	0x00a8,0x0161,	// UMLAUT			=> S WITH HACEK
	0x00b4,0x017d,	// ACCUTE ACCENT	=> CAPITAL Z WITH HACEK
	0x00b8,0x017e,	// CEDILLA			=> Z WITH HACEK
	0x00bc,0x0152,	// QUARTER			=> CAPITAL EOGULE
	0x00bd,0x0153,	// ONE HALF			=> EOGULE
	0x00be,0x0178,	// THREE QUARTERS	=> CAPITAL Y WITH DIAERESIS
	0
};

mobilesdk_char TLatin9Reader::read(const unsigned char*& address, unsigned long& size, bool& malformed) const {
	mobilesdk_char result = TLatin1Reader::read(address,size,malformed);
	int i=0;
	while (TRANSLATION[i]) {
		if (TRANSLATION[i]==result) return TRANSLATION[i+1];
		++i;
	}
	return result;
}

void TLatin9Reader::translateLatin1(mobilesdk_char* address, unsigned long size) {
	while (size) {
		int i=0;
		while (TRANSLATION[i]) {
			if (TRANSLATION[i]==*address) {
				*address=TRANSLATION[i+1];
				break;
			}
			++i;
		}
		++address;
		--size;
	}
}


} // namespace text
