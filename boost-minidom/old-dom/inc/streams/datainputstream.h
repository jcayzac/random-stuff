// $Id: datainputstream.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(DATAINPUTSTREAM_INCLUDED)
#define DATAINPUTSTREAM_INCLUDED

#include <llsdk/inc/common/sharedptr.h>
#include <llsdk/inc/text/encoding.h>
#include <llsdk/inc/streams/filterinputstream.h>
#include <llsdk/inc/streams/bufferedinputstream.h>
#include <llsdk/inc/memory/memory.h>

namespace msdk {

/** @brief Data input stream
  * Implementation is inlined right here to avoid performance bottlenecks when
  * using a CDataInputStream around a CByteArrayInputStream (method calls, ...)
  */
class CDataInputStream: public CFilterInputStream {
public:
	enum EByteOrder {
		HOST_ORDER,
		NETWORK_ORDER
	};
private:
	static const unsigned long ORDER = 0x01000000;
	inline static bool badOrder(EByteOrder order) {
		return (order != ((*((const unsigned char*)ORDER))?NETWORK_ORDER:HOST_ORDER));
	}
public:
	CDataInputStream(const shared_ptr<IInputStream>& in, EByteOrder order=NETWORK_ORDER)
	: m_eByteOrder(order)
	{ 
		if (in && in->markSupported())
			m_pIn=in;
		else
			m_pIn.reset(new CBufferedInputStream(in));	
	}
	virtual ~CDataInputStream() { }

	//
	bool readUnsignedInt8(unsigned char& v) {
		return read(v);
	}

	bool readUnsignedInt16(unsigned short& v) {
		bool result = false;
		if (available()>=2) {
			unsigned char* mem = (unsigned char*)&v;
			result = (2==read(mem,0,2));
			if (badOrder(m_eByteOrder)) {
				unsigned char c = mem[0];
				mem[0]=mem[1];
				mem[1]=mem[0];
			}
		}
		return result;
	}

	bool readUnsignedInt32(unsigned long& v) {
		bool result = false;
		if (available()>=4) {
			unsigned short* mem = (unsigned short*)&v;
			result = readUnsignedInt16(mem[0]);
			result = result && readUnsignedInt16(mem[1]);
			if (badOrder(m_eByteOrder)) {
				unsigned short tmp = mem[0];
				mem[0]=mem[1];
				mem[1]=mem[0];
			}
		}
		return result;
	}

	bool readUnsignedInt64(unsigned long long& v) {
		bool result = false;
		if (available()>=8) {
			unsigned long* mem = (unsigned long*)&v;
			result = readUnsignedInt32(mem[0]);
			result = result && readUnsignedInt32(mem[1]);
			if (badOrder(m_eByteOrder)) {
				unsigned long tmp = mem[0];
				mem[0]=mem[1];
				mem[1]=mem[0];
			}
		}
		return result;
	}

	bool readBoolean(bool& v) {
		unsigned char c;
		if (!readUnsignedInt8(c)) return false;
		return (c!=0);
	}
	bool readInt64(long long& v)	{	return readUnsignedInt64(	*(	(unsigned long long*)&v	)); }
	bool readInt32(long& v)			{	return readUnsignedInt32(	*(	(unsigned long*)&v		)); }
	bool readInt16(short& v)		{	return readUnsignedInt16(	*(	(unsigned short*)&v		)); }
	bool readInt8(char& v)			{	return readUnsignedInt8(	*(	(unsigned char*)&v		)); }
private:
	EByteOrder	m_eByteOrder;
};

} // namespace msdk

#include <llsdk/inc/memory/memory_end.h>
#endif // DATAINPUTSTREAM_INCLUDED
