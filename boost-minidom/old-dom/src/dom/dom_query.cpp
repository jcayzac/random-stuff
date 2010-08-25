// $Id: dom_query.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/dom/dom_node.h>
#include <llsdk/inc/dom/dom_document.h>
#include <llsdk/inc/dom/dom_query.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

static void explodeNonEscaped(const DOMString& s, mobilesdk_char sep, msdk::vector<DOMString>& result) {
	unsigned long st_beg=0;
	unsigned long st_end;
	msdk::vector<mobilesdk_char> cs;

	const mobilesdk_char* buffer = s.Get();
	unsigned long length = s.Length();

	result.clear();
	bool loop=true;
	while (loop) {
		st_end=s.Find(sep, st_beg);
		if (st_end==msdk::CString::npos) {
			for (unsigned long c=st_beg; c<length; ++c)
				cs.push_back(buffer[c]);
			loop=false;
		}
		else {
			bool escaped = (st_end>0 && buffer[st_end-1]==0x005c);
			if (escaped) --st_end;
			for (unsigned long c=st_beg; c<st_end; ++c)
				cs.push_back(buffer[c]);
			if (escaped) {
				st_beg=st_end+2;
				continue;
			}
		}
		cs.push_back(0);
		DOMString newString(&cs[0]);
		result.push_back(newString);
		cs.clear();
		st_beg=st_end+1;
	}
}

class XVar {
public:
	enum TType {
		INVALID_TYPE,
		NODESET_TYPE,
		STRING_TYPE,
		INTEGER_TYPE,
	};
public:
	XVar(): mType(INVALID_TYPE) { }
	XVar(TType type)
	: mType(type), mInteger(0)
	{
		if (type==NODESET_TYPE) {
			mNodeSet.reset(new NodeListImpl);
			if (!mNodeSet) mType=INVALID_TYPE;
		}
	}
	XVar(long v)
	: mType(INTEGER_TYPE), mInteger(v)
	{
	}
	XVar(const DOMString& v)
	: mType(STRING_TYPE), mString(v), mInteger(0)
	{
	}
	XVar(const NodeList& v)
	: mType(NODESET_TYPE), mNodeSet(v), mInteger(0)
	{
		if (!mNodeSet) mType=INVALID_TYPE;
	}
	XVar(const XVar& o)
	: mType(o.mType)
	, mNodeSet(o.mNodeSet)
	, mString(o.mString)
	, mInteger(o.mInteger)
	{
	}
	//
	bool isString() const	{ return mType==STRING_TYPE;  }
	bool isNodeSet() const	{ return mType==NODESET_TYPE; }
	bool isInteger() const	{ return mType==INTEGER_TYPE; }
	operator bool() const	{ return mType!=INVALID_TYPE; }
	const NodeList&		nodeSetValue() const	{ return mNodeSet; }
	const DOMString&	stringValue() const		{ return mString; }
	long				integerValue() const	{ return mInteger; }
private:
	TType			mType;
	NodeList		mNodeSet;
	DOMString		mString;
	long			mInteger;
};

struct TToken {
	enum {
		TOKEN_NOTYPE,
		// Variables
		TOKEN_VAR,
		// Unary operators
		TOKEN_NOT,
		// Binary operators
		TOKEN_EQUAL, TOKEN_NOT_EQUAL, TOKEN_AND, TOKEN_OR, TOKEN_LESS, TOKEN_LESS_OR_EQUAL, TOKEN_GREATER, TOKEN_GREATER_OR_EQUAL,
		// Identifier
		TOKEN_SYMBOL,
		// Brackets
		TOKEN_OPEN, TOKEN_CLOSE,
		// Functions
		TOKEN_ATTRIB, TOKEN_POSITION,
	};
	//
	TToken(): mType(TOKEN_NOTYPE) { }
	TToken(int t): mType(t) { }
	TToken(int t, long v): mType(t), mParam(v) { }
	TToken(int t, const DOMString& v): mType(t), mParam(v) { }
	TToken(const TToken& o): mType(o.mType), mParam(o.mParam) { }
	//
	int			mType;
	XVar		mParam;
};

