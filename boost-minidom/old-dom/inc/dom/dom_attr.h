// $Id: dom_attr.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(DOM_ATTR_INCLUDED)
#define DOM_ATTR_INCLUDED

#include <llsdk/inc/dom/dom_types.h>
#include <llsdk/inc/dom/dom_node.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

class AttrImpl: public NodeImpl {
private:
	AttrImpl(const AttrImpl&);
public:
	AttrImpl(const DOMString& name);
	virtual ~AttrImpl() { }
	void Output(DOMString& output) const;

	const DOMString&	name() const	{ return nodeName(); }
	const DOMString&	value() const	{ return m_oValue; }
	void				setValue(const DOMString& v)	{ m_oValue=v; }
	Element				ownerElement() const;

	// From Node
	Node cloneNode(bool deep=false) const;
	bool textContent(DOMString& output) const { output=m_oValue; return true; }
	void setTextContent(const DOMString& newContent) { m_oValue=newContent; }
};

} // namespace dom

#include <llsdk/inc/memory/memory_end.h>
#endif // DOM_ATTR_INCLUDED
