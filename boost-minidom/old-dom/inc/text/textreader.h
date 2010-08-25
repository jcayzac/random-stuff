// $Id: textreader.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(TEXTREADER_INCLUDED)
#define TEXTREADER_INCLUDED

#include <llsdk/inc/text/encoding.h>
#include <llsdk/inc/streams/inputstream.h>
#include <llsdk/inc/common/cstring.h>
#include <llsdk/inc/common/sharedptr.h>
#include <llsdk/inc/scheduler/cservice.h>
#include <llsdk/inc/scheduler/ctask.h>
#include <llsdk/inc/scheduler/cevent.h>
#include <llsdk/inc/memory/memory.h>

namespace text {

class ICharacterReader {
public:
	/// @brief Tries to read a character from a buffer
	/// @param address Address of the buffer
	/// @param size Remainging size in the buffer
	/// @param malformed true if no valid character could be found, false on success.
	/// @return a character code
	virtual mobilesdk_char read(const unsigned char*& address, unsigned long& size, bool& malformed) const = 0;
	virtual TEncoding encoding() const = 0;
};

/** @brief Text reader
  */
class CTextReader: public msdk::CTask {
public:
	class ITextReaderAsyncListener {
	public:
		enum TReturnCode {
			TR_NOERROR,
			TR_ENDOFFILE,
			TR_STOPPED,
		};
	public:
		virtual ~ITextReaderAsyncListener() { }
		virtual bool OnAsyncCharacters(const mobilesdk_char* buffer, unsigned long length, int returncode) = 0;
	};
private:
	CTextReader(const msdk::shared_ptr<msdk::IInputStream>& in, TEncoding encoding, bool transactions);
public:
	/// Creates a CTextReader with the input stream in.
	/// @param in			Input stream whom this stream will feed upon
	/// @param encoding		Input encoding.
	/// @note	CTextReader objects constructed with this constructor do not support transactions.
	/// @note	If encoding is ENCODING_AUTODETECT, the stream will try to guess its encoding as it is read:
	///			- If a BOM is present, the stream will use either utf-16 or utf-8.
	///			- If no BOM is present, the stream temporarily uses us-ascii until we get an invalid character.
	///			- On any subsequent malformed character:
	///				- If encoding is us-ascii, it switches to latin1
	///				- If encoding is utf-16, it switches to ucs2
	///	@note	If no BOM is present, the CTextReader object cannot automatically detect ucs-2, utf-16, or utf-8.
	/// @note	Windows-1252 and Latin9 cannot be autodetected.
	/// @note encoding() result might change while reading, as the stream adapts itself to a more fit encoding
	CTextReader(const msdk::shared_ptr<msdk::IInputStream>& in, TEncoding encoding=ENCODING_AUTODETECT);

	// Creates a CTextReader object that support transactions.
	static CTextReader* NewTextReader(const msdk::shared_ptr<msdk::IInputStream>& in, TEncoding encoding=ENCODING_AUTODETECT);
	virtual ~CTextReader();
	operator bool() const { return m_bConstructed; }

	/// Returns the character encoding for this stream
	TEncoding	encoding() const;
	void setEncoding(TEncoding);

	/// Read a character from this stream
	bool		readCharacter(mobilesdk_char& c);

	/// Read a line from this stream
	/// @note Any leading line break or paragraph separator is removed from the result.
	bool		readLine(msdk::CString& s);

	/// Fill an array with characters
	/// @return Amount of read characters
	unsigned long	readCharacters(mobilesdk_char* b, unsigned long off, unsigned long len);
	inline unsigned long readCharacters(mobilesdk_char* b, unsigned long len) {
		return readCharacters(b,0,len);
	}

	bool startTransaction(ITextReaderAsyncListener* listener);
	void stopTransaction(bool silent=true);

protected:
	bool construct(const msdk::shared_ptr<msdk::IInputStream>& in);
	CMobileSDKError onEvent(msdk::EventPointerT);
private:
	bool									m_bConstructed;
	bool									m_bForceEncoding;
	bool									m_bTransactionsAllowed;
	TEncoding								m_eEncoding;
	msdk::shared_ptr<ICharacterReader>		m_pReader;
	msdk::shared_ptr<msdk::IInputStream>	m_pIn;
	unsigned long							m_ulSerial;

	// Transaction
	msdk::vector<mobilesdk_char>			m_pAvailable;
	msdk::EventPointerT						m_pAsyncLoop;
	ITextReaderAsyncListener*				m_pAsyncListener;
};

class CharacterReaderFactory: public msdk::CService {
public:
	static msdk::shared_ptr<ICharacterReader> getReader(TEncoding);
	static unsigned long getUnique();
	static CharacterReaderFactory* GetInstance();
	virtual ~CharacterReaderFactory() { }
private:
	CharacterReaderFactory();
	bool Construct();
private:
	msdk::vector<msdk::shared_ptr<ICharacterReader> >	m_pReaders;
	unsigned long										m_ulUnique;
};

} // namespace text

#include <llsdk/inc/memory/memory_end.h>
#endif // TEXTREADER_INCLUDED