inline bool operator==(const TToken &a,const TToken &b)
{
	return (a.mType == b.mType && a.mParam == b.mParam);
}

static const int PRECEDENCE[] = {
	0, // TOKEN_NOTYPE,
	0, // TOKEN_VAR,
	5, // TOKEN_NOT,
	3, // TOKEN_EQUAL,
	3, // TOKEN_NOT_EQUAL,
	2, // TOKEN_AND,
	1, // TOKEN_OR,
	4, // TOKEN_LESS,
	4, // TOKEN_LESS_OR_EQUAL,
	4, // TOKEN_GREATER,
	4, // TOKEN_GREATER_OR_EQUAL,
	0, // TOKEN_SYMBOL,
	0, // TOKEN_OPEN,
	0, // TOKEN_CLOSE,
	0, // TOKEN_ATTRIB,
	0, // TOKEN_POSITION,
};
inline bool operator>(const TToken& a, const TToken& b) {
	return PRECEDENCE[a.mType]>PRECEDENCE[b.mType];
}

class SubExpressionImpl {
public:
	SubExpressionImpl() { }
	SubExpressionImpl(const SubExpressionImpl& o): mTokens(o.mTokens) { }
	static SubExpression buildSubExpression(const DOMString& source);
	XVar evaluate(const Node& n) const;
private:
	bool buildProgram(const DOMString& source);
	bool tokenizeSource(const DOMString& source);
	bool infix2postfix();
private:
	msdk::vector<TToken> mTokens;
};


static void debugTokens(const DOMString& extra, const msdk::vector<TToken>& v) {
#if defined(NAV_PLAT_WIN32)
	DOMString o=extra;
	o+="\n--- Vector @";
	o+=(unsigned long)(void*)(&v);
	o+=" ---\n";
	msdk::vector<TToken>::const_iterator it = v.begin();
	unsigned long index=1;
	while (it!=v.end()) {
		const TToken& t = *it; ++it;
		o+=index;
		o+="\t[";
		switch (t.mType) {
		case TToken::TOKEN_NOTYPE:			o+="NOTYPE";			break;
		case TToken::TOKEN_VAR:
			o+="VAR";
			if (t.mParam.isInteger())		{ o+="=(integer,";  o+=t.mParam.integerValue(); o+=")"; }
			else if (t.mParam.isString())	{ o+="=(string,\""; o+=t.mParam.stringValue(); o+="\")"; }
			else if (t.mParam.isNodeSet())	{ o+="=(nodeset, size="; o+=t.mParam.nodeSetValue()->size(); o+=")"; }
			else							o+="(invalid)";
			break;
		case TToken::TOKEN_NOT:				o+="NOT";				break;
		case TToken::TOKEN_EQUAL:			o+="EQUAL";				break;
		case TToken::TOKEN_NOT_EQUAL:		o+="NOT_EQUAL";			break;
		case TToken::TOKEN_AND:				o+="AND";				break;
		case TToken::TOKEN_OR:				o+="OR";				break;
		case TToken::TOKEN_LESS:			o+="LESS";				break;
		case TToken::TOKEN_LESS_OR_EQUAL:	o+="LESS_OR_EQUAL";		break;
		case TToken::TOKEN_GREATER:			o+="GREATER";			break;
		case TToken::TOKEN_GREATER_OR_EQUAL:o+="GREATER_OR_EQUAL";	break;
		case TToken::TOKEN_SYMBOL:			o+="SYMBOL";			break;
		case TToken::TOKEN_OPEN:			o+="OPEN";				break;
		case TToken::TOKEN_CLOSE:			o+="CLOSE";				break;
		case TToken::TOKEN_ATTRIB:			o+="ATTRIB";			break;
		case TToken::TOKEN_POSITION:		o+="POSITION";			break;
		}
		o+="]\n";
		++index;
	}
	o+="-----------------------------------\n\n";
	OutputDebugStringW(o.Get());
#else
	(void)extra;
	(void)v;
#endif
}

