// $Id: fileinputstream.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(FILEINPUTSTREAM_INCLUDED)
#define FILEINPUTSTREAM_INCLUDED
#include <llsdk/inc/common/cstring.h>
#include <llsdk/inc/storage/cpath.h>
#include <llsdk/inc/storage/clocalfile.h>
#include <llsdk/inc/streams/inputstream.h>
#include <llsdk/inc/memory/memory.h>

namespace msdk {

class CFileInputStream: public IInputStream {
public:
	CFileInputStream(const CString& path);
	CFileInputStream(const CPath& path);
	CFileInputStream(const CLocalFile& file);
	virtual ~CFileInputStream();

	bool opened()  const;

	// From IInputStream
	unsigned long available() const;
	bool eof() const;
	bool read(unsigned char& b);
	unsigned long read(unsigned char* b, unsigned long off, unsigned long len);
	unsigned long skip(unsigned long n);

	// Locking is not supported: It could be, but forcing the use
	// of a CBufferedInputStream on top of a CFileInputStream will
	// improve performances.
private:
	CLocalFile		m_oFile;
	unsigned long	m_ulFileOffset;
	unsigned long	m_ulRemainingSize;
};

} // namespace msdk

#include <llsdk/inc/memory/memory_end.h>
#endif // FILEINPUTSTREAM_INCLUDED
