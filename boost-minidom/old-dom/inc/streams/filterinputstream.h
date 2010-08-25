// $Id: filterinputstream.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(FILTERINPUTSTREAM_INCLUDED)
#define FILTERINPUTSTREAM_INCLUDED

#include <llsdk/inc/common/sharedptr.h>
#include <llsdk/inc/streams/inputstream.h>
#include <llsdk/inc/memory/memory.h>

namespace msdk {

class CFilterInputStream: public IInputStream {
protected:
	shared_ptr<IInputStream> m_pIn;
	CFilterInputStream() { }
	CFilterInputStream(const shared_ptr<IInputStream>& in): m_pIn(in) { }
public:
	virtual ~CFilterInputStream() { }

	// From IInputStream
	virtual unsigned long available() const {
		if (!m_pIn) return 0;
		return m_pIn->available();
	}
	virtual bool eof() const {
		if (!m_pIn) return true;
		return (m_pIn->eof() && !available());
	}
	virtual bool read(unsigned char& b) {
		if (!m_pIn) return false;
		return m_pIn->read(b);
	}
	virtual unsigned long read(unsigned char* b, unsigned long off, unsigned long len) {
		if (!m_pIn) return 0;
		return m_pIn->read(b,off,len);
	}
	virtual unsigned long skip(unsigned long n) {
		if (!m_pIn) return 0;
		return m_pIn->skip(n);
	}
	virtual bool lockSupported(unsigned long maxBufferSize) const {
		if (!m_pIn) return false;
			return m_pIn->lockSupported(maxBufferSize);
	}
	virtual bool locked() const {
		if (!m_pIn) return false;
			return m_pIn->locked();
	}
	virtual bool lock(const unsigned char*& lockedBuffer, unsigned long& bufferSize) {
		if (!m_pIn) return false;
			return m_pIn->lock(lockedBuffer,bufferSize);
	}
	virtual void unlock() {
		if (m_pIn)
			m_pIn->unlock();
	}
};

} // namespace msdk

#include <llsdk/inc/memory/memory_end.h>
#endif // FILTERINPUTSTREAM_INCLUDED