bool SubExpressionImpl::buildProgram(const DOMString& source) {
	if (!tokenizeSource(source))
		return false;
	if (!infix2postfix())
		return false;
#if defined(NAV_PLAT_WIN32)
	DOMString o;
	o+="Program for expression \"";
	o+=source;
	o+="\":";
	debugTokens(o, mTokens);
#endif
	return true;
}

bool SubExpressionImpl::tokenizeSource(const DOMString& source) {
	mTokens.clear();

	// Split around '
	msdk::vector<DOMString> tokens0;
	explodeNonEscaped(source,0x0027,tokens0);
	if (tokens0.empty())
		return false;

	// Tokenize (coarse)
	unsigned long length = tokens0.size();
	for (unsigned long i=0; i<length; ++i) {
		DOMString& token = tokens0[i];
		if (i%2) mTokens.push_back(TToken(TToken::TOKEN_VAR,token));
		else {
			const mobilesdk_char* buffer = token.Get();
			unsigned long bufferLength = token.Length();
			msdk::vector<mobilesdk_char> symbol;
			while(bufferLength) {
				mobilesdk_char c = *(buffer++); --bufferLength;
				bool ws=text::IsWhiteSpace(c);
				if (ws || c==0x0021 || c==0x0026 || c==0x0028 || c==0x0029 || c==0x003c || c==0x003d || c==0x003e || c==0x0040 || c==0x007c) {
					if (!symbol.empty()) {
						symbol.push_back(0);
						mTokens.push_back(TToken(TToken::TOKEN_SYMBOL, DOMString(&symbol[0])));
						symbol.clear();
					}
				}
				switch(c) {
				case 0x0021:	mTokens.push_back(TToken(TToken::TOKEN_NOT)); break;
				case 0x0026:	if (bufferLength && (*buffer==0x0026)) {
									++buffer;
									--bufferLength;
									mTokens.push_back(TToken(TToken::TOKEN_AND));
								}
								else
									return false;
								break;
				case 0x0028:	mTokens.push_back(TToken(TToken::TOKEN_OPEN));		break;
				case 0x0029:	mTokens.push_back(TToken(TToken::TOKEN_CLOSE));		break;
				case 0x003c:	mTokens.push_back(TToken(TToken::TOKEN_LESS));		break;
				case 0x003d:	mTokens.push_back(TToken(TToken::TOKEN_EQUAL));		break;
				case 0x003e:	mTokens.push_back(TToken(TToken::TOKEN_GREATER));	break;
				case 0x0040:	mTokens.push_back(TToken(TToken::TOKEN_ATTRIB));		break;
				case 0x007c:	if (bufferLength && (*buffer==0x007c)) {
									++buffer;
									--bufferLength;
									mTokens.push_back(TToken(TToken::TOKEN_OR));
								}
								else
									return false;
								break;
				// Because UCS is beautiful...
				case 0x2260:	mTokens.push_back(TToken(TToken::TOKEN_NOT_EQUAL));	break;
				case 0x2264:	mTokens.push_back(TToken(TToken::TOKEN_LESS_OR_EQUAL));	break;
				case 0x2265:	mTokens.push_back(TToken(TToken::TOKEN_LESS_OR_EQUAL));	break;
				default:		if (!ws) symbol.push_back(c); break;
				}
			}
			if (!symbol.empty()) {
				symbol.push_back(0);
				mTokens.push_back(TToken(TToken::TOKEN_SYMBOL, DOMString(&symbol[0])));
				symbol.clear();
			}
		}
	}

	// Tokenize (fine)
	msdk::vector<TToken> tokens;
	tokens.swap(mTokens);
	length = tokens.size();
	for (unsigned long i=0; i<length; ++i) {
		TToken& token = tokens[i];

		switch(token.mType) {
		// Replace [NOT,EQUAL] with [NOT_EQUAL]
		case TToken::TOKEN_NOT:
			if (i+1<length && tokens[i+1].mType==TToken::TOKEN_EQUAL) {
				token.mType=TToken::TOKEN_NOT_EQUAL;
				++i; //skip next
			}
			mTokens.push_back(token);
			break;
		// Replace [GREATER,EQUAL] with [GREATER_OR_EQUAL]
		case TToken::TOKEN_GREATER:
			if (i+1<length && tokens[i+1].mType==TToken::TOKEN_EQUAL) {
				token.mType=TToken::TOKEN_GREATER_OR_EQUAL;
				++i; //skip next
			}
			mTokens.push_back(token);
			break;
		// Replace [LESS,EQUAL] with [LESS_OR_EQUAL]
		case TToken::TOKEN_LESS:
			if (i+1<length && tokens[i+1].mType==TToken::TOKEN_EQUAL) {
				token.mType=TToken::TOKEN_LESS_OR_EQUAL;
				++i; //skip next
			}
			mTokens.push_back(token);
			break;
		// Replace [ATTRIB,SYMBOL] with [ATTRIB(SYMBOL)]
		case TToken::TOKEN_ATTRIB:
			if (i+1<length && tokens[i+1].mType==TToken::TOKEN_SYMBOL && tokens[i+1].mParam.isString()) {
				mTokens.push_back(TToken(TToken::TOKEN_ATTRIB,tokens[i+1].mParam));
				++i; // skip next
			}
			else return false;
			break;
		case TToken::TOKEN_SYMBOL:
			// Replace [SYMBOL("attr"),OPEN,VAR,CLOSE] with [ATTRIB(VAR)]
			if (i+3<length &&
				tokens[i+1].mType==TToken::TOKEN_OPEN &&
				tokens[i+2].mType==TToken::TOKEN_VAR &&
				tokens[i+3].mType==TToken::TOKEN_CLOSE &&
				token.mParam.isString() &&
				token.mParam.stringValue()=="attr" &&
				tokens[i+2].mParam.isString()) {
				mTokens.push_back(TToken(TToken::TOKEN_ATTRIB,tokens[i+2].mParam.stringValue()));
				i+=3; // skip next 3
			}
			else
			// Replace [SYMBOL("position",OPEN,CLOSE] with [POSITION]
			if (i+2<length &&
				tokens[i+1].mType==TToken::TOKEN_OPEN &&
				tokens[i+2].mType==TToken::TOKEN_CLOSE &&
				token.mParam.isString() &&
				token.mParam.stringValue()=="position") {
				mTokens.push_back(TToken(TToken::TOKEN_POSITION));
				i+=2; // skip next
			}
			else
			// Replace [SYMBOL("not")] with [NOT]
			if (token.mParam.isString() &&
				token.mParam.stringValue()=="not") {
				mTokens.push_back(TToken(TToken::TOKEN_NOT));
			}
			else
			// Replace [SYMBOL("and")] with [AND]
			if (token.mParam.isString() &&
				token.mParam.stringValue()=="and") {
				mTokens.push_back(TToken(TToken::TOKEN_AND));
			}
			else
			// Replace [SYMBOL("or")] with [OR]
			if (token.mParam.isString() &&
				token.mParam.stringValue()=="or") {
				mTokens.push_back(TToken(TToken::TOKEN_OR));
			}
			else if (token.mParam.isString()) {
				const mobilesdk_char* buffer = token.mParam.stringValue().Get();
				unsigned long length = token.mParam.stringValue().Length();
				bool isNumber=true;
				for (unsigned long c=0; c<length; ++c) {
					if (buffer[c]<0x0030 || buffer[c]>0x0039)
						if (c>0 || buffer[c]!=0x002d) {
							isNumber=false;
							break;
						}
				}
				if (!isNumber) {
					// Unresolved symbol!!
					return false;
				}
				mTokens.push_back(TToken(TToken::TOKEN_VAR, token.mParam.stringValue().ToLong()));
			}
			break;
		default:
			mTokens.push_back(token);
		}
	}
	return true;
}
bool SubExpressionImpl::infix2postfix() {
	msdk::vector<TToken> tmp;
	tmp.swap(mTokens);
	msdk::vector<TToken> stack;
	msdk::vector<TToken>::iterator it=tmp.begin();
	while(it!=tmp.end()) {
		TToken& token = *(it++);
		if (token.mType==TToken::TOKEN_VAR ||
			token.mType==TToken::TOKEN_ATTRIB ||
			token.mType==TToken::TOKEN_POSITION) {
			// Operand
			mTokens.push_back(token);
		}
		else if (token.mType==TToken::TOKEN_OPEN) {
			stack.push_back(token);
		}
		else if (token.mType==TToken::TOKEN_CLOSE && !stack.empty()) {
			while (stack.back().mType!=TToken::TOKEN_OPEN) {
				mTokens.push_back(stack.back());
				stack.pop_back();
			}
			stack.pop_back();
		}
		else {
			while (!stack.empty() && stack.back()>token) {
				mTokens.push_back(stack.back());
				stack.pop_back();
			}
			stack.push_back(token);
		}
	}
	while (!stack.empty()) {
		mTokens.push_back(stack.back());
		stack.pop_back();
	}
	return true;
}
SubExpression SubExpressionImpl::buildSubExpression(const DOMString& source) {
	SubExpression sub(new SubExpressionImpl());
	if (!(sub && sub->buildProgram(source)))
		return SubExpression();
	return sub;
}
XVar SubExpressionImpl::evaluate(const Node& n) const {
	msdk::vector<TToken> stack;
	msdk::vector<TToken>::const_iterator it=mTokens.begin();
	while(it!=mTokens.end()) {
		const TToken& token = *(it++);
		// variables
		if (token.mType==TToken::TOKEN_VAR)
			stack.push_back(token);
		// attrib("id")
		// @id
		else if (token.mType==TToken::TOKEN_ATTRIB) {
			Node attr = n->getAttribute(token.mParam.stringValue());
			if (attr)
				stack.push_back(TToken(TToken::TOKEN_VAR, attr->nodeValue()));
			else
				stack.push_back(TToken(TToken::TOKEN_VAR, ""));
		}
		// position()
		else if (token.mType==TToken::TOKEN_POSITION) {
			stack.push_back(TToken(TToken::TOKEN_VAR, n->nodePosition()));
		}
		// NOT
		else if (token.mType==TToken::TOKEN_NOT) {
			TToken param = stack.back(); stack.pop_back();
			if (param.mType==TToken::TOKEN_VAR) {
				bool v = param.mParam;
				if (param.mParam.isInteger())		v=(param.mParam.integerValue()!=0);
				else if (param.mParam.isString())	v=!param.mParam.stringValue().IsEmpty();
				else if (param.mParam.isNodeSet())	v=!param.mParam.nodeSetValue()->empty();
				param.mParam = XVar(v?0:1);
			}
			stack.push_back(param);
		}
		// binary operators
		else {
			TToken right = stack.back(); stack.pop_back();
			TToken left  = stack.back(); stack.pop_back();
			TToken result(TToken::TOKEN_VAR);
			if (left.mType==TToken::TOKEN_VAR && right.mType==TToken::TOKEN_VAR) {
				// equality
				if (token.mType==TToken::TOKEN_EQUAL || token.mType==TToken::TOKEN_NOT_EQUAL) {
					// if one of the terms is a string, turn the other term into a string as well
					// (except is it's a nodeset)
					if (left.mParam.isString() || right.mParam.isString()) {
						if (left.mParam.isInteger()) {
							DOMString asString;
							asString+=left.mParam.integerValue();
							left.mParam = XVar(asString);
						}
						if (right.mParam.isInteger()) {
							DOMString asString;
							asString+=right.mParam.integerValue();
							right.mParam = XVar(asString);
						}
					}
					// if and only if the two terms have the same type, check equality
					// else result holds an invalid value
					if (left.mParam.isInteger() && right.mParam.isInteger())
						result.mParam = XVar((left.mParam.integerValue()==right.mParam.integerValue())?1:0);
					else if (left.mParam.isString() && right.mParam.isString())
						result.mParam = XVar((left.mParam.stringValue()==right.mParam.stringValue())?1:0);
					else if (left.mParam.isNodeSet() && right.mParam.isNodeSet()) {
						bool same=false;
						const NodeListImpl& leftList	= *left.mParam.nodeSetValue();
						const NodeListImpl& rightList	= *right.mParam.nodeSetValue();
						if (leftList.size()==leftList.size()) {
							NodeListImpl::const_iterator l = leftList.begin();
							NodeListImpl::const_iterator r = rightList.begin();
							while (l!=leftList.end() && r!=rightList.end()) {
								const Node& leftNode  = *l; ++l;
								const Node& rightNode = *r; ++r;
								if (!leftNode->isSameNode(rightNode)) break;
							}
							same=true;
						}
						result.mParam = XVar(same?1:0);
					}
					// Apply NOT_EQUAL if result is valid
					if (token.mType==TToken::TOKEN_NOT_EQUAL && result.mParam.isInteger()) {
						result.mParam = XVar(result.mParam.integerValue()?0:1);
					}
				}
				// Logical
				else if (token.mType==TToken::TOKEN_AND || token.mType==TToken::TOKEN_OR) {
					bool l = left.mParam;
					if (left.mParam.isInteger())		l=(left.mParam.integerValue()!=0);
					else if (left.mParam.isString())	l=!left.mParam.stringValue().IsEmpty();
					else if (left.mParam.isNodeSet())	l=!left.mParam.nodeSetValue()->empty();
					bool r = right.mParam;
					if (right.mParam.isInteger())		r=(right.mParam.integerValue()!=0);
					else if (right.mParam.isString())	r=!right.mParam.stringValue().IsEmpty();
					else if (right.mParam.isNodeSet())	r=!right.mParam.nodeSetValue()->empty();

					if (token.mType==TToken::TOKEN_AND)	result.mParam = XVar((l&&r)?1:0);
					else								result.mParam = XVar((l||r)?1:0);
				}
				// Numbers comparison
				else if (left.mParam.isInteger() && right.mParam.isInteger()) {
					long l = left.mParam.integerValue();
					long r = right.mParam.integerValue();

					switch(token.mType) {
					case TToken::TOKEN_LESS:				result.mParam = XVar((l<r)?1:0);	break;
					case TToken::TOKEN_LESS_OR_EQUAL:		result.mParam = XVar((l<=r)?1:0);	break;
					case TToken::TOKEN_GREATER:				result.mParam = XVar((l>r)?1:0);	break;
					case TToken::TOKEN_GREATER_OR_EQUAL:	result.mParam = XVar((l>=r)?1:0);	break;
					}
				}
			}
			stack.push_back(result);
		}
	}
	if (stack.empty())
		return XVar();
	return XVar(stack[0].mParam);
}

