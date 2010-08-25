// $Id: unicode.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(UNICODE_INCLUDED)
#define UNICODE_INCLUDED

#include <llsdk/inc/memory/memory.h>

namespace text {

static const mobilesdk_char LINE_BREAKING[] = {
0x000a,	// LINE FEED
0x000c,	// FORM FEED
0x0085,	// NEXT LINE
0x2028, // LINE SEPARATOR
0x2029,	// PARAGRAPH SEPARATOR
0
};

static const mobilesdk_char NON_BREAKING_SPACE[] = {
0x00a0,	// NBSP
0x202f,	// NARROW NBSP
0x2060, // WORD JOINER
0xfeff, // ZERO WIDTH NO-BREAK SPACE (also used as BOM)
0
};

static const mobilesdk_char BREAKING_SPACE[] = {
0x0009,	// HORIZONTAL TAB
0x000b,	// VERTICAL TAB
0x000d,	// CARRIAGE RETURN
0x0020,	// SPACE
0x1680,	// OGHAM SPACE MARK
0x180e,	// MONGOLIAN VOWEL SEPARATOR
0x2000,	// EN QUAD
0x2001,	// EM QUAD
0x2002,	// EN SPACE
0x2003,	// EM SPACE
0x2004,	// THREE-PER-EM SPACE
0x2005,	// FOUR-PER-EM SPACE
0x2006,	// SIX-PER-EM SPACE
0x2007,	// FIGURE SPACE
0x2008,	// PUNCTUATION SPACE
0x2009,	// THIN SPACE
0x200a,	// HAIR SPACE
0x205f,	// MEDIUM MATHEMATICAL SPACE
0x3000, // IDEOGRAPHIC SPACE
0
};

static inline bool IsLineBreakingSpace(const mobilesdk_char& c) {
	int index=0;
	while (LINE_BREAKING[index]) {
		if (LINE_BREAKING[index]==c) return true;
		++index;
	}
	return false;
}

static inline bool IsBreakingWhiteSpace(const mobilesdk_char& c) {
	if (IsLineBreakingSpace(c)) return true;
	int index=0;
	while (BREAKING_SPACE[index]) {
		if (BREAKING_SPACE[index]==c) return true;
		++index;
	}
	return false;
}

static inline bool IsNonBreakingWhiteSpace(const mobilesdk_char& c) {
	int index=0;
	while (NON_BREAKING_SPACE[index]) {
		if (NON_BREAKING_SPACE[index]==c) return true;
		++index;
	}
	return false;
}

static inline bool IsWhiteSpace(const mobilesdk_char& c) {
	return IsBreakingWhiteSpace(c) || IsNonBreakingWhiteSpace(c);
}


static inline void TrimSpaces(mobilesdk_char*& data, unsigned long& length) {
	// Trim leading spaces
	while(length && (IsWhiteSpace(data[length-1]))) --length;
	while (length && (IsWhiteSpace(*data))) { ++data; --length; }
}

} // namespace text

#include <llsdk/inc/memory/memory_end.h>
#endif // UNICODE_INCLUDED
