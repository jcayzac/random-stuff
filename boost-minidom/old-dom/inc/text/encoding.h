// $Id: encoding.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(ENCODING_INCLUDED)
#define ENCODING_INCLUDED

#include <llsdk/inc/memory/memory.h>

namespace text {

/// @brief List of supported encodings
enum TEncoding {
	ENCODING_UNKNOWN = 0,
	ENCODING_AUTODETECT,
	// 7-bit encodings
	ENCODING_US_ASCII,
	// 8-bit encodings
	ENCODING_LATIN1,
	ENCODING_ISO_8859_1=ENCODING_LATIN1,
	ENCODING_LATIN9,
	ENCODING_ISO_8859_15=ENCODING_LATIN9,
	ENCODING_UTF8,
	ENCODING_WINDOWS_1252,
	// 16-bit encodings
	ENCODING_UCS2LE,
	ENCODING_UCS2BE,
	ENCODING_UTF16LE,	//<! @note Surrogates are not supported.
	ENCODING_UTF16BE,	//<! @note Surrogates are not supported.
	/* Not supported:
	ENCODING_UCS4LE,
	ENCODING_UCS4BE,
	ENCODING_UTF32LE,
	ENCODING_UTF32BE,
	*/
	ENCODING_LAST=ENCODING_UTF16BE
};

} // namespace text

#include <llsdk/inc/memory/memory_end.h>
#endif // ENCODING_INCLUDED