XPathExpression::XPathExpression()
: mIterator(0)
, mFound(false)
, mMalformed(true)
{ }

XPathExpression::XPathExpression(const DOMString& expression)
: mIterator(0)
, mFound(false)
, mMalformed(true)
{
	// Compile expression
	msdk::vector<DOMString> levels = expression.Explode(L'/');
	msdk::vector<DOMString>::iterator it = levels.begin();
	while (it!=levels.end()) {
		DOMString& level = *it; ++it;
		msdk::vector<DOMString> tmp = level.Explode(L'[');
		if (!addTag(tmp[0]))
			return; // malformed
		SubExpression program;
		if (tmp.size()>1) {
			msdk::vector<DOMString> source = tmp[1].Explode(L']');
			if (source.size()>1) {
				program = SubExpressionImpl::buildSubExpression(source[0]);
			}
			if (!program)
				return; // malformed
		}
		if (!addSubExpression(program))
			return; // malformed
	}
	mMalformed=false;
}

XPathExpression::XPathExpression(const XPathExpression& o)
: mIterator(o.mIterator)
, mFound(o.mFound)
, mMalformed(o.mMalformed)
{
	msdk::vector<DOMString>::const_iterator tagIterator = o.mTags.begin();
	while(tagIterator!=o.mTags.end()) {
		mMalformed=mMalformed||mTags.push_back(*tagIterator); ++tagIterator;
	}
	msdk::vector<SubExpression>::const_iterator subIterator = o.mExpressions.begin();
	while(subIterator!=o.mExpressions.end()) {
		mMalformed=mMalformed||mExpressions.push_back(*subIterator); ++subIterator;
	}
	NodeListImpl::const_iterator resultIterator = o.mResult.begin();
	while(resultIterator!=o.mResult.end()) {
		mMalformed=mMalformed||mResult.push_back(*resultIterator); ++resultIterator;
	}
}

