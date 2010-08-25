// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(DOM_NODE_INCLUDED)
#define DOM_NODE_INCLUDED

#include "dom_types.hpp"

namespace dom {

/** @brief Base class for all DOM nodes.
  *
  * http://www.w3.org/TR/DOM-Level-2-Core/core.html#ID-1950641247
  * @note Nodes may or may not have a parent element.
  * @note Nodes always have an owner. i.e.) ownerDocument() never returns null.
  * @note Since the DOM specs say most of the API must be availble via the \
  *       Node class, there are more reinterpret_cast in this class than it \
  *       takes to make you cry.
  */
class Node {
private:
    friend class DOMImplementation;  //<! Needed for all those create*() methods in DOM
    friend class Element;            //<! Needed for attaching/detaching child nodes
    friend class Document;           //<! Needed for attaching/detaching the document element.
public:
    enum TNodeType {
        ELEMENT_NODE    = 1,         //<! An element.
        ATTRIBUTE_NODE,              //<! An element attribute.
        TEXT_NODE,                   //<! Some text
        CDATA_SECTION_NODE,          //<! Not yet implemented.
        ENTITY_REFERENCE_NODE,       //<! Not yet implemented.
        ENTITY_NODE,                 //<! Not yet implemented.
        PROCESSING_INSTRUCTION_NODE, //<! Not yet implemented.
        COMMENT_NODE,                //<! Not yet implemented.
        DOCUMENT_NODE,               //<! A document.
        DOCUMENT_TYPE_NODE,          //<! Not yet implemented.
        DOCUMENT_FRAGMENT_NODE,      //<! Not yet implemented.
        NOTATION_NODE,               //<! Not yet implemented.
    };
private:
    Node();
    Node(const Node&);
protected:
    Node(const std::wstring& name, TNodeType type);
public:
    virtual ~Node() { }

    const std::wstring& nodeName() const        { return m_oName; }
    const std::wstring& nodeValue() const       { return m_oValue; }
    TNodeType           nodeType() const          { return m_eType; }

    const std::wstring& namespaceURI() const;
    const std::wstring& prefix() const          { return m_oPrefix; }
    const std::wstring& localName() const       { return m_oLocalName; }
    const std::wstring& lookupPrefix(const std::wstring& namespaceURI) const;
    bool                isDefaultNamespace(const std::wstring& namespaceURI) const;
    const std::wstring& lookupNamespaceURI(const std::wstring& prefix) const;

    DocumentPtr         ownerDocument() const   { return m_pOwnerDocument.lock(); }
    NodePtr             parentNode() const      { return m_pParent.lock(); }

    virtual NodePtr     firstChild() const      { return NodePtr(); }
    virtual NodePtr     lastChild() const       { return NodePtr(); }
    NodePtr             previousSibling() const { return m_pPreviousSibling.lock(); }
    NodePtr             nextSibling() const     { return m_pNextSibling; }
    unsigned long       nodePosition() const;

    virtual NodePtr     childNodes() const { return NodePtr(); }
    virtual NodePtr     attributes() const { return NodePtr(); }
    NodePtr             operator[](int);

    virtual NodePtr     insertBefore(const NodePtr& newChild, const NodePtr& refChild)   { return NodePtr(); }
    virtual NodePtr     replaceChild(const NodePtr& newChild, const NodePtr& oldChild)   { return NodePtr(); }
    virtual NodePtr     removeChild(const NodePtr& oldChild)                             { return NodePtr(); }
    virtual NodePtr     appendChild(const NodePtr& newChild)                             { return NodePtr(); }
    virtual bool        hasChildNodes() const                                            { return false;  }
    virtual bool        hasAttributes() const                                            { return false;  }

    virtual NodePtr     getAttribute(const std::wstring& name) const { return NodePtr(); }
    virtual void        setAttribute(const std::wstring& name, const std::wstring& value) { }
    virtual NodePtr     getElementById(const std::wstring& id) const { return NodePtr(); }
    virtual NodeListPtr getElementsByTagName(const std::wstring& name) const { return NodeListPtr(); }

    virtual NodePtr     cloneNode(bool deep=false) const = 0;
    virtual bool        textContent(std::wstring& output) const = 0;
    virtual void        setTextContent(const std::wstring& newContent) = 0;
    // Comparison
    friend bool         operator==(const Node& a, const Node& b);
    friend bool         operator!=(const Node& a, const Node& b);
    bool                isSameNode(const NodePtr& n) const;
    bool                isEqualNode(const NodePtr& n) const;

public:
    NodePtr _myself() const;
    void    _setParent(const NodePtr&);
    void    _setOwnerDocument(const DocumentPtr&);

private:
    // Only set by Element when attaching/detaching a child Node
    void _setPreviousSibling(NodePtr node)    { m_pPreviousSibling = node; }
    void _setNextSibling(NodePtr node)        { m_pNextSibling = node; }

private:
    // Only set on creation by the owner Document
    // Gives a Node the power to return its own shared_ptr
    void _setSelf(NodePtr self) { m_pSelf = self; }

protected:
    // Only set by derivated classes
    std::wstring    m_oValue;

private:
    TNodeType       m_eType;
    std::wstring    m_oName;
    //
    std::wstring    m_oPrefix;
    std::wstring    m_oLocalName;
    //
    DocumentWeakPtr m_pOwnerDocument;
    NodeWeakPtr     m_pParent;
    NodeWeakPtr     m_pSelf;
    //
    NodePtr         m_pNextSibling;
    NodeWeakPtr     m_pPreviousSibling;
};

}; // namespace dom

#endif // DOM_NODE_INCLUDED
