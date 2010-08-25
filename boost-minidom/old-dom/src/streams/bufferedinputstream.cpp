// $Id: bufferedinputstream.cpp 10328 2008-04-08 12:58:06Z jbernardes $
#include <llsdk/inc/streams/bufferedinputstream.h>
#include <llsdk/inc/memory/memory.h>
#include <string.h>

namespace msdk {

static const unsigned long DEFAULT_BUFFER_SIZE = 1024;

CBufferedInputStream::CBufferedInputStream(const shared_ptr<IInputStream>& in, unsigned long bufferSize, unsigned long lockableSize)
: CFilterInputStream(in)
, m_ulHeadSize(lockableSize)
, m_ulMaxBytes(bufferSize)
, m_ulCount(0)
, m_ulPos(0)
, m_bLocked(false)
{
	if (!m_ulHeadSize)
		m_ulHeadSize=DEFAULT_BUFFER_SIZE;
	if (!m_ulMaxBytes)
		m_ulMaxBytes=DEFAULT_BUFFER_SIZE;
	if (m_ulMaxBytes<m_ulHeadSize)
		m_ulMaxBytes=m_ulHeadSize;
	m_bSimpleProxy = (in && in->lockSupported(m_ulHeadSize));
	if (!m_bSimpleProxy)
		m_oBuf.Reset(new unsigned char[m_ulMaxBytes + m_ulHeadSize]);
}

unsigned long CBufferedInputStream::skip(unsigned long n) {
	if (m_bSimpleProxy)
		return m_pIn->skip(n);
	else if (m_bLocked)
		return 0;
	else if (n==0)
		return 0;
	else if (n<=m_ulCount) {
		// Remove n bytes from buffer
		if (m_ulPos + n < m_ulHeadSize + m_ulMaxBytes) {
			// linear buffer
			m_ulPos+=n;
			m_ulCount-=n;
			return n;
		}
		else {
			// circular buffer
			unsigned long skipped=m_ulHeadSize + m_ulMaxBytes - m_ulPos; // len from pos to the end
			m_ulPos=m_ulHeadSize;
			m_ulCount-=skipped;
			n-=skipped;
			return skipped+=skip(n);
		}
	}
	else {
		// Skip the whole buffer, then refill it, and do it while len>0
		unsigned long skipped = skip(m_ulCount);
		n-=skipped;
		bufferize();
		return skipped+skip(n);
	}
}

bool CBufferedInputStream::read(unsigned char& b) {
	if (m_bSimpleProxy)
		return m_pIn->read(b);
	else if (m_bLocked)
		return false;
	else {
		return (1==read(&b,0,1));
	}
}
unsigned long CBufferedInputStream::read(unsigned char* b, unsigned long off, unsigned long len) {
	if (m_bSimpleProxy)
		return m_pIn->read(b,off,len);
	else if (m_bLocked)
		return 0;
	else if (len==0)
		return 0;
	else if (len<=m_ulCount) {
		// Read "len" bytes from buffer
		if (m_ulPos + len < m_ulHeadSize + m_ulMaxBytes) {
			// linear buffer
			memcpy(b+off,m_oBuf.get()+m_ulPos,len);
			m_ulPos+=len;
			m_ulCount-=len;
			return len;
		}
		else {
			// circular buffer
			unsigned long wrote=m_ulHeadSize + m_ulMaxBytes - m_ulPos;	// len from pos to the end
			memcpy(b+off,m_oBuf.get()+m_ulPos,wrote);						// copy (1st)
			m_ulPos=m_ulHeadSize;
			m_ulCount-=wrote;
			len-=wrote;
			off+=wrote;
			return wrote+read(b,off,len);
		}
	}
	else {
		// Read the whole buffer, then refill it, and do it while len>0
		unsigned long wrote = read(b,off,m_ulCount);
		len-=wrote;
		off+=wrote;
		bufferize();
		return wrote+read(b,off,len);
	}
}

bool CBufferedInputStream::lock(const unsigned char*& lockedBuffer, unsigned long& bufferSize) {
	if (m_bSimpleProxy)
		return m_pIn->lock(lockedBuffer,bufferSize);
	else if (m_bLocked) {
		lockedBuffer=0;
		bufferSize=0;
		return false;
	}
	else if (bufferSize==0) {
		lockedBuffer=0;
		bufferSize=0;
		return true;
	}
	else if (bufferSize<=m_ulHeadSize) {
		if (bufferSize>m_ulCount)
			bufferize();
		if (m_ulPos + bufferSize < m_ulHeadSize + m_ulMaxBytes) {
			// linear buffer
			lockedBuffer=m_oBuf.get()+m_ulPos;
			m_bLocked=true;
			if (bufferSize>m_ulCount) {
				bufferSize=m_ulCount;
				return false;
			}
			return true;
		}
		else {
			// circular buffer
			// 1/ linearize
			unsigned long displaced = m_ulHeadSize + m_ulMaxBytes - m_ulPos;
			unsigned long newPos  = m_ulPos - m_ulMaxBytes;
			for (unsigned long i=0; i<displaced; ++i)
				*(m_oBuf.get()+newPos+i)=*(m_oBuf.get()+m_ulPos+i);
			m_ulPos=newPos;
			// 2/ Call the linear code
			return lock(lockedBuffer,bufferSize);
		}
	}
	else {
		// bufferSize>m_ulHeadSize
		// 1/ Realloc so that m_ulHeadSize==bufferSize and m_ulMaxBytes==bufferSize
		CScopeArrayPtr<unsigned char> newBuffer(new unsigned char[bufferSize<<1]);
		if (!newBuffer) {
			lockedBuffer=0;
			bufferSize=0;
			return false;
		}
		// Read "len" bytes from buffer
		if (m_ulPos + m_ulCount < m_ulHeadSize + m_ulMaxBytes) {
			// linear buffer
			memcpy(newBuffer.get(),m_oBuf.get()+m_ulPos,m_ulCount);
		}
		else {
			// circular buffer
			unsigned long tmp=m_ulHeadSize + m_ulMaxBytes - m_ulPos;	// len from pos to the end
			memcpy(newBuffer.get(),m_oBuf.get()+m_ulPos,tmp);					// copy (1st)
			m_ulPos=m_ulHeadSize;
			memcpy(newBuffer.get()+tmp,m_oBuf.get()+m_ulPos,m_ulCount-tmp);
		}
		m_oBuf.Swap(newBuffer);
		m_ulPos=0;
		m_ulHeadSize=bufferSize;
		m_ulMaxBytes=bufferSize;

		bufferize();
		bool result = (bufferSize==m_ulCount);
		bufferSize=m_ulCount;
		lockedBuffer = m_oBuf.get();
		m_bLocked=true;
		return result;
	}
}

bool CBufferedInputStream::locked() const {
	if (m_bSimpleProxy)
		return m_pIn->locked();
	else {
		return m_bLocked;
	}
}
void CBufferedInputStream::unlock() {
	if (m_bSimpleProxy)
		m_pIn->unlock();
	else {
		m_bLocked=false;
	}
}

void CBufferedInputStream::bufferize() {
	// m_pIn exists
	// m_pIn->lockSupported(0) returns false
	if (!m_oBuf) return;
	if (!m_pIn->available()) return;

	// How many bytes do we want to read?
	unsigned long fillCount = m_ulMaxBytes-m_ulCount;
	if (!fillCount) return; // buffer full

	// Starting at which position?
	unsigned long startIndex = m_ulPos+m_ulCount;
	if (startIndex + fillCount < m_ulHeadSize + m_ulMaxBytes) {
		// linear buffer
		unsigned long readCount=0;
		do {
			readCount += m_pIn->read(m_oBuf.get()+startIndex, readCount, fillCount-readCount);
		} while (readCount>0 && readCount<fillCount);
		m_ulCount+=readCount;
	}
	else {
		// circular buffer
		unsigned long tmp=m_ulHeadSize + m_ulMaxBytes - startIndex;	// len from startIndex to the end
		unsigned long readCount=0;
		do {
			readCount += m_pIn->read(m_oBuf.get()+startIndex, readCount, tmp-readCount);
		} while (readCount>0 && readCount<tmp);
		m_ulCount+=tmp;
		bufferize();
	}
}

} // namespace msdk



