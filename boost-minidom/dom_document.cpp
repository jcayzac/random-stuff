// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "dom_document.hpp"
#include "dom_text.hpp"
#include "dom_attr.hpp"
#include "dom_element.hpp"
using namespace dom;

Document::Document()
: Element(L"#D")
, m_oEncoding(L"utf-16")
{
    m_eType=DOCUMENT_NODE;
}
Document::~Document() { }

NodePtr Document::createElement(const std::wstring& name) {
    return constructNode(new Element(name));
}
NodePtr Document::createTextNode(const std::wstring& data) {
    return constructNode(new Text(data));
}
NodePtr Document::createAttribute(const std::wstring& name) {
    return constructNode(new Attr(name));
}

NodePtr Document::constructNode(Node* object) {
    if (object) {
        // Create a shared pointer to the Node
        NodePtr domObject(object);
        // Store a weak version in the node itself
        domObject->_setSelf(domObject);
        // Store a weak pointer to this document in the new node
        domObject->_setOwnerDocument(ownerDocument());
        return domObject;
    }
    return NodePtr();
}

NodePtr Document::documentElement () const {
    return firstChild();
}