void XPathExpression::evaluate() {
	bool absolute=mTags.front().IsEmpty();
	if (absolute) {
		NodeListImpl newList;
		NodeConstIterator it = mResult.begin();
		while(it!=mResult.end()) {
			Document doc = (*it)->ownerDocument(); ++it;
			if (newList.push_back(doc->_myself())) return;
		}
		mResult.swap(newList);
	}

	// Remove duplicates


	msdk::vector<DOMString>::iterator		tagIterator		= mTags.begin();
	msdk::vector<SubExpression>::iterator	programIterator	= mExpressions.begin();
	unsigned long level=0;
	while (tagIterator!=mTags.end()) {
		const DOMString&	tag		= *(tagIterator++);
		SubExpression&		program	= *(programIterator++);
		if (absolute && (level==0)) {
			++level;
			continue;
		}
		bool all = (tag==msdk_char("*"));
		bool up  = (tag==msdk_char(".."));
		NodeListImpl tmpList;
		NodeListImpl::iterator node = mResult.begin();
		while (node!=mResult.end()) {
			Node parent = *(node++);
			if (up) {
				Node grandFather = parent->parentNode();
				if (grandFather) {
					if (program) {
						XVar result = program->evaluate(grandFather);
						if (result.isInteger() && (result.integerValue()!=0))
							tmpList.push_back(grandFather);
					}
					else tmpList.push_back(grandFather);
				}
			}
			else {
				Node child = parent->firstChild();
				while (child) {
					if (all||child->nodeName()==tag) {
						if (program) {
							XVar result = program->evaluate(child);
							if (result.isInteger() && (result.integerValue()!=0))
								tmpList.push_back(child);
						}
						else tmpList.push_back(child);
					}
					child=child->nextSibling();
				}
			}
		}
		tmpList.swap(mResult);
		++level;
	}
}

