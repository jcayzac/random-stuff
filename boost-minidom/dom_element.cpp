// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>
#include "dom_attr.hpp"
#include "dom_element.hpp"
#include "dom_document.hpp"
#include "dom_utils.hpp"
using namespace dom;

Element::Element(const std::wstring& name)
: Node(name, ELEMENT_NODE)
{ }

Element::~Element() {
}

NodePtr Element::getAttribute(const std::wstring& name) const {
    if (!hasAttributes()) return NodePtr();
    NodePtr child = m_pFirstAttribute;
    while (child) {
        if (child->nodeName()==name) return child;
        child = child->nextSibling();
    }
    return NodePtr();
}
void Element::setAttribute(const std::wstring& name, const std::wstring& value) {
    // FIXME: clean this method
    std::vector<std::wstring> attrNameParts;
    boost::algorithm::split(attrNameParts, name, boost::algorithm::is_any_of(L":"));
    if (attrNameParts.size()==2) {
        if (attrNameParts[0]==L"xmlns") {
            // Attribute is a XML Namespace declaration
            NamespaceInfo info;
            info.prefix    = attrNameParts[1];
            info.uri    = value;
            m_oNamespacesInfo.push_back(info);
            return;
        }
    }
    if (name==L"xmlns") {
        // Default namespace
            NamespaceInfo info;
            info.uri    = value;
            m_oNamespacesInfo.push_back(info);
            return;
    }

    NodePtr attr = ownerDocument()->createAttribute(name);
    reinterpret_cast<Attr*>(attr.get())->setValue(value);
    appendChild(attr);
}

