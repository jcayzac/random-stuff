// $Id: dom_document.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(DOM_DOCUMENT_INCLUDED)
#define DOM_DOCUMENT_INCLUDED

#include <llsdk/inc/dom/dom_types.h>
#include <llsdk/inc/dom/dom_element.h>
#include <llsdk/inc/dom/dom_query.h>
#include <llsdk/inc/text/encoding.h>
#include <llsdk/inc/common/cstring.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

class DocumentImpl : public ElementImpl {
private:
	friend class DOMImplementation;
	friend class NodeImpl; // Needed for remove from orphans
	DocumentImpl();
	DocumentImpl(const DocumentImpl& o);
public:
	virtual ~DocumentImpl();
	void Output(DOMString& output) const;

	Node	createElement(const DOMString& name);
	Node	createTextNode(const DOMString& data);
	Node	createAttribute(const DOMString& name);
	Node	documentElement () const;

	const msdk::CString&	docType() const	 { return m_oDoctype; }
	text::TEncoding			encoding() const { return m_eEncoding; }

	void setDocType(const msdk::CString& dt) { m_oDoctype=dt; }
	void setEncoding(text::TEncoding e) { m_eEncoding=e; }

private:
	// Used by create*()
	Node	constructNode(NodeImpl* object);
private:
	msdk::CString	m_oDoctype;
	text::TEncoding	m_eEncoding;
};
	
} // namespace dom

#include <llsdk/inc/memory/memory_end.h>
#endif // DOM_DOCUMENT_INCLUDED
