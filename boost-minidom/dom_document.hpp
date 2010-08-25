// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(DOM_DOCUMENT_INCLUDED)
#define DOM_DOCUMENT_INCLUDED

#include "dom_element.hpp"

namespace dom {

/** Implementation of the DOCUMENT_NODE DOM node.
  * http://www.w3.org/TR/DOM-Level-2-Core/core.html#i-Document
  */
class Document : public Element {
private:
    friend class DOMImplementation;
    friend class Node; // Needed for remove from orphans
private:
    // Only the DOMImplementation can construct Documents
    Document();
    Document(const Document& o);
public:
    virtual ~Document();

    NodePtr    createElement(const std::wstring& name);
    NodePtr    createTextNode(const std::wstring& data);
    NodePtr    createAttribute(const std::wstring& name);
    NodePtr    documentElement () const;

public:
    const std::wstring&    docType() const  { return m_oDoctype; }
    const std::wstring&    encoding() const { return m_oEncoding; }

    void setDocType(const std::wstring& dt) { m_oDoctype=dt; }
    void setEncoding(const std::wstring& e) { m_oEncoding=e; }

private:
    // Used by create*()
    NodePtr    constructNode(Node* object);
private:
    std::wstring m_oDoctype;  //<! Document's type
    std::wstring m_oEncoding; //<! Document's encoding (ignored right now)
};

}; // namespace dom

#endif // DOM_DOCUMENT_INCLUDED