NodePtr Element::insertBefore(const NodePtr& newChild, const NodePtr& refChild) {
    // FIXME: raise proper DOMExceptions
    if (!newChild || !refChild)
        return NodePtr();
    // Cannot insert Node if it already has a parent.
    if (newChild->parentNode())
        return NodePtr();
    // Cannot insert Node if refChild parent is not this Node.
    if (!isSameNode(refChild->parentNode()))
        return NodePtr();
    // Cannot insert an Attr before a non Attr or a non Attr before an Attr
    if ((newChild->nodeType()==ATTRIBUTE_NODE && refChild->nodeType()!=ATTRIBUTE_NODE) ||
        (newChild->nodeType()!=ATTRIBUTE_NODE && refChild->nodeType()==ATTRIBUTE_NODE))
        return NodePtr();

    newChild->_setParent(_myself());
    //
    NodePtr previous = refChild->previousSibling();
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

NodePtr Element::replaceChild(const NodePtr& newChild, const NodePtr& oldChild) {
    // FIXME: raise proper DOMExceptions
    if (!newChild || !oldChild)
        return NodePtr();
    // Cannot insert Node if it already has a parent.
    if (newChild->parentNode())
        return NodePtr();
    // Cannot insert Node if refChild parent is not this Node.
    if (!isSameNode(oldChild->parentNode()))
        return NodePtr();
    // Cannot replace an Attr with a non Attr or a non Attr with an Attr
    if ((newChild->nodeType()==ATTRIBUTE_NODE && oldChild->nodeType()!=ATTRIBUTE_NODE) ||
        (newChild->nodeType()!=ATTRIBUTE_NODE && oldChild->nodeType()==ATTRIBUTE_NODE))
        return NodePtr();
    //
    newChild->_setParent(_myself());
    newChild->_setOwnerDocument(ownerDocument());
    //
    NodePtr previous = oldChild->previousSibling();
    if (previous) {
        newChild->_setPreviousSibling(previous);
        previous->_setNextSibling(newChild);
    }
    else if (newChild->nodeType()==ATTRIBUTE_NODE)
        m_pFirstAttribute = newChild;
    else
        m_pFirstChild = newChild;
    //
    NodePtr next = oldChild->nextSibling();
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

NodePtr Element::appendChild(const NodePtr& n) {
    // FIXME: raise proper DOMExceptions
    if (!n)
        return NodePtr();
    // Cannot append Node if it already has a parent.
    if (n->parentNode())
        return NodePtr();
    //
    n->_setParent(_myself());
    n->_setOwnerDocument(ownerDocument());
    //
    NodePtr last = (n->nodeType()==ATTRIBUTE_NODE)?m_pLastAttribute:m_pLastChild;
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
NodePtr Element::removeChild(const NodePtr& n) {
    // FIXME: raise proper DOMExceptions
    if (!n)
        return NodePtr();
    // Cannot remove a Node if we don't own it.
    if (n->parentNode()!=_myself())
        return NodePtr();

    NodePtr& first = (n->nodeType()==ATTRIBUTE_NODE)?m_pFirstChild:m_pLastChild;
    NodePtr& last  = (n->nodeType()==ATTRIBUTE_NODE)?m_pLastAttribute:m_pLastChild;

    if (n->isSameNode(first))
        first=n->nextSibling();
    if (n->isSameNode(last))
        last=n->previousSibling();
    if (n->previousSibling())
        n->previousSibling()->_setNextSibling(n->nextSibling());
    if (n->nextSibling())
        n->nextSibling()->_setPreviousSibling(n->previousSibling());
    n->_setParent(NodePtr());
    return n;
}
bool Element::hasChildNodes() const {
    return (m_pFirstChild.get()!=0);
}
bool Element::hasAttributes() const {
    return (m_pFirstAttribute.get()!=0);
}

NodePtr Element::cloneNode(bool deep) const {
    // FIXME: raise proper DOMExceptions
    if (ownerDocument()) {
        NodePtr newElement = ownerDocument()->createElement(tagName());
        Element* newImpl = reinterpret_cast<Element*>(newElement.get());
        if (!newImpl) return NodePtr();

        // Clone attributes
        NodePtr child = m_pFirstAttribute;
        while(child) {
            Attr* attr = reinterpret_cast<Attr*>(child.get());
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
    return NodePtr();
}

bool Element::textContent(std::wstring& output) const {
    bool result=false;
    NodePtr child = m_pFirstChild;
    while (child) {
        //FIXME: slow (many reallocs and copies)
        std::wstring tmp;
        bool tmpr = child->textContent(tmp);
        if (tmpr) {
            result=true;
            output+=tmp;
        }
        child=child->nextSibling();
    }
    return result;
}

void Element::setTextContent(const std::wstring& newContent) {
    if (nodeType()==DOCUMENT_NODE) return;
    // Remove all child nodes
    while (m_pFirstChild) removeChild(m_pFirstChild);
    // Create and append a new child with the requested content
    NodePtr textNode = ownerDocument()->createTextNode(newContent);
    if (textNode) appendChild(textNode);
}

NodePtr Element::getElementById(const std::wstring& id) const {
    NodePtr child = m_pFirstChild;
    while (child) {
        if (child->nodeType()==ELEMENT_NODE) {
            NodePtr attr = child->getAttribute(L"id");
            if (attr && attr->nodeValue()==id) return child;
        }
        child=child->nextSibling();
    }
    return NodePtr();
}
NodeListPtr Element::getElementsByTagName(const std::wstring& name) const {
    NodeListPtr output(new NodeList);
    if (output) _addElementsByTagName(output,name);
    return output;
}
void Element::_addElementsByTagName(NodeListPtr& output, const std::wstring& name) const {
    NodePtr child = m_pFirstChild;
    while (child) {
        if (child->nodeType()==ELEMENT_NODE && child->nodeName()==name) {
            output->push_back(child);
            reinterpret_cast<Element*>(child.get())->_addElementsByTagName(output,name);
        }
        child=child->nextSibling();
    }
}
void Element::_setOwnerDocument(const DocumentPtr& document) {
    Node::_setOwnerDocument(document);
    NodePtr child = m_pFirstAttribute;
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

/// Helper function object for _getNamespaceInfoBy
struct NSFindBy {
    std::wstring NamespaceInfo::*mWhat;
    const std::wstring& mValue;
    NSFindBy(std::wstring NamespaceInfo::*what, const std::wstring& value) : mWhat(what), mValue(value) { }
    bool operator()(const NamespaceInfo& test) {
        return test.*mWhat == mValue;
    }
};

const NamespaceInfo& Element::_getNamespaceInfoBy(std::wstring NamespaceInfo::*what, const std::wstring& value) const {
    std::vector<NamespaceInfo>::const_iterator result = find_if(
        m_oNamespacesInfo.begin(),
        m_oNamespacesInfo.end(),
        NSFindBy(what, value));
    if (result == m_oNamespacesInfo.end())
        throw DOMException(DOMException::NOT_FOUND_ERR);
    return *result;
}