XPathExpression& XPathExpression::evaluateNode(const Node& contextNode) {
	if (!mMalformed) {
		// Clear
		mResult.clear();
		mIterator=0;
		mFound=false;
		if (mResult.push_back(contextNode))
			return *this;
		// Evaluate
		evaluate();
	}
	return *this;
}
XPathExpression& XPathExpression::evaluateNodes(const NodeList& contextNodes) {
	if (contextNodes) {
		NodeConstIterator it = contextNodes->begin();
		while(it!=contextNodes->end()) {

		}
	}
	return *this;
}

XPathExpression::operator bool() const {
	return !mMalformed;
}
bool operator>>(XPathExpression& xpe,Node& result) {
	if (!(xpe.mIterator<xpe.mResult.size()))
		return false;
	result = xpe.mResult[xpe.mIterator++];
	return true;
}

bool operator>>(XPathExpression& xpe,NodeList& result) {
	if (xpe.mResult.empty())
		return false;
	if (!result) result.reset(new NodeListImpl);
	if (!result) return false;
	*result = xpe.mResult;
	return true;
}

Node XPathExpression::operator[](unsigned long index) const {
	if (mResult.size()>index && index>=0) {
		return mResult[index];
	}
	return Node();
}

unsigned long XPathExpression::nodeCount() const {
	return mResult.size();
}

