// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(DOM_TYPES_INCLUDED)
#define DOM_TYPES_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>
#include <exception>

namespace dom {

// Forward declarations
class Node;
class Attr;
class Text;
class Element;
class Document;
class DocumentType;

/** Implementation of DOMException.
  * http://www.w3.org/TR/DOM-Level-2-Core/core.html#ID-17189187
  */
class DOMException : public std::exception {
public:
    enum {
        INDEX_SIZE_ERR = 1,
        DOMSTRING_SIZE_ERR,
        HIERARCHY_REQUEST_ERR,
        WRONG_DOCUMENT_ERR,
        INVALID_CHARACTER_ERR,
        NO_DATA_ALLOWED_ERR,
        NO_MODIFICATION_ALLOWED_ERR,
        NOT_FOUND_ERR,
        NOT_SUPPORTED_ERR,
        INUSE_ATTRIBUTE_ERR,
        INVALID_STATE_ERR,
        SYNTAX_ERR,
        INVALID_MODIFICATION_ERR,
        NAMESPACE_ERR,
        INVALID_ACCESS_ERR,
    };
public:
    DOMException(unsigned short c): mCode(c) { }
    unsigned short code() const { return mCode; }
    virtual const char* what() const throw() {
        return "DOMException";
    }
private:
    unsigned short mCode;
};


// From DOM-Level-2-Core, section 1.1.2, "Memory Management"
// (http://www.w3.org/TR/DOM-Level-2-Core/core.html#ID-249F15BA)
//
// To ensure a consistent API across [...] platforms, the DOM does not address
// memory management issues at all, but instead leaves these for the
// implementation. Neither of the explicit language bindings defined by the
// DOM API (for ECMAScript and Java) require any memory management methods,
// but DOM bindings for other languages (especially C or C++) may require such
// support.

// The following typedefs will help dealing with memory management

typedef boost::shared_ptr<Node>         NodePtr;
typedef boost::shared_ptr<Attr>         AttrPtr;
typedef boost::shared_ptr<Text>         TextPtr;
typedef boost::shared_ptr<Element>      ElementPtr;
typedef boost::shared_ptr<Document>     DocumentPtr;
typedef boost::shared_ptr<DocumentType> DocumentTypePtr;

typedef boost::weak_ptr<Node>         NodeWeakPtr;
typedef boost::weak_ptr<Attr>         AttrWeakPtr;
typedef boost::weak_ptr<Text>         TextWeakPtr;
typedef boost::weak_ptr<Element>      ElementWeakPtr;
typedef boost::weak_ptr<Document>     DocumentWeakPtr;
typedef boost::weak_ptr<DocumentType> DocumentTypeWeakPtr;

// DOM defines a NodeList as follow
// (http://www.w3.org/TR/DOM-Level-2-Core/core.html#ID-536297177)
// 
// interface NodeList {
//        Node item(in unsigned long index);
//        readonly attribute unsigned long    length;
// };
// 
// ...but for simplicity/efficiency, I will use a std::vector<> instead
typedef std::vector<NodePtr>        NodeList;
typedef boost::shared_ptr<NodeList> NodeListPtr;
typedef boost::weak_ptr<NodeList>   NodeListWeakPtr;
typedef NodeList::iterator          NodeIterator;
typedef NodeList::const_iterator    NodeConstIterator;

/** Basic implementation of DOMImplementation.
  * http://www.w3.org/TR/DOM-Level-2-Core/core.html#ID-102161490
  */
class DOMImplementation {
public:
    static bool        hasFeature(const std::wstring& feature, const std::wstring& version);
    static DocumentPtr createDocument();
};

}; // namespace dom

#endif // DOM_TYPES_INCLUDED
