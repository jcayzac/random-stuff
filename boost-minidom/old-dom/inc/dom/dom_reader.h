// $Id: dom_reader.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(DOM_READER_INCLUDED)
#define DOM_READER_INCLUDED

#include <llsdk/inc/common/cscopeptr.h>
#include <llsdk/inc/common/cstring.h>
#include <llsdk/inc/common/tlist.h>
#include <llsdk/inc/scheduler/ctask.h>
#include <llsdk/inc/scheduler/cevent.h>
#include <llsdk/inc/text/encoding.h>
#include <llsdk/inc/text/textreader.h>
#include <llsdk/inc/dom/dom_document.h>
#include <llsdk/inc/dom/dom_node.h>

#include <llsdk/inc/memory/memory.h>

namespace dom {

class CDOMReader
: public text::CTextReader::ITextReaderAsyncListener {

	// -- Reader construction/destruction -----------------------------------
public:
	CDOMReader(const msdk::shared_ptr<msdk::IInputStream>& in, text::TEncoding encoding=text::ENCODING_AUTODETECT);
	virtual ~CDOMReader();
	operator bool() const { return m_bConstructed; }
private:
	bool construct(const msdk::shared_ptr<msdk::IInputStream>& in, text::TEncoding encoding);
private:
	bool m_bConstructed;
	// ----------------------------------------------------------------------


	// -- Synchronous reading -----------------------------------------------
public:
	Document readDocument();
	// ----------------------------------------------------------------------


	// -- Transactions (Asynchonous reading) --------------------------------
public:
	class IDOMReaderAsyncListener {
	public:
		enum TReturnCode {
			TR_NOERROR,
			TR_STOPPED,
		};
	public:
		virtual ~IDOMReaderAsyncListener() { }
		virtual void OnDOMDocumentComplete(const Document& document, int returncode) = 0;
	};
public:
	bool startTransaction(IDOMReaderAsyncListener* listener);
	void stopTransaction(bool silent=true);
private:
	bool OnAsyncCharacters(const mobilesdk_char* buffer, unsigned long length, int returncode);
private:
	IDOMReaderAsyncListener*		m_pAsyncListener;
	// ----------------------------------------------------------------------


	// -- Error management --------------------------------------------------
public:
	bool nextWarning(msdk::CString& warning)	{ return next(m_oWarnings,	warning); }
	bool nextError(msdk::CString& errors)		{ return next(m_oErrors,	errors);  }
private:
	bool next(msdk::list<msdk::CString>& l, msdk::CString& m);
private:
	msdk::list<msdk::CString>		m_oWarnings;
	msdk::list<msdk::CString>		m_oErrors;
	// ----------------------------------------------------------------------


	// -- Parsing -----------------------------------------------------------
private:
	bool	parseTextBuffer();
	bool	ParseAngleBracketBlock(msdk::vector<mobilesdk_char>& data);
	bool	ParseNewHeader(mobilesdk_char* data, unsigned long length);
	bool	ParseNewDTDElement(mobilesdk_char* data, unsigned long length);
	bool	ParseNewTag(mobilesdk_char* data, unsigned long length);
	bool	ParseNewElement(mobilesdk_char* data, unsigned long length);
	bool	ParseAttributes(mobilesdk_char* data, unsigned long length);
	bool	CloseTag(mobilesdk_char* data, unsigned long length);
	void	RedecodeTextBuffer(const msdk::CString& encodingName);
	bool	FatalError(const char*);
	bool	MemoryError();
private:
	unsigned long					m_ulCurrentLineNumber;
	msdk::vector<mobilesdk_char>	m_pTextDocument;
	// ----------------------------------------------------------------------


private:
	msdk::CScopePtr<text::CTextReader>	m_oTextReader;
	unsigned long						m_bInValue:1;
	msdk::list<Node>					m_oNodeStack;
	Document							m_oDocument;
};

} // namespace dom

#include <llsdk/inc/memory/memory_end.h>
#endif // DOM_READER_INCLUDED
