// $Id: dom_text.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/dom/dom_text.h>
#include <llsdk/inc/dom/dom_element.h>
#include <llsdk/inc/dom/dom_document.h>
#include <llsdk/inc/text/unicode.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

TextImpl::TextImpl(const DOMString& data)
: NodeImpl("#text",TEXT_NODE)
{
	m_oValue=data;
}
Node TextImpl::cloneNode(bool) const {
	if (ownerDocument()) {
		Node newText = ownerDocument()->createTextNode(m_oValue);
		return newText;
	}
	return Node();
}
void TextImpl::Output(DOMString& output) const {
	DOMString v = wholeText();
	EncodeEntities(v,false);
	output+=v;
}

Text TextImpl::replaceWholeText(const DOMString& content) {
	Node sibling = nextSibling();
	while (sibling && sibling->nodeType()==TEXT_NODE) {
		parentNode()->removeChild(sibling);
		sibling=nextSibling();
	}
	m_oValue = content;
	return Text(_myself(), msdk::static_cast_tag());
}

const DOMString& TextImpl::wholeText() const {
	Node sibling	= nextSibling();
	if (!(sibling && sibling->nodeType()==TEXT_NODE)) return nodeValue();

	Node self		= _myself();
	TextImpl* sibling_ptr	= reinterpret_cast<TextImpl*>(sibling.get());
	TextImpl* this_ptr		= reinterpret_cast<TextImpl*>(self.get());

	this_ptr->m_oTempString = nodeValue()+sibling_ptr->wholeText();
	sibling_ptr->m_oTempString.Clear();

	return m_oTempString;
}

bool TextImpl::isWhitespaceOnly() const {
	unsigned long nvl=nodeValue().Length();
	for (unsigned long c=0; c<nvl; ++c)
		if (!text::IsWhiteSpace(nodeValue().Get()[c])) return false;
	return true;
}

bool TextImpl::isElementContentWhitespace() const {
	return isWhitespaceOnly();
}

Text TextImpl::splitText(unsigned long offset) {
#pragma message(TODO("TextImpl::splitText() is not implemented yet"))
	return Text();
}

} // namespace dom
