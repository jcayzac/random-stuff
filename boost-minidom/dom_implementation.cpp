// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "dom_types.hpp"
#include "dom_document.hpp"
using namespace dom;

bool DOMImplementation::hasFeature(const std::wstring& feature, const std::wstring& version) {
    // FIXME: this is so lame -_-;;;
    return false;
}

DocumentPtr DOMImplementation::createDocument() {
    Document* object = new Document();
    if (object) {
        DocumentPtr document(object);
        document->_setSelf(document);
        document->_setOwnerDocument(document);
        return document;
    }
    return DocumentPtr();
}

