// $Id: dom_text.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(DOM_TEXT_INCLUDED)
#define DOM_TEXT_INCLUDED

#include <llsdk/inc/dom/dom_types.h>
#include <llsdk/inc/dom/dom_node.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

class TextImpl: public NodeImpl {
private:
	TextImpl(const TextImpl&);
public:
	TextImpl(const DOMString& data);
	virtual ~TextImpl() { }
	void Output(DOMString& output) const;
	// Interface CharacterData
	DOMString&			data()			{ return m_oValue; }
	const DOMString&	data() const	{ return m_oValue; }
	unsigned long		length() const	{ return m_oValue.Length(); }
	DOMString			substringData(unsigned long offset, unsigned long count) const;
	void				appendData(const DOMString& arg);
	void				insertData(unsigned long offset, const DOMString& arg);
	void				deleteData(unsigned long offset, unsigned long count);
	void				replaceData(unsigned long offset, unsigned long count, const DOMString& arg);
	//
	Text				splitText(unsigned long offset);
	Text				replaceWholeText(const DOMString& content);
	const DOMString&	wholeText() const;
	bool				isWhitespaceOnly() const;
	bool				isElementContentWhitespace() const;

	// From Node
	Node cloneNode(bool deep=false) const;
	bool textContent(DOMString& output) const { output=m_oValue; return true; }
	void setTextContent(const DOMString& newContent) { m_oValue=newContent; }
private:
	mutable DOMString m_oTempString;
};

} // namespace dom

#include <llsdk/inc/memory/memory_end.h>
#endif // DOM_TEXT_INCLUDED
