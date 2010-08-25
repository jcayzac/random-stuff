// $Id: textreader_windows1252.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/text/textreader_windows1252.h>
#include <llsdk/inc/memory/memory.h>

namespace text {

static const mobilesdk_char TRANSLATION[] = {
0x0081,0x0000,
0x008d,0x0000,
0x008f,0x0000,
0x0090,0x0000,
0x009d,0x0000,
0x0080,0x20ac,
0x0082,0x201a,
0x0083,0x0192,
0x0084,0x201e,
0x0085,0x2026,
0x0086,0x2020,
0x0087,0x2021,
0x0088,0x02c6,
0x0089,0x2030,
0x008a,0x0160,
0x008b,0x2039,
0x008c,0x0152,
0x008e,0x017d,
0x0091,0x2018,
0x0092,0x2019,
0x0093,0x201c,
0x0094,0x201d,
0x0095,0x2022,
0x0096,0x2013,
0x0097,0x2014,
0x0098,0x02dc,
0x0099,0x2122,
0x009a,0x0161,
0x009b,0x203a,
0x009c,0x0153,
0x009e,0x017e,
0x009f,0x0178,
0
};

mobilesdk_char TWindows1252Reader::read(const unsigned char*& address, unsigned long& size, bool& malformed) const {
	mobilesdk_char result = TLatin1Reader::read(address,size,malformed);
	int i=0;
	while (TRANSLATION[i]) {
		if (TRANSLATION[i]==result) return TRANSLATION[i+1];
		++i;
	}
	return result;
}
void TWindows1252Reader::translateLatin1(mobilesdk_char* address, unsigned long size) {
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
