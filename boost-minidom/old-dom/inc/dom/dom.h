// $Id: dom.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(DOM_INCLUDED)
#define DOM_INCLUDED

#include <llsdk/inc/dom/dom_types.h>
#include <llsdk/inc/dom/dom_node.h>
#include <llsdk/inc/dom/dom_text.h>
#include <llsdk/inc/dom/dom_attr.h>
#include <llsdk/inc/dom/dom_element.h>
#include <llsdk/inc/dom/dom_document.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

/** @brief DOM Implementation
  */
class DOMImplementation {
public:
	static Document createDocument();
};

} // namespace msdk

#include <llsdk/inc/memory/memory_end.h>
#endif // DOM_INCLUDED
