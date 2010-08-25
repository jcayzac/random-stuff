// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>
#include "dom_node.hpp"
#include "dom_element.hpp"
#include "dom_utils.hpp"
using namespace dom;

Node::Node(const std::wstring& name, TNodeType type)
: m_eType(type)
, m_oName(name)
{
    // Try to deduce a prefix and a localname
    std::vector<std::wstring> nameParts;
    boost::algorithm::split(nameParts, name, boost::algorithm::is_any_of(L":"));
    if (nameParts.size()<2)
        m_oLocalName=m_oName;
    else {
        m_oPrefix    = nameParts[0];
        m_oLocalName = nameParts[1];
    }
}

bool operator==(const Node& a, const Node& b) {
    return (&a==&b);
}

bool operator!=(const Node& a, const Node& b) {
    return (&a!=&b);
}

bool Node::isSameNode(const NodePtr& n) const {
    return (n.get()==this);
}

bool Node::isEqualNode(const NodePtr& n) const {
    if (isSameNode(n)) return true;
    if (    (nodeType()!=n->nodeType()) ||
            (nodeName()!=n->nodeName()) ||
            (localName()!=n->localName()) ||
            (namespaceURI()!=n->namespaceURI()) ||
            (prefix()!=n->prefix()) ||
            (nodeValue()!=n->nodeValue()))
        return false;
    NodePtr child      = attributes();
    NodePtr otherChild = n->attributes();
    // FIXME: WTF?? childrenOK will always be false at the end of that loop!!
    bool childrenOK;
    while (true==(childrenOK=(child.get() && child.get()==otherChild.get() && child->isEqualNode(otherChild)))) {
        child      = child->nextSibling();
        otherChild = otherChild->nextSibling();
    }
    if (!childrenOK) return false;
    child      = firstChild();
    otherChild = n->firstChild();
    // FIXME: WTF?? childrenOK will always be false at the end of that loop!!
    while (true==(childrenOK=(child.get() && child.get()==otherChild.get() && child->isEqualNode(otherChild)))) {
        child        = child->nextSibling();
        otherChild    = otherChild->nextSibling();
    }
    if (!childrenOK) return false;
    return false;
}

NodePtr Node::_myself() const {
    // Return a shared pointer to itself
    return m_pSelf.lock();
}

void Node::_setParent(const NodePtr& parent) {
    if (nodeType()==DOCUMENT_NODE) return;
    m_pParent = parent;
    if (parent && parent->ownerDocument())
        _setOwnerDocument(parent->ownerDocument());
}

void Node::_setOwnerDocument(const DocumentPtr& document) {
    m_pOwnerDocument = document;
}

const std::wstring& Node::namespaceURI() const {
    return lookupNamespaceURI(prefix());
}

const std::wstring& Node::lookupPrefix(const std::wstring& namespaceURI) const {
    if (nodeType()==ELEMENT_NODE) {
        const Element* impl = reinterpret_cast<const Element*>(this);
        return impl->_getNamespaceInfoBy(&NamespaceInfo::uri, namespaceURI).prefix;
    }
    NodePtr parent = parentNode();
    while (parent) {
        if (parent->nodeType()==ELEMENT_NODE)
            return parent->lookupPrefix(namespaceURI);
        parent = parent->parentNode();
    }
    throw DOMException(DOMException::NOT_FOUND_ERR);
}

bool Node::isDefaultNamespace(const std::wstring& namespaceURI) const {
    try {
        return namespaceURI==lookupNamespaceURI(L"");
    }
    catch(const DOMException& e) {
        // If no default namespace has been registered,
        // then the default URI must be empty
        if (e.code()==DOMException::NOT_FOUND_ERR)
            return namespaceURI.empty();
        else throw;
    }
}

const std::wstring& Node::lookupNamespaceURI(const std::wstring& prefix) const {
    if (nodeType()==ELEMENT_NODE) {
        const Element* impl = reinterpret_cast<const Element*>(this);
        return impl->_getNamespaceInfoBy(&NamespaceInfo::prefix, prefix).uri;
    }
    NodePtr parent = parentNode();
    while (parent) {
        if (parent->nodeType()==ELEMENT_NODE)
            return parent->lookupNamespaceURI(prefix);
        parent = parent->parentNode();
    }
    throw DOMException(DOMException::NOT_FOUND_ERR);
}

unsigned long Node::nodePosition() const {
    if (!parentNode()) return 0;
    unsigned long position=1;
    NodePtr previous = previousSibling();
    while(previous) {
        ++position;
        previous = previous->previousSibling();
    }
    return position;
}

// Supports walking a list of nodes from the first element:
NodePtr Node::operator[](int i) {
    if (i>0) {
        if (nextSibling())
            return (*nextSibling())[--i];
        else
            return NodePtr();
    }
    else if (i==0) return _myself();
    else return NodePtr();
}
