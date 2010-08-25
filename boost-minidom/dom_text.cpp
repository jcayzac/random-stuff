// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "dom_text.hpp"
#include "dom_document.hpp"
using namespace dom;

Text::Text(const std::wstring& data)
: Node(L"#T",TEXT_NODE)
{
    m_oValue=data;
}

NodePtr Text::cloneNode(bool) const {
    if (ownerDocument()) {
        NodePtr newText = ownerDocument()->createTextNode(m_oValue);
        return newText;
    }
    return NodePtr();
}

TextPtr Text::replaceWholeText(const std::wstring& content) {
    NodePtr sibling = nextSibling();
    while (sibling && sibling->nodeType()==TEXT_NODE) {
        parentNode()->removeChild(sibling);
        sibling=nextSibling();
    }
    m_oValue = content;
    return TextPtr(_myself(),boost::detail::static_cast_tag());
}

const std::wstring& Text::wholeText() const {
    NodePtr sibling = nextSibling();
    if (!(sibling && sibling->nodeType()==TEXT_NODE))
        return nodeValue();

    Text* sibling_ptr = reinterpret_cast<Text*>(sibling.get());

    m_oTempString = nodeValue()+sibling_ptr->wholeText();
    sibling_ptr->m_oTempString.clear();

    return m_oTempString;
}

