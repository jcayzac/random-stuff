// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(DOM_XPATH_INCLUDED)
#define DOM_XPATH_INCLUDED

namespace dom {

#if 0 // Not yet ported!

class SubExpression;
typedef boost::shared_ptr<SubExpression> SubExpressionPtr;

// Basic XPath support
class XPathExpression {
public:
    /// @brief Default constructor. Construct an invalid expression.
    /// The only purpose of this constructor is to allow the developer to create sequences of expressions.
    XPathExpression();
    XPathExpression(const XPathExpression& o);

    /// @brief Construct a new expression.
    ///
    /// Syntax:
    /// expression :=    expression-item |
    ///                    expression-item '/' expression
    ///
    /// expression-item :=    tagname |
    ///                        tagname '[' subexpression ']'
    /// 
    /// tagname        :=    '*' |
    ///                 '..' |
    ///                    string
    ///
    /// subexpression :=    unaryOp subexpression |
    ///                        subexpression binaryOp subexpression |
    ///                        '(' subexpression ')' |
    ///                        constant |
    ///                        function
    ///    
    /// unaryOp := '!' | 'not'
    /// 
    /// binaryOp :=    '||' | 'or' | '&&' | 'and' |
    ///                '==' | '!=' | '\u2260'
    ///                '>' | '>=' | '\u2265' |
    ///                '<' | '<=' | '\u2264'
    /// 
    /// constant := unsigned long | string
    /// 
    /// function := @ string | attr '(' string ')' | position '(' ')'
    /// 
    XPathExpression(const std::wstring& expression);

    /// @brief Return true if the expression could be parsed, false otherwise
    operator bool() const;

    /// @brief Reset this expression object and re-apply the expression on a new context node.
    /// @return Self.
    XPathExpression& evaluateNode(const NodePtr& contextNode);

    /// @brief Reset this expression object and re-apply the expression on a new set of nodes.
    /// @return Self.
    XPathExpression& evaluateNodes(const NodeListPtr& contextNodes);

    /// @brief Get the next result node of this expression
    /// @return true if a Node object was fetched, false otherwise.
    /// @note if no Node is available at this time, result is untouched.
    friend bool operator>>(XPathExpression& xpe,NodePtr& result);

    /// @brief Copy all the result nodes of this expression into the given NodeList object.
    /// If result is null, it is allocated by this method.
    /// @return true if some Node object were fetched, false otherwise.
    /// @note if no Node is available at this time, result is untouched.
    friend bool operator>>(XPathExpression& xpe,NodeListPtr& result);

    /// @brief Returns the Nth result node of this expression.
    NodePtr operator[](unsigned long index) const;

    /// @brief Reset internal iterator so that after calling (*this) >> nodeObject,
    ///    nodeObject contains a copy of the first result Node.
    inline void reset() { mIterator=0; }

    /// @brief Returns how many result nodes this expression resulted in.
    unsigned long nodeCount() const;
private:
    void addTag(const std::wstring& v) { mTags.push_back(v); }
    void addSubExpression(const SubExpressionPtr& v) { mExpressions.push_back(v); }
    void evaluate();
private:
    std::vector<std::wstring>     mTags;
    std::vector<SubExpressionPtr> mExpressions;
    //
    NodeList      mResult;
    unsigned long mIterator;
    bool          mFound;
    bool          mMalformed;
};

#endif // 0

}; // namespace dom

#endif // DOM_XPATH_INCLUDED
