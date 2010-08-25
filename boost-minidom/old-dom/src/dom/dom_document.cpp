// $Id: dom_document.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/dom/dom_document.h>
#include <llsdk/inc/dom/dom_text.h>
#include <llsdk/inc/dom/dom_attr.h>
#include <llsdk/inc/dom/dom_element.h>
#include <llsdk/inc/dom/dom.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

DocumentImpl::DocumentImpl()
: ElementImpl("#document")
, m_eEncoding(text::ENCODING_UNKNOWN)
{
	m_eType=DOCUMENT_NODE;
}
DocumentImpl::~DocumentImpl() { }

void DocumentImpl::Output(DOMString& output) const {
	output+=DOMString("<?xml version=\"1.0\" encoding=\"");
	switch(encoding()) {
	case text::ENCODING_LATIN9:			output+=DOMString("ISO-8859-15");	break;
	case text::ENCODING_UTF8:			output+=DOMString("UTF-8");			break;
	case text::ENCODING_WINDOWS_1252:	output+=DOMString("windows-1252");	break;
	case text::ENCODING_UCS2LE:
	case text::ENCODING_UTF16LE:
	case text::ENCODING_UCS2BE:
	case text::ENCODING_UTF16BE:		output+=DOMString("UTF-16");		break;
	default:							output+=DOMString("ISO-8859-1");	break;
	}
	output+=DOMString("\"");
	if (hasAttributes()) {
		Node child = m_pFirstAttribute;
		while (child) {
			const DOMString& attrName  = child->nodeName();
			if (attrName!="encoding" && attrName!="version")
				child->Output(output);
			child = child->nextSibling();
		}
	}
	output+=DOMString("?>\n");
	if (!docType().IsEmpty()) {
		output+=DOMString("<!DOCTYPE ");
		output+=docType();
		output+=DOMString(">\n");
	}
	Node root = documentElement();
	if (root) root->Output(output);
}

Node DocumentImpl::createElement(const DOMString& name) {
	return constructNode(new ElementImpl(name));
}
Node DocumentImpl::createTextNode(const DOMString& data) {
	return constructNode(new TextImpl(data));
}
Node DocumentImpl::createAttribute(const DOMString& name) {
	return constructNode(new AttrImpl(name));
}
Node DocumentImpl::constructNode(NodeImpl* object) {
	if (object) {
		Node domObject(object);
		domObject->_setSelf(domObject);
		domObject->_setOwnerDocument(ownerDocument());
		return domObject;
	}
	return Node();
}

Node DocumentImpl::documentElement () const {
	return firstChild();
}

} // namespace dom
