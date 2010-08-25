// $Id: dom.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/dom/dom.h>
#include <llsdk/inc/scheduler/cscheduler.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

// static
Document DOMImplementation::createDocument() {
	dom::DocumentImpl* object = new dom::DocumentImpl();
	if (object) {
		dom::Document document(object);
		document->_setSelf(document);
		document->_setOwnerDocument(document);
		return document;
	}
	return Document();
}

} // namespace msdk
