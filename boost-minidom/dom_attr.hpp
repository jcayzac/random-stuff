// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(DOM_ATTR_INCLUDED)
#define DOM_ATTR_INCLUDED

#include "dom_node.hpp"

namespace dom {

/// @brief Implementation of the ATTRIBUTE_NODE DOM node.
/// http://www.w3.org/TR/DOM-Level-2-Core/core.html#ID-637646024
class Attr: public Node {
private:
    Attr(const Attr&);
public:
    Attr(const std::wstring& name);
    virtual ~Attr() { }

    const std::wstring& name() const  { return nodeName(); }
    const std::wstring& value() const { return m_oValue; }
    void                setValue(const std::wstring& v) { m_oValue=v; }
    ElementPtr          ownerElement() const;

    // From Node
    NodePtr cloneNode(bool deep=false) const;
    bool    textContent(std::wstring& output) const { output=m_oValue; return true; }
    void    setTextContent(const std::wstring& newContent) { m_oValue=newContent; }
};

}; // namespace dom

#endif // DOM_ATTR_INCLUDED
