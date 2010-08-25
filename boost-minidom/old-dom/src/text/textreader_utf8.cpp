// $Id: textreader_utf8.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/text/textreader_utf8.h>
#include <llsdk/inc/memory/memory.h>

namespace text {

template <typename T>
static mobilesdk_char convertToUtf8(const T*& address, unsigned long& size, bool& malformed) {
	malformed=false;
	if (size>=1) {
		// ASCII compatibility range
		mobilesdk_char first = (mobilesdk_char)(address[0]);
		if (first<=0x007f) {
			++address;
			--size;
			return first;
		}
		else if (size>=2 &&
				first>=0x00c2 &&
				first<=0x00df) {
			// 2-byte characters
			mobilesdk_char second = (mobilesdk_char)(address[1]);
			if (second>=0x0080 && second<=0x00bf) {
				address+=2;
				size+=2;
				return ((first&0x001f)<<6)|(second&0x003f);
			}
			else if (size>=3 &&
					first>=0x00e0 &&
					first<=0x00ef &&
					second>=0x0080 &&
					second<=0x00bf) {
				// 3-byte characters
				mobilesdk_char third = (mobilesdk_char)(address[2]);
				if (third>=0x0080 && third<=0x00bf) {
					address+=2;
					size+=2;
					return ((first&0x000f)<<12)|((second&0x003f)<<6)|(third&0x003f);
				}
				else if (size>=4 &&
						first>=0x00f0 &&
						first<=0x00f4 &&
						second>=0x0080 &&
						second<=0x00bf &&
						third>=0x0080 &&
						third<=0x00bf) {
					// 4-byte characters
					address+=4;
					size-=4;
					// result will exceed 16-bit, so it's ignored
				}
				else malformed=true;
			}
		}
	}
	return 0;
}


mobilesdk_char TUtf8Reader::read(const unsigned char*& address, unsigned long& size, bool& malformed) const {
	return convertToUtf8<unsigned char>(address,size,malformed);
}
void TUtf8Reader::translateLatin1(mobilesdk_char* address, unsigned long size) {
	if (!size) return;
	const mobilesdk_char*	utf8Address	= address;
	unsigned long			utf8Size	= size;
	bool malformed=false;
	mobilesdk_char c=0;
	do {
		c = convertToUtf8<mobilesdk_char>(utf8Address,utf8Size,malformed);
		*address=c;
		++address;
		--size;
	} while (utf8Size && c && !malformed);
	// Pad with zeroes
	while (size) {
		*address=0;
		++address;
		--size;
	}
}

} // namespace text
