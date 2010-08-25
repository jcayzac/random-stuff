// $Id: dom_element.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(DOM_ELEMENT_INCLUDED)
#define DOM_ELEMENT_INCLUDED

#include <llsdk/inc/dom/dom_types.h>
#include <llsdk/inc/dom/dom_node.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

struct NamespaceInfo {
	DOMString	prefix;
	DOMString	uri;
};

inline bool operator==(const NamespaceInfo &a,const NamespaceInfo &b)
{
	return (a.prefix == b.prefix && a.uri == b.uri);
}

class ElementImpl: public NodeImpl {
private:
	friend class NodeImpl;
	friend class DocumentImpl;
	ElementImpl(const ElementImpl&);
public:
	ElementImpl(const DOMString& name);
	virtual ~ElementImpl();
	virtual void Output(DOMString& output) const;

	//
	const DOMString&	tagName() const { return nodeName(); }

	// From Node
	Node				childNodes() const	{ return firstChild(); }
	Node				firstChild() const	{ return m_pFirstChild; }
	Node				lastChild() const	{ return m_pLastChild; }
	Node				attributes() const	{ return m_pFirstAttribute; }
	Node				insertBefore(const Node& newChild, const Node& refChild);
	Node				replaceChild(const Node& newChild, const Node& oldChild);
	Node				removeChild(const Node&);
	Node				appendChild(const Node& newChild);
	bool				hasChildNodes() const;
	bool				hasAttributes() const;
	Node				cloneNode(bool deep=false) const;
	bool				textContent(DOMString& output) const;
	void				setTextContent(const DOMString& newContent);
	Node				getAttribute(const DOMString& name) const;
	void				setAttribute(const DOMString& name, const DOMString& value);
	Node				getElementById(const DOMString& id) const;
	NodeList			getElementsByTagName(const DOMString& name) const;

protected:
	// From Node
	void				_setOwnerDocument(const Document&);
private:
	// Used by ElementImpl::getElementsByTagName()
	void _addElementsByTagName(NodeList& output, const DOMString& name) const;

	const NamespaceInfo* _getNamespaceInfoByURI(const DOMString& uri) const;
	const NamespaceInfo* _getNamespaceInfoByPrefix(const DOMString& prefix) const;

protected:
	Node						m_pFirstChild;
	Node						m_pLastChild;
	Node						m_pFirstAttribute;
	Node						m_pLastAttribute;
	msdk::vector<NamespaceInfo>	m_oNamespacesInfo;
};

} // namespace dom

#include <llsdk/inc/memory/memory_end.h>
#endif // DOM_ELEMENT_INCLUDED
