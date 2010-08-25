// $Id: bufferedinputstream.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(BUFFEREDINPUTSTREAM_INCLUDED)
#define BUFFEREDINPUTSTREAM_INCLUDED

#include <llsdk/inc/common/sharedptr.h>
#include <llsdk/inc/common/cscopearrayptr.h>
#include <llsdk/inc/streams/filterinputstream.h>
#include <llsdk/inc/memory/memory.h>

namespace msdk {

class CBufferedInputStream: public CFilterInputStream {
protected:
	CScopeArrayPtr<unsigned char> m_oBuf;	//<! The internal (circular) buffer array where the data is stored.
	unsigned long	m_ulHeadSize;	//<! Capacity of the linear part
	unsigned long	m_ulMaxBytes;	//<! Capacity of the circular part
	unsigned long	m_ulCount;		//<! Bytes currently in the buffer
	unsigned long	m_ulPos;		//<! Current position
	bool			m_bLocked;		//<! Locked?
	bool			m_bSimpleProxy;	//<! If the child input stream supports locking
public:
	/// Creates a BufferedInputStream  with the specified buffer size, and saves its argument, the input stream in, for later use.
	CBufferedInputStream(const shared_ptr<IInputStream>& in, unsigned long bufferSize, unsigned long lockableSize);
	virtual ~CBufferedInputStream() { }

	// From IInputStream
	bool read(unsigned char& b);
	unsigned long read(unsigned char* b, unsigned long off, unsigned long len);
	unsigned long available() const { return m_ulCount; }
	unsigned long skip(unsigned long n);

	// CBufferedInputStream supports locking
	bool lockSupported(unsigned long maxBufferSize) const {
		return (maxBufferSize<=m_ulHeadSize);
	}
	bool locked() const;
	void unlock();
	bool lock(const unsigned char*& lockedBuffer, unsigned long& bufferSize);
private:
	void bufferize();
};

} // namespace msdk

#include <llsdk/inc/memory/memory_end.h>
#endif // BUFFEREDINPUTSTREAM_INCLUDED
