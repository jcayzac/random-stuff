// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include "dom_attr.hpp"
#include "dom_document.hpp"
using namespace dom;

Attr::Attr(const std::wstring& name)
: Node(name,ATTRIBUTE_NODE)
{ }

NodePtr Attr::cloneNode(bool /* deep_has_no_meaning_here */) const {
    if (ownerDocument()) {
        NodePtr newAttr = ownerDocument()->createAttribute(name());
        Attr* newImpl = reinterpret_cast<Attr*>(newAttr.get());
        newImpl->setValue(value());
        return newAttr;
    }
    return NodePtr(); // FIXME: raise DOMException instead
}

ElementPtr Attr::ownerElement() const {
    NodePtr parent = parentNode();
    while (parent) {
        if (parent->nodeType()==ELEMENT_NODE)
            return ElementPtr(parent, boost::detail::static_cast_tag());
        parent = parent->parentNode();
    }
    return ElementPtr();
}
