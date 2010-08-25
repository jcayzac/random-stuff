// $Id: dom_element.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/dom/dom_element.h>
#include <llsdk/inc/dom/dom_attr.h>
#include <llsdk/inc/dom/dom_document.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

ElementImpl::ElementImpl(const DOMString& name)
: NodeImpl(name, ELEMENT_NODE)
{
}
ElementImpl::~ElementImpl() {
	// TODO: Detach all child nodes
}
void ElementImpl::Output(DOMString& output) const {
	output+=DOMString("<");
	// Tag
	output+=tagName();
	// Namespaces
	msdk::vector<NamespaceInfo>::const_iterator it = m_oNamespacesInfo.begin();
	while(it!=m_oNamespacesInfo.end()) {
		const NamespaceInfo& info = *it; ++it;
		output+=DOMString(" xmlns");
		if (!info.prefix.IsEmpty())
			output+=DOMString(":")+info.prefix;
		output+=DOMString("=\"")+info.uri+DOMString("\"");
	}

	// Attributes
	Node child = m_pFirstAttribute;
	while (child) {
		child->Output(output);
		child = child->nextSibling();
	}
	// Child
	child = m_pFirstChild;
	if (hasChildNodes()) {
		if (child==m_pLastChild && child->nodeType()==NodeImpl::TEXT_NODE)
			output+=DOMString(">");
		else
			output+=DOMString(">\n");
		while (child) {
			child->Output(output);
			child = child->nextSibling();
		}
		output+=DOMString("</");
		output+=tagName();
		output+=DOMString(">\n");
	}
	else {
		output+=DOMString(" />\n");
	}
}

//
Node ElementImpl::getAttribute(const DOMString& name) const {
	if (!hasAttributes()) return Node();
	Node child = m_pFirstAttribute;
	while (child) {
		if (child->nodeName()==name) return child;
		child = child->nextSibling();
	}
	return Node();
}
void ElementImpl::setAttribute(const DOMString& name, const DOMString& value) {
	msdk::vector<DOMString> attrNameParts = name.Explode(L':');
	if (attrNameParts.size()==2) {
		if (attrNameParts[0]==msdk_char("xmlns")) {
			// Attribute is a XML Namespace declaration
			NamespaceInfo info;
			info.prefix	= attrNameParts[1];
			info.uri	= value;
			m_oNamespacesInfo.push_back(info);
			return;
		}
	}
	if (name==msdk_char("xmlns")) {
		// Default namespace
			NamespaceInfo info;
			info.uri	= value;
			m_oNamespacesInfo.push_back(info);
			return;
	}

	Node attr = ownerDocument()->createAttribute(name);
	reinterpret_cast<AttrImpl*>(attr.get())->setValue(value);
	appendChild(attr);
}

Node ElementImpl::insertBefore(const Node& newChild, const Node& refChild) {
	if (!newChild || !refChild)
		return Node();
	// Cannot insert Node if it already has a parent.
	if (newChild->parentNode())
		return Node();
	// Cannot insert Node if refChild parent is not this Node.
	if (!isSameNode(refChild->parentNode()))
		return Node();
	// Cannot insert an Attr before a non Attr or a non Attr before an Attr
	if ((newChild->nodeType()==ATTRIBUTE_NODE && refChild->nodeType()!=ATTRIBUTE_NODE) ||
		(newChild->nodeType()!=ATTRIBUTE_NODE && refChild->nodeType()==ATTRIBUTE_NODE))
		return Node();

	newChild->_setParent(_myself());
	//
	Node previous	= refChild->previousSibling();
	if (previous) {
		newChild->_setPreviousSibling(previous);
		previous->_setNextSibling(newChild);
	}
	else if (newChild->nodeType()==ATTRIBUTE_NODE)
		m_pFirstAttribute = newChild;
	else
		m_pFirstChild = newChild;
	//
	newChild->_setNextSibling(refChild);
	refChild->_setPreviousSibling(newChild);
	return newChild;
}
Node ElementImpl::replaceChild(const Node& newChild, const Node& oldChild) {
	if (!newChild || !oldChild)
		return Node();
	// Cannot insert Node if it already has a parent.
	if (newChild->parentNode())
		return Node();
	// Cannot insert Node if refChild parent is not this Node.
	if (!isSameNode(oldChild->parentNode()))
		return Node();
	// Cannot replace an Attr with a non Attr or a non Attr with an Attr
	if ((newChild->nodeType()==ATTRIBUTE_NODE && oldChild->nodeType()!=ATTRIBUTE_NODE) ||
		(newChild->nodeType()!=ATTRIBUTE_NODE && oldChild->nodeType()==ATTRIBUTE_NODE))
		return Node();
	//
	newChild->_setParent(_myself());
	newChild->_setOwnerDocument(ownerDocument());
	//
	Node previous = oldChild->previousSibling();
	if (previous) {
		newChild->_setPreviousSibling(previous);
		previous->_setNextSibling(newChild);
	}
	else if (newChild->nodeType()==ATTRIBUTE_NODE)
		m_pFirstAttribute = newChild;
	else
		m_pFirstChild = newChild;
	//
	Node next = oldChild->nextSibling();
	if (next) {
		newChild->_setNextSibling(next);
		next->_setPreviousSibling(newChild);
	}
	else if (newChild->nodeType()==ATTRIBUTE_NODE)
		m_pLastAttribute = newChild;
	else
		m_pLastChild = newChild;
	//
	return newChild;
}