// ------------------------ old SingleValueQuery class ----------------------------

static bool evaluateExpression(Node Node, const DOMString& expression, msdk::list<DOMString>* errors) {
	if (expression.IsEmpty()) return true;
	if (errors) errors->push_back("Expressions not supported right now");
	return false;
}

static void applyOnNode(Node node, const DOMString& tagName, const DOMString& expression, NodeListImpl& output, msdk::list<DOMString>* errors) {
	if (!node) return;
	Node child = node->firstChild();
	bool all = (tagName.IsEmpty() || (tagName==msdk_char("*")));
	while (child) {
		if (all || (child->nodeName()==tagName)) {
			if (evaluateExpression(child, expression, errors)) output.push_back(child);
		}
		child = child->nextSibling();
	}
}

static void applyOnNodes(NodeListImpl& elements, const DOMString& criterium, msdk::list<DOMString>* errors) {
	if (criterium.IsEmpty()) return;

	DOMString tagName;
	DOMString expression;
	unsigned long LSB = criterium.Find(L'[');
	unsigned long RSB = criterium.Find(L']');
	if (LSB!=DOMString::npos && RSB!=DOMString::npos) {
		if (LSB>RSB) {
			if (errors) errors->push_back("Bad expression: square bracket mismatch!");
			return;
		}
	}
	else tagName=criterium;

	NodeListImpl tmpList;
	NodeListImpl::iterator it = elements.begin();
	while (it!=elements.end())
		applyOnNode(*(it++), tagName, expression, tmpList, errors);
	elements.swap(tmpList);
}


