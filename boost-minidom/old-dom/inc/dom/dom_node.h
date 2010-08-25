// $Id: dom_node.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(DOM_NODE_INCLUDED)
#define DOM_NODE_INCLUDED

#include <llsdk/inc/dom/dom_types.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

/** @brief Base class for all DOM nodes.
  * @note Nodes may or may not have a parent element.
  * @note Nodes always have an owner. i.e.) ownerDocument() never returns null.
  */
class NodeImpl {
	friend class DOMImplementation;	// Needed for all those create*() methods in DOM
	friend class ElementImpl;		// Needed for attaching/detaching child nodes
	friend class DocumentImpl;		// Needed for attaching/detaching the document element.
public:
	enum TNodeType {
		ELEMENT_NODE	= 1,			//<! An element.
		ATTRIBUTE_NODE,					//<! An element attribute.
		TEXT_NODE,						//<! Some text
		CDATA_SECTION_NODE,				//<! Not implemented.
		ENTITY_REFERENCE_NODE,			//<! Not implemented.
		ENTITY_NODE,					//<! Not implemented.
		PROCESSING_INSTRUCTION_NODE,	//<! Not implemented.
		COMMENT_NODE,					//<! Not implemented.
		DOCUMENT_NODE,					//<! A document.
		DOCUMENT_TYPE_NODE,				//<! Not implemented.
		DOCUMENT_FRAGMENT_NODE,			//<! Not implemented.
		NOTATION_NODE,					//<! Not implemented.
	};
private:
	NodeImpl();
	NodeImpl(const NodeImpl&);
protected:
	NodeImpl(const DOMString& name, TNodeType type);
public:
	virtual ~NodeImpl() { }
	virtual void Output(DOMString& output) const = 0;
	void debug(DOMString& output) const;
	static void EncodeEntities(DOMString& value, bool quoted);

	const DOMString&	nodeName() const		{ return m_oName; }
	const DOMString&	nodeValue() const		{ return m_oValue; }
	TNodeType			nodeType() const		{ return m_eType; }

	const DOMString&	namespaceURI() const;
	const DOMString&	prefix() const			{ return m_oPrefix; }
	const DOMString&	localName() const		{ return m_oLocalName; }
	const DOMString&	lookupPrefix(const DOMString& namespaceURI) const;
	bool				isDefaultNamespace(const DOMString& namespaceURI) const;
	const DOMString&	lookupNamespaceURI(const DOMString& prefix) const;

	Document			ownerDocument() const	{ return m_pOwnerDocument.lock(); }
	Node				parentNode() const		{ return m_pParent.lock(); }

	virtual Node		firstChild() const		{ return Node(); }
	virtual Node		lastChild() const		{ return Node(); }
	Node				previousSibling() const	{ return m_pPreviousSibling.lock(); }
	Node				nextSibling() const		{ return m_pNextSibling; }
	unsigned long		nodePosition() const;

	virtual Node		childNodes() const		{ return Node(); }
	virtual Node		attributes() const		{ return Node(); }
	Node				operator[](int);

	virtual Node		insertBefore(const Node& /*newChild, const Node& refChild*/)	{ return Node(); }
	virtual Node		replaceChild(const Node& /*newChild, const Node& oldChild*/)	{ return Node(); }
	virtual Node		removeChild(const Node& /*oldChild*/)							{ return Node(); }
	virtual Node		appendChild(const Node& /*newChild*/)							{ return Node(); }
	virtual bool		hasChildNodes() const											{ return false;  }
	virtual bool		hasAttributes() const											{ return false;  }

	virtual Node		getAttribute(const DOMString& /*name*/) const						{ return Node(); }
	virtual void		setAttribute(const DOMString& /*name*/, const DOMString& /*value*/)	{ }
	virtual Node		getElementById(const DOMString& /*id*/) const						{ return Node(); }
	virtual NodeList	getElementsByTagName(const DOMString& /*name*/) const				{ return NodeList(); }

	virtual Node		cloneNode(bool deep=false) const = 0;
	virtual bool		textContent(DOMString& output) const = 0;
	virtual void		setTextContent(const DOMString& newContent) = 0;
	// Comparison
	friend bool			operator==(const NodeImpl& a, const NodeImpl& b);
	friend bool			operator!=(const NodeImpl& a, const NodeImpl& b);
	bool				isSameNode(const Node& n) const;
	bool				isEqualNode(const Node& n) const;

	// Conversion
	friend bool operator>>(const Node& node,DOMString& result);
	friend bool operator>>(const Node& node,unsigned short& result);
	friend bool operator>>(const Node& node,unsigned long& result);
	friend bool operator>>(const Node& node,short& result);
	friend bool operator>>(const Node& node,long& result);
	friend bool operator>>(const Node& node,float& result);
	friend bool operator>>(const Node& node,bool& result);

public:
	Node				_myself() const;
	void				_setParent(const Node&);
	void				_setOwnerDocument(const Document&);


private:
	// Only set by Element when attaching/detaching a child Node
	void _setPreviousSibling(Node node)	{ m_pPreviousSibling = node; }
	void _setNextSibling(Node node)		{ m_pNextSibling = node; }

private:
	// Only set on creation by the owner Document
	void _setSelf(Node self) { m_pSelf = self; }

protected:
	// Only set by derivated classes
	DOMString	m_oValue;

private:
	TNodeType	m_eType;
	DOMString	m_oName;
	//
	DOMString	m_oPrefix;
	DOMString	m_oLocalName;
	//
	WeakDocument	m_pOwnerDocument;
	WeakNode		m_pParent;
	WeakNode		m_pSelf;
	//
	Node			m_pNextSibling;
	WeakNode		m_pPreviousSibling;
};

} // namespace dom

#include <llsdk/inc/memory/memory_end.h>
#endif // DOM_NODE_INCLUDED
