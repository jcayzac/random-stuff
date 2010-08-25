// $Id: dom_node.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/dom/dom_node.h>
#include <llsdk/inc/dom/dom_element.h>
#include <llsdk/inc/dom/dom_document.h>
#include <llsdk/inc/dom/dom.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

// static
void NodeImpl::EncodeEntities(DOMString& value, bool /*quoted*/) {
	DOMString tmp;
	mobilesdk_char v[2]; v[1]=0;
	for(unsigned long c=0; c<value.Length(); ++c) {
		v[0] = value[c];
		switch(v[0]) {
		case 0x003c:	tmp+=DOMString("&lt;");		continue;
		case 0x003e:	tmp+=DOMString("&gt;");		continue;
		case 0x0026:	tmp+=DOMString("&amp;");	continue;
		case 0x0022:	tmp+=DOMString("&quot;");	continue;
		default:		tmp+=DOMString(v);			continue;
		}
	}
	value.Swap(tmp);
}


NodeImpl::NodeImpl(const DOMString& name, TNodeType type)
: m_eType(type)
, m_oName(name)
{
	// Try to deduce a prefix and a localname
	msdk::vector<DOMString> nameParts = name.Explode(L':');
	if (nameParts.size()<2)
		m_oLocalName=m_oName;
	else {
		m_oPrefix		= nameParts[0];
		m_oLocalName	= nameParts[1];
	}
}

bool operator==(const NodeImpl& a, const NodeImpl& b) {
	return (&a==&b);
}
bool operator!=(const NodeImpl& a, const NodeImpl& b) {
	return (&a!=&b);
}

bool NodeImpl::isSameNode(const Node& n) const {
	return (n.get()==this);
}

bool NodeImpl::isEqualNode(const Node& n) const {
	if (isSameNode(n)) return true;
	if (	(nodeType()!=n->nodeType()) ||
			(nodeName()!=n->nodeName()) ||
			(localName()!=n->localName()) ||
			(namespaceURI()!=n->namespaceURI()) ||
			(prefix()!=n->prefix()) ||
			(nodeValue()!=n->nodeValue()))
		return false;
	Node child		= attributes();
	Node otherChild	= n->attributes();
	bool childrenOK;
	while (true==(childrenOK=(child.get() && child.get()==otherChild.get() && child->isEqualNode(otherChild)))) {
		child		= child->nextSibling();
		otherChild	= otherChild->nextSibling();
	}
	if (!childrenOK) return false;
	child		= firstChild();
	otherChild	= n->firstChild();
	while (true==(childrenOK=(child.get() && child.get()==otherChild.get() && child->isEqualNode(otherChild)))) {
		child		= child->nextSibling();
		otherChild	= otherChild->nextSibling();
	}
	if (!childrenOK) return false;
	return false;
}

Node NodeImpl::_myself() const {
	return m_pSelf.lock();
}
void NodeImpl::_setParent(const Node& parent) {
	if (nodeType()==DOCUMENT_NODE) return;
	m_pParent = parent;
	if (parent && parent->ownerDocument())
		_setOwnerDocument(parent->ownerDocument());
}
void NodeImpl::_setOwnerDocument(const Document& document) {
	m_pOwnerDocument = document;
}

const DOMString& NodeImpl::namespaceURI() const {
	return lookupNamespaceURI(prefix());
}

const DOMString& NodeImpl::lookupPrefix(const DOMString& namespaceURI) const {
	if (nodeType()==ELEMENT_NODE) {
		const ElementImpl* impl = reinterpret_cast<const ElementImpl*>(this);
		const NamespaceInfo* info = impl->_getNamespaceInfoByURI(namespaceURI);
		if (info) return info->prefix;
	}
	Node parent = parentNode();
	while (parent) {
		if (parent->nodeType()==ELEMENT_NODE)
			return parent->lookupPrefix(namespaceURI);
		parent = parent->parentNode();
	}
	return m_oEmpty;
}

bool NodeImpl::isDefaultNamespace(const DOMString& namespaceURI) const {
	const DOMString& defaultURI = lookupNamespaceURI("");
	return (defaultURI==namespaceURI);
}
const DOMString& NodeImpl::lookupNamespaceURI(const DOMString& prefix) const {
	if (nodeType()==ELEMENT_NODE) {
		const ElementImpl* impl = reinterpret_cast<const ElementImpl*>(this);
		const NamespaceInfo* info = impl->_getNamespaceInfoByPrefix(prefix);
		if (info) return info->uri;
	}
	Node parent = parentNode();
	while (parent) {
		if (parent->nodeType()==ELEMENT_NODE)
			return parent->lookupPrefix(prefix);
		parent = parent->parentNode();
	}
	return m_oEmpty;
}

unsigned long NodeImpl::nodePosition() const {
	if (!parentNode()) return 0;
	unsigned long position=1;
	Node previous = previousSibling();
	while(previous) {
		++position;
		previous = previous->previousSibling();
	}
	return position;
}

void NodeImpl::debug(DOMString& output) const {
	if (nodeType()==DOCUMENT_NODE)
		output+="document:/";
	else {
		Node parent = parentNode();
		if (parent) {
			parent->debug(output);
			output+="/";
		}
		else {
			output+="orphans://";
		}
		unsigned long position = nodePosition();
		if (position) {
			output+="|index=";
			output+=position;
		}
		output+="|name=";
		output+=nodeName();
		output+="|";
	}
}

// Supports walking a list of nodes from the first element:
Node NodeImpl::operator[](int i) {
	if (i>0) {
		if (nextSibling())
			return (*nextSibling())[--i];
		else
			return Node();
	}
	else if (i==0) return _myself();
	else return Node();
}

// Convertion of content
bool operator>>(const Node& node,DOMString& result) {
	return node->textContent(result);
}
bool operator>>(const Node& node,unsigned short& result) {
	DOMString tmp;
	if (node->textContent(tmp)) {
		unsigned long ul = tmp.ToULong();
		if (ul<65536) {
			result=(unsigned short)ul;
			return true;
		}
	}
	return false;
}
bool operator>>(const Node& node,unsigned long& result) {
	DOMString tmp;
	if (node->textContent(tmp)) {
		result=tmp.ToULong();
		return true;
	}
	return false;
}
bool operator>>(const Node& node,short& result) {
	DOMString tmp;
	if (node->textContent(tmp)) {
		long l = tmp.ToLong();
		if (l>-32769 && l<32768) {
			result=(short)l;
			return true;
		}
	}
	return false;
}
bool operator>>(const Node& node,long& result) {
	DOMString tmp;
	if (node->textContent(tmp)) {
		result=tmp.ToLong();
		return true;
	}
	return false;
}
bool operator>>(const Node& node,float& result) {
	DOMString tmp;
	if (node->textContent(tmp)) {
		bool ok=false;
		float f=tmp.ToFloat(&ok);
		if (ok) {
			result=f;
			return true;
		}
	}
	return false;
}
bool operator>>(const Node& node,bool& result) {
	DOMString tmp;
	if (node->textContent(tmp)) {
		tmp.ToLowerCase();
		if (tmp=="yes" || tmp=="true") {
			result=true;
		}
		else if (tmp=="no" || tmp=="false") {
			result=false;
		}
		else {
			result=(tmp.ToLong()!=0);
		}
		return true;
	}
	return false;
}

} // namespace dom
