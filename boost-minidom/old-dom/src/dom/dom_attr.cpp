// $Id: dom_attr.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/dom/dom_attr.h>
#include <llsdk/inc/dom/dom_element.h>
#include <llsdk/inc/dom/dom_document.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {
	
AttrImpl::AttrImpl(const DOMString& name)
: NodeImpl(name,ATTRIBUTE_NODE)
{ }

Node AttrImpl::cloneNode(bool) const {
	if (ownerDocument()) {
		Node newAttr = ownerDocument()->createAttribute(name());
		AttrImpl* newImpl = reinterpret_cast<AttrImpl*>(newAttr.get());
		newImpl->setValue(value());
		return newAttr;
	}
	return Node();
}
void AttrImpl::Output(DOMString& output) const {
	DOMString v = value();
	EncodeEntities(v,true);
	output+=DOMString(" ")+name()+DOMString("=\"")+v+DOMString("\"");
}

Element AttrImpl::ownerElement() const {
	Node parent = parentNode();
	while (parent) {
		if (parent->nodeType()==ELEMENT_NODE) return Element(parent, msdk::static_cast_tag());
		parent = parent->parentNode();
	}
	return Element();
}

} // namespace dom
