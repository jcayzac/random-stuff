// $Id: textreader.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/text/textreader_ascii.h>
#include <llsdk/inc/text/textreader_latin1.h>
#include <llsdk/inc/text/textreader_latin9.h>
#include <llsdk/inc/text/textreader_utf8.h>
#include <llsdk/inc/text/textreader_windows1252.h>
#include <llsdk/inc/text/textreader_ucs2.h>
#include <llsdk/inc/text/textreader_utf16.h>
#include <llsdk/inc/text/unicode.h>
#include <llsdk/inc/streams/bufferedinputstream.h>
#include <llsdk/inc/scheduler/cscheduler.h>
#include <llsdk/inc/memory/memory.h>

namespace text {

static const unsigned long  REQUESTED_BUFFER_SIZE = 4;

CTextReader::CTextReader(const msdk::shared_ptr<msdk::IInputStream>& in, TEncoding encoding, bool transactions)
: m_bConstructed(false)
, m_bForceEncoding(false)
, m_bTransactionsAllowed(transactions)
, m_eEncoding(encoding)
, m_pAsyncListener(0)
{
	m_bConstructed = construct(in);
}

CTextReader::CTextReader(const msdk::shared_ptr<msdk::IInputStream>& in, TEncoding encoding)
: m_bConstructed(false)
, m_bForceEncoding(false)
, m_bTransactionsAllowed(false)
, m_eEncoding(encoding)
, m_pAsyncListener(0)
{
	m_bConstructed = construct(in);
}

CTextReader* CTextReader::NewTextReader(const msdk::shared_ptr<msdk::IInputStream>& in, TEncoding encoding) {
	CTextReader* self = new CTextReader(in,encoding,true);
	if (self) {
		if (!self->m_bConstructed) {
			delete self;
			self=0;
		}
	}
	return self;
}


bool CTextReader::construct(const msdk::shared_ptr<msdk::IInputStream>& in) {
	// 
	if (m_eEncoding!=ENCODING_AUTODETECT) {
		m_bForceEncoding=true;
		setEncoding(m_eEncoding);
		if (!m_pReader) return false;
	}

	// Set the input stream
	if (in && in->lockSupported(REQUESTED_BUFFER_SIZE))
		m_pIn=in;
	else
		m_pIn.reset(new msdk::CBufferedInputStream(in,REQUESTED_BUFFER_SIZE,REQUESTED_BUFFER_SIZE));
	if (!m_pIn) return false;
	if (in->eof()) return false;

	if (!m_bTransactionsAllowed) return true;

	// Create a unique name
	m_ulSerial = CharacterReaderFactory::getUnique();
	if (!m_ulSerial) return false;
	setName(msdk::CString("/tasks/text readers/") + m_ulSerial);

	m_pAsyncLoop.reset(new msdk::CContinueEvent());
	if (!m_pAsyncLoop) return false;

	return (MobileSDKError_NoError==msdk::CScheduler::sAddTask(this));
}
CTextReader::~CTextReader()
{
	stopTransaction(false);
}

text::TEncoding CTextReader::encoding() const {
	if (!m_pReader) return ENCODING_UNKNOWN;
	return m_eEncoding;
}

void CTextReader::setEncoding(TEncoding e) {
	m_eEncoding=e;
	m_bForceEncoding=true;
	m_pReader = CharacterReaderFactory::getReader(m_eEncoding);
}

bool CTextReader::readCharacter(mobilesdk_char& c) {
	c=0;
	if (!m_bConstructed) return false;

	// Try to always lock four bytes ahead...
	const unsigned char* inputBuffer=0;
	unsigned long lockedBytes=REQUESTED_BUFFER_SIZE;
	m_pIn->lock(inputBuffer,lockedBytes);
	unsigned long skipBytes=0;

	if (!m_bForceEncoding) {
		if (m_eEncoding==ENCODING_AUTODETECT) {
			if (lockedBytes>=2) {
				// Try to detect a 16-bit BOM
				if (inputBuffer[0]==0xff && inputBuffer[1]==0xfe) {
					skipBytes=2;
					setEncoding(ENCODING_UTF16LE);
				}
				else if (inputBuffer[0]==0xfe && inputBuffer[1]==0xff) {
					skipBytes=2;
					setEncoding(ENCODING_UTF16BE);
				}
				else if (lockedBytes>=3) {
					if (inputBuffer[0]==0xef && inputBuffer[1]==0xbb && inputBuffer[2]==0xbf) {
						skipBytes=3;
						setEncoding(ENCODING_UTF8);
					}
					else {
						setEncoding(ENCODING_US_ASCII);
					}
				}
			}
		}
	}
	m_pIn->unlock();
	if (!m_pReader) return false;

	if (skipBytes)
		m_pIn->skip(skipBytes);

	// Relock buffer
	lockedBytes=REQUESTED_BUFFER_SIZE;
	m_pIn->lock(inputBuffer,lockedBytes);
	unsigned long remainingBytes=lockedBytes;

	bool malformed=false;
	mobilesdk_char result = m_pReader->read(inputBuffer, remainingBytes, malformed);
	m_pIn->unlock();

	// Change encoding if this one is not good.
	if (malformed) {
		if (m_bForceEncoding) {
			return false;
		}
		else {
			// US-ASCII -> LATIN1
			if (encoding()==ENCODING_US_ASCII && result>0x007f) {
				setEncoding(ENCODING_LATIN1);
				return readCharacter(c);
			}
			// UTF-16 -> UCS2
			else if (encoding()==ENCODING_UTF16LE) {
				setEncoding(ENCODING_UCS2LE);
				return readCharacter(c);
			}
			else if (encoding()==ENCODING_UTF16BE) {
				setEncoding(ENCODING_UCS2BE);
				return readCharacter(c);
			}
			else {
				// ???
				return false;
			}
		}
	}

	unsigned long characterSizeInBytes = lockedBytes-remainingBytes;
	if (characterSizeInBytes)
		m_pIn->skip(characterSizeInBytes);

	if (result && !malformed) {
		// Save result
		if (m_pAsyncListener)
			m_pAvailable.push_back(result);
		// Return
		c=result;
		return true;
	}
	return false;
}
bool CTextReader::readLine(msdk::CString& s) {
	s="";
	if (!m_bConstructed) return false;
	bool result=false;

	mobilesdk_char c[2]; c[1]=0;
	while(readCharacter(c[0])) {
		result=true;
		if (IsLineBreakingSpace(c[0])) break;
		s+=c;
	}
	return result;
}
unsigned long CTextReader::readCharacters(mobilesdk_char* b, unsigned long off, unsigned long len) {
	if (!m_bConstructed) return 0;

	mobilesdk_char c;
	unsigned long count=0;
	while (count<len) {
		if (!readCharacter(c)) break;
		b[off+count]=c;
		++count;
	}
	return count;
}
bool CTextReader::startTransaction(CTextReader::ITextReaderAsyncListener* listener) {
	if (!m_bConstructed) return false;
	if (!m_bTransactionsAllowed) return false;
	if (m_pAsyncListener && (m_pAsyncListener!=listener))
		return false;
	m_pAsyncListener = listener;
	return (MobileSDKError_NoError==sendEvent(this, m_pAsyncLoop));
}
void CTextReader::stopTransaction(bool silent) {
	if (!m_bConstructed) return;
	if (!m_pAsyncListener) return;
	if (!silent) {
		m_pAsyncListener->OnAsyncCharacters(0,0,ITextReaderAsyncListener::TR_STOPPED);
	}
	m_pAsyncListener=0;
}

CMobileSDKError CTextReader::onEvent(msdk::EventPointerT ev) {
	if (m_pAsyncListener && (ev->getType() == msdk::CContinueEvent::Type())) {
		if (!m_pAvailable.empty()) {
			m_pAsyncListener->OnAsyncCharacters(&m_pAvailable[0], m_pAvailable.size(), ITextReaderAsyncListener::TR_NOERROR);
			m_pAvailable.clear();
			sendEvent(this, m_pAsyncLoop);
		}
		else if (m_pIn->eof()) {
			m_pAsyncListener->OnAsyncCharacters(0, 0, ITextReaderAsyncListener::TR_ENDOFFILE);
			stopTransaction(false);
		}
		else sendEvent(this, m_pAsyncLoop);
	}
	return MobileSDKError_NoError;
}

// --------------------------------

CharacterReaderFactory::CharacterReaderFactory()
: msdk::CService("/services/character reader factory")
, m_ulUnique(1)
{ }

bool CharacterReaderFactory::Construct() {
	unsigned long count = ENCODING_LAST+1;
	if (m_pReaders.resize(count, msdk::shared_ptr<ICharacterReader>()))
		return false;
	// m_pReaders[ENCODING_UNKNOWN] = null
	// m_pReaders[ENCODING_AUTODETECT] = null
	m_pReaders[ENCODING_US_ASCII].reset(new TUSAsciiReader);
	m_pReaders[ENCODING_LATIN1].reset(new TLatin1Reader);
	m_pReaders[ENCODING_LATIN9].reset(new TLatin9Reader);
	m_pReaders[ENCODING_UTF8].reset(new TUtf8Reader);
	m_pReaders[ENCODING_WINDOWS_1252].reset(new TWindows1252Reader);
	m_pReaders[ENCODING_UCS2LE].reset(new TUcs2Reader(true));
	m_pReaders[ENCODING_UCS2BE].reset(new TUcs2Reader(false));
	m_pReaders[ENCODING_UTF16LE].reset(new TUtf16Reader(true));
	m_pReaders[ENCODING_UTF16BE].reset(new TUtf16Reader(false));
	for (unsigned long c = 0; c < count; ++c) {
		if (c==ENCODING_UNKNOWN || c==ENCODING_AUTODETECT) continue;
		if (!m_pReaders[c]) return false;
	}
	return (MobileSDKError_NoError==msdk::CScheduler::sAddService(this));
}

msdk::shared_ptr<ICharacterReader> CharacterReaderFactory::getReader(TEncoding e) {
	CharacterReaderFactory* self = GetInstance();
	if (self) return self->m_pReaders[e];
	return msdk::shared_ptr<ICharacterReader>();
}
unsigned long CharacterReaderFactory::getUnique() {
	CharacterReaderFactory* self = GetInstance();
	if (!self) return 0;
	return ++self->m_ulUnique;
}
CharacterReaderFactory* CharacterReaderFactory::GetInstance() {
	CharacterReaderFactory* self = static_cast<CharacterReaderFactory*>(msdk::CScheduler::sGetService("/services/character reader factory"));
	if (self) return self;
	self = new CharacterReaderFactory;
	if (self && !self->Construct()) {
		delete self;
		self=0;
	}
	return self;
}

} // namespace text