Node ElementImpl::appendChild(const Node& n) {
	if (!n)
		return Node();
	// Cannot append Node if it already has a parent.
	if (n->parentNode())
		return Node();
	//
	n->_setParent(_myself());
	n->_setOwnerDocument(ownerDocument());
	//
	Node last = (n->nodeType()==ATTRIBUTE_NODE)?m_pLastAttribute:m_pLastChild;
	if (last)
		last->_setNextSibling(n);
	n->_setPreviousSibling(last);
	if (n->nodeType()==ATTRIBUTE_NODE) {
		m_pLastAttribute=n;
		if (!m_pFirstAttribute) m_pFirstAttribute=n;
	}
	else {
		m_pLastChild=n;
		if (!m_pFirstChild) m_pFirstChild=n;
	}

	return n;
}
Node ElementImpl::removeChild(const Node& n) {
	if (!n)
		return Node();
	// Cannot remove a Node if we don't own it.
	if (n->parentNode()!=_myself())
		return Node();

	Node& first = (n->nodeType()==ATTRIBUTE_NODE)?m_pFirstChild:m_pLastChild;
	Node& last  = (n->nodeType()==ATTRIBUTE_NODE)?m_pLastAttribute:m_pLastChild;

	if (n->isSameNode(first))
		first=n->nextSibling();
	if (n->isSameNode(last))
		last=n->previousSibling();
	if (n->previousSibling())
		n->previousSibling()->_setNextSibling(n->nextSibling());
	if (n->nextSibling())
		n->nextSibling()->_setPreviousSibling(n->previousSibling());
	n->_setParent(Node());
	return n;
}
bool ElementImpl::hasChildNodes() const {
	return (m_pFirstChild.get()!=0);
}
bool ElementImpl::hasAttributes() const {
	return (m_pFirstAttribute.get()!=0);
}

Node ElementImpl::cloneNode(bool deep) const {
	if (ownerDocument()) {
		Node newElement = ownerDocument()->createElement(tagName());
		ElementImpl* newImpl = reinterpret_cast<ElementImpl*>(newElement.get());
		if (!newImpl) return Node();

		// Clone attributes
		Node child = m_pFirstAttribute;
		while(child) {
			AttrImpl* attr = reinterpret_cast<AttrImpl*>(child.get());
			newImpl->setAttribute(attr->name(), attr->value());
			child = child->nextSibling();
		}

		if (deep) {
			// Clone children
			child = m_pFirstChild;
			while(child) {
				newImpl->appendChild(child->cloneNode(deep));
				child = child->nextSibling();
			}
		}
		return newElement;
	}
	return Node();
}

bool ElementImpl::textContent(DOMString& output) const {
	bool result=false;
	Node child = m_pFirstChild;
	while (child) {
		DOMString tmp;
		bool tmpr = child->textContent(tmp);
		if (tmpr) {
			result=true;
			output+=tmp;
		}
		child=child->nextSibling();
	}
	return result;
}

void ElementImpl::setTextContent(const DOMString& newContent) {
	if (nodeType()==DOCUMENT_NODE) return;
	// Remove all child nodes
	while (m_pFirstChild) removeChild(m_pFirstChild);
	// Create and append a new child with the requested content
	Node textNode = ownerDocument()->createTextNode(newContent);
	if (textNode) appendChild(textNode);
}

Node ElementImpl::getElementById(const DOMString& id) const {
	Node child = m_pFirstChild;
	while (child) {
		if (child->nodeType()==ELEMENT_NODE) {
			Node attr = child->getAttribute("id");
			if (attr && attr->nodeValue()==id) return child;
		}
		child=child->nextSibling();
	}
	return Node();
}
NodeList ElementImpl::getElementsByTagName(const DOMString& name) const {
	NodeList output(new NodeListImpl);
	if (output) _addElementsByTagName(output,name);
	return output;
}
void ElementImpl::_addElementsByTagName(NodeList& output, const DOMString& name) const {
	Node child = m_pFirstChild;
	while (child) {
		if (child->nodeType()==ELEMENT_NODE && child->nodeName()==name) {
			output->push_back(child);
			reinterpret_cast<ElementImpl*>(child.get())->_addElementsByTagName(output,name);
		}
		child=child->nextSibling();
	}
}
void ElementImpl::_setOwnerDocument(const Document& document) {
	NodeImpl::_setOwnerDocument(document);
	Node child = m_pFirstAttribute;
	while (child) {
		child->_setOwnerDocument(ownerDocument());
		child = child->nextSibling();
	}
	child = m_pFirstChild;
	while (child) {
		child->_setOwnerDocument(ownerDocument());
		child = child->nextSibling();
	}
}

const NamespaceInfo* ElementImpl::_getNamespaceInfoByURI(const DOMString& uri) const {
	msdk::vector<NamespaceInfo>::const_iterator it = m_oNamespacesInfo.begin();
	while(it!=m_oNamespacesInfo.end()) {
		const NamespaceInfo& info = *it; ++it;
		if (info.uri==uri) return &info;
	}
	return 0;
}

const NamespaceInfo* ElementImpl::_getNamespaceInfoByPrefix(const DOMString& prefix) const {
	msdk::vector<NamespaceInfo>::const_iterator it = m_oNamespacesInfo.begin();
	while(it!=m_oNamespacesInfo.end()) {
		const NamespaceInfo& info = *it; ++it;
		if (info.prefix==prefix) return &info;
	}
	return 0;
}

} // namespace dom
