// $Id: bytearrayinputstream.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(BYTEARRAYINPUTSTREAM_INCLUDED)
#define BYTEARRAYINPUTSTREAM_INCLUDED
#include <llsdk/inc/memory/memory.h>

namespace msdk {

/// CByteArrayInputStream maps a unsigned char array to an IInputStream
class CByteArrayInputStream: public IInputStream {
public:
	/// @brief Wrap an IInputStream around an array
	/// @param address	Address of the array
	/// @param size		Size of the array, in bytes
	/// @note Array's ownership is not transfered to the resulting object.
	CByteArrayInputStream(const unsigned char* address, unsigned long size)
	: m_pCurrentAddress(address)
	, m_ulRemainingSize(size)
	, m_bLocked(false)
	{
		if (!address) {
			m_ulRemainingSize=0;
		}
	}
	virtual ~CByteArrayInputStream()
	{ }

	// From IInputStream
	unsigned long available() const {
		return m_ulRemainingSize;
	}
	bool eof() const {
		return (m_ulRemainingSize==0);
	}
	bool read(unsigned char& b) {
		if (m_bLocked) return false; // locked
		if (!m_ulRemainingSize) return false;
		b=*m_pCurrentAddress;
		++m_pCurrentAddress;
		--m_ulRemainingSize;
		return true;
	}
	unsigned long read(unsigned char* b, unsigned long off, unsigned long len) {
		if (m_bLocked) return 0; // locked
		len=(len>m_ulRemainingSize)?m_ulRemainingSize:len;
		if (len) {
			memcpy(b+off,m_pCurrentAddress,len);
			m_pCurrentAddress+=len;
			m_ulRemainingSize-=len;
			return len;
		}
		return 0;
	}
	unsigned long skip(unsigned long n) {
		if (m_bLocked) return 0; // locked
		if (!m_ulRemainingSize) return 0;
		n=(n>m_ulRemainingSize)?m_ulRemainingSize:n;
		m_pCurrentAddress+=n;
		m_ulRemainingSize-=n;
		return n;
	}
	bool lockSupported(unsigned long maxBufferSize) const {
		return (maxBufferSize<=m_ulRemainingSize);
	}
	bool locked() const { return m_bLocked; }
	bool lock(const unsigned char*& lockedBuffer, unsigned long& bufferSize) {
		if (m_bLocked) {
			lockedBuffer=0;
			bufferSize=0;
			return false; // already locked
		}
		bufferSize=(bufferSize>m_ulRemainingSize)?m_ulRemainingSize:bufferSize;
		lockedBuffer=m_pCurrentAddress;
		m_bLocked=true;
		return m_bLocked;
	}
	void unlock() {
		if (m_bLocked)
			m_bLocked=false;
	}
private:
	const unsigned char*	m_pCurrentAddress; //<! Current data address
	unsigned long			m_ulRemainingSize; //<! Remaining data size, in bytes.
	bool					m_bLocked;
};

} // namespace msdk

#include <llsdk/inc/memory/memory_end.h>
#endif // BYTEARRAYINPUTSTREAM_INCLUDED