NodeList SQuery(Node root, DOMString query, msdk::list<DOMString>* errors) {
	msdk::vector<DOMString> queryElements = query.Explode(L'/');

	if (queryElements.empty()) {
		if (errors) errors->push_back(DOMString("Empty query"));
		return NodeList();
	}
	if (queryElements.size()==1 && queryElements[0].IsEmpty()) {
		if (errors) errors->push_back(DOMString("'/' is not a valid query"));
		return NodeList();
	}

	NodeList result(new NodeListImpl());
	if (!result || result->push_back(root)) {
		if (errors) errors->push_back(DOMString("Memory error"));
		return NodeList();;
	}

	msdk::vector<DOMString>::iterator it = queryElements.begin();
	while (it!=queryElements.end()) applyOnNodes(*result, *(it++), errors);
	return result;
}

// -- SingleValueQuery -----------------------------------------------------

SingleValueQuery::SingleValueQuery(dom::Node _root)
: root(_root)
, found(false)
{ }

const SingleValueQuery& SingleValueQuery::operator()(const DOMString& path) {
	found=false;
	result.Clear();
	dom::NodeList nodes = dom::SQuery(root, path, 0);
	if (!nodes->empty())
		found=nodes->back()->textContent(result);
	return *this;
}

bool operator>>(const SingleValueQuery& svq, DOMString& result) {
	if (svq) result=svq.result;
	return svq;
}

bool operator>>(const SingleValueQuery& svq, unsigned short& result) {
	if (svq) result=(unsigned short)svq.result.ToULong();
	return svq;
}
bool operator>>(const SingleValueQuery& svq,unsigned long& result) {
	if (svq) result=svq.result.ToULong();
	return svq;
}
bool operator>>(const SingleValueQuery& svq,short& result) {
	if (svq) result=(short)svq.result.ToLong();
	return svq;
}
bool operator>>(const SingleValueQuery& svq,long& result) {
	if (svq) result=svq.result.ToLong();
	return svq;
}
bool operator>>(const SingleValueQuery& svq,float& result) {
	if (svq) result=svq.result.ToFloat();
	return svq;
}
bool operator>>(const SingleValueQuery& svq,bool& result) {
	if (svq) {
		msdk::CString v(svq.result);
		v.ToLowerCase();
		if (v.IsEmpty()) {
			result=false;
		}
		else if ((v=="no") || (v=="false")) {
			result=false;
		}
		else if ((v=="yes") || (v=="true")) {
			result=true;
		}
		else {
			result=(v.ToULong()>0);
		}
	}
	return svq;
}

} // namespace dom
