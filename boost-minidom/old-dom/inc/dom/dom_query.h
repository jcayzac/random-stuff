// $Id: dom_query.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(DOM_QUERY_INCLUDED)
#define DOM_QUERY_INCLUDED

#include <llsdk/inc/common/cstring.h>
#include <llsdk/inc/dom/dom_types.h>
#include <llsdk/inc/text/unicode.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

class SubExpressionImpl;
typedef msdk::shared_ptr<SubExpressionImpl> SubExpression;

// An XPath-like expression
class XPathExpression {
public:
	/// @brief Default constructor. Construct an invalid expression.
	/// The only purpose of this constructor is to allow the developer to create sequences of expressions.
	XPathExpression();
	XPathExpression(const XPathExpression& o);

	/// @brief Construct a new expression.
	///
	/// Syntax:
	/// expression :=	expression-item |
	///					expression-item '/' expression
	///
	/// expression-item :=	tagname |
	///						tagname '[' subexpression ']'
	/// 
	/// tagname		:=	'*' |
	/// 				'..' |
	///					string
	///
	/// subexpression :=	unaryOp subexpression |
	///						subexpression binaryOp subexpression |
	///						'(' subexpression ')' |
	///						constant |
	///						function
	///	
	/// unaryOp := '!' | 'not'
	/// 
	/// binaryOp :=	'||' | 'or' | '&&' | 'and' |
	///				'==' | '!=' | '\u2260'
	///				'>' | '>=' | '\u2265' |
	///				'<' | '<=' | '\u2264'
	/// 
	/// constant := unsigned long | string
	/// 
	/// function := @ string | attr '(' string ')' | position '(' ')'
	/// 
	XPathExpression(const DOMString& expression);

	/// @brief Return true if the expression could be parsed, false otherwise
	operator bool() const;

	/// @brief Reset this expression object and re-apply the expression on a new context node.
	/// @return Self.
	XPathExpression& evaluateNode(const Node& contextNode);

	/// @brief Reset this expression object and re-apply the expression on a new set of nodes.
	/// @return Self.
	XPathExpression& evaluateNodes(const NodeList& contextNodes);

	/// @brief Get the next result node of this expression
	/// @return true if a Node object was fetched, false otherwise.
	/// @note if no Node is available at this time, result is untouched.
	friend bool operator>>(XPathExpression& xpe,Node& result);

	/// @brief Copy all the result nodes of this expression into the given NodeList object.
	/// If result is null, it is allocated by this method.
	/// @return true if some Node object were fetched, false otherwise.
	/// @note if no Node is available at this time, result is untouched.
	friend bool operator>>(XPathExpression& xpe,NodeList& result);

	/// @brief Returns the Nth result node of this expression.
	Node operator[](unsigned long index) const;

	/// @brief Reset internal iterator so that after calling (*this) >> nodeObject,
	///	nodeObject contains a copy of the first result Node.
	inline void reset() { mIterator=0; }

	/// @brief Returns how many result nodes this expression resulted in.
	unsigned long nodeCount() const;
private:
	bool addTag(const DOMString& v)					{ return (0==mTags.push_back(v)); }
	bool addSubExpression(const SubExpression& v)	{ return (0==mExpressions.push_back(v)); }
	void evaluate();
private:
	msdk::vector<DOMString>		mTags;
	msdk::vector<SubExpression> mExpressions;
	//
	NodeListImpl				mResult;
	unsigned long				mIterator;
	bool						mFound;
	bool						mMalformed;
};

struct SingleValueQuery {
	SingleValueQuery(Node _root);
	const SingleValueQuery& operator()(const DOMString& path);

	operator bool() const { return found; }
	operator const DOMString&() const { return result; }
	friend bool operator>>(const SingleValueQuery& svq,DOMString& result);
	friend bool operator>>(const SingleValueQuery& svq,unsigned short& result);
	friend bool operator>>(const SingleValueQuery& svq,unsigned long& result);
	friend bool operator>>(const SingleValueQuery& svq,short& result);
	friend bool operator>>(const SingleValueQuery& svq,long& result);
	friend bool operator>>(const SingleValueQuery& svq,float& result);
	friend bool operator>>(const SingleValueQuery& svq,bool& result);
private:
	DOMString		result;
	Node			root;
	bool			found;
};

} // namespace dom

#include <llsdk/inc/memory/memory_end.h>
#endif // DOM_QUERY_INCLUDED
