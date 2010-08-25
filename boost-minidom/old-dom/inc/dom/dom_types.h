// $Id: dom_types.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(DOM_TYPES_INCLUDED)
#define DOM_TYPES_INCLUDED

#include <llsdk/inc/common/cstring.h>
#include <llsdk/inc/common/tlist.h>
#include <llsdk/inc/common/sharedptr.h>
#include <llsdk/inc/common/cscopeptr.h>
#include <llsdk/inc/common/weak_ptr.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {
typedef msdk::CString		DOMString;
typedef unsigned long long	DOMUnique;

class NodeImpl;
class AttrImpl;
class TextImpl;
class ElementImpl;
class DocumentImpl;
class DOMImplementation;

typedef msdk::shared_ptr<NodeImpl>		Node;
typedef msdk::shared_ptr<AttrImpl>		Attr;
typedef msdk::shared_ptr<TextImpl>		Text;
typedef msdk::shared_ptr<ElementImpl>	Element;
typedef msdk::shared_ptr<DocumentImpl>	Document;

typedef msdk::weak_ptr<NodeImpl>		WeakNode;
typedef msdk::weak_ptr<AttrImpl>		WeakAttr;
typedef msdk::weak_ptr<TextImpl>		WeakText;
typedef msdk::weak_ptr<ElementImpl>		WeakElement;
typedef msdk::weak_ptr<DocumentImpl>	WeakDocument;

typedef msdk::vector<Node>				NodeListImpl;
typedef msdk::shared_ptr<NodeListImpl>	NodeList;
typedef msdk::weak_ptr<NodeListImpl>	WeakNodeList;

typedef NodeListImpl::iterator			NodeIterator;
typedef NodeListImpl::const_iterator	NodeConstIterator;

} // namespace dom

#include <llsdk/inc/memory/memory_end.h>
#endif // DOM_TYPES_INCLUDED
