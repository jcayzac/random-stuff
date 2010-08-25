// $Id: inputstream.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(INPUTSTREAM_INCLUDED)
#define INPUTSTREAM_INCLUDED
#include <llsdk/inc/memory/memory.h>

namespace msdk {

class IInputStream {
public:
	/// Closes this input stream and releases any system resources associated with the stream.
	virtual ~IInputStream() { }
	/// Returns the number of bytes that can be read (or skipped over)
	/// from this input stream without blocking by the next caller of a
	/// method for this input stream.
	virtual unsigned long available() const = 0;
	/// Return true if this stream is exhausted.
	virtual bool eof() const = 0;
	/// Reads the next byte of data from the input stream.
	virtual bool read(unsigned char& b) = 0;
	/// Reads some number of bytes from the input stream and stores them into the buffer array b.
	virtual unsigned long read(unsigned char* b, unsigned long off, unsigned long len) = 0;
	inline unsigned long read(unsigned char* b, unsigned long len) {
		return read(b, 0, len);
	}

	/// Skips over and discards n bytes of data from this input stream.
	virtual unsigned long skip(unsigned long n) = 0;

	/// Tests if this input stream supports the lock and unlock methods.
	virtual bool lockSupported(unsigned long /*maxBufferSize*/) const { return false; }
	/// Tells weither this stream is locked or not
	virtual bool locked() const { return false; }
	/// Lock the current input stream.
	/// @param lockedBuffer Receives the address of the locked buffer on success, null otherwise.
	/// @param bufferSize Requested number of bytes to be locked. On output, receives the number of bytes that could be locked.
	/// @return true if bufferSize is inchanged, i.e. we've got all the bytes we requested. false if it has changed.
	virtual bool lock(const unsigned char*& /*lockedBuffer*/, unsigned long& /*bufferSize*/) { return false; }
	/// Unlock this stream
	virtual void unlock() { }
};

} // namespace msdk

#include <llsdk/inc/memory/memory_end.h>
#endif // INPUTSTREAM_INCLUDED
