// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(DOM_ELEMENT_INCLUDED)
#define DOM_ELEMENT_INCLUDED

#include "dom_node.hpp"

namespace dom {

/// Provides basic XML Namespaces support
struct NamespaceInfo {
    std::wstring prefix;
    std::wstring uri;
};
inline bool operator==(const NamespaceInfo &a,const NamespaceInfo &b) {
    return (a.prefix == b.prefix && a.uri == b.uri);
}

/** Implementation of ELEMENT_NODE DOM nodes.
  * http://www.w3.org/TR/DOM-Level-2-Core/core.html#ID-745549614
  */
class Element: public Node {
private:
    friend class Node;
    friend class Document;
    Element(const Element&);
public:
    Element(const std::wstring& name);
    virtual ~Element();

    const std::wstring& tagName() const { return nodeName(); }

    // From Node
    NodePtr        childNodes() const    { return firstChild(); }
    NodePtr        firstChild() const    { return m_pFirstChild; }
    NodePtr        lastChild() const    { return m_pLastChild; }
    NodePtr        attributes() const    { return m_pFirstAttribute; }
    NodePtr        insertBefore(const NodePtr& newChild, const NodePtr& refChild);
    NodePtr        replaceChild(const NodePtr& newChild, const NodePtr& oldChild);
    NodePtr        removeChild(const NodePtr& oldChild);
    NodePtr        appendChild(const NodePtr& newChild);
    bool           hasChildNodes() const;
    bool           hasAttributes() const;
    NodePtr        cloneNode(bool deep=false) const;
    bool           textContent(std::wstring& output) const;
    void           setTextContent(const std::wstring& newContent);
    NodePtr        getAttribute(const std::wstring& name) const;
    void           setAttribute(const std::wstring& name, const std::wstring& value);
    NodePtr        getElementById(const std::wstring& id) const;
    NodeListPtr    getElementsByTagName(const std::wstring& name) const;

protected:
    // From Node
    void _setOwnerDocument(const DocumentPtr&);
private:
    // Used by ElementImpl::getElementsByTagName()
    void _addElementsByTagName(NodeListPtr& output, const std::wstring& name) const;
    const NamespaceInfo& _getNamespaceInfoBy(std::wstring NamespaceInfo::*what, const std::wstring& value) const;

protected:
    NodePtr                    m_pFirstChild;     //<! First child node
    NodePtr                    m_pLastChild;      //<! Last child node
    NodePtr                    m_pFirstAttribute; //<! First attribute node
    NodePtr                    m_pLastAttribute;  //<! Last attribute node
    std::vector<NamespaceInfo> m_oNamespacesInfo; //<! XML namespaces
};

};

#endif // DOM_ELEMENT_INCLUDED
