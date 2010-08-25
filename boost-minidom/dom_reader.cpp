// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/archive/iterators/istream_iterator.hpp>
#include <sstream>
#include <algorithm>
#include "dom_reader.hpp"
#include "dom_document.hpp"
#include "dom_utils.hpp"
#include "utf8_codecvt_facet.hpp"
using namespace dom;

namespace {
    static const wchar_t CHAR_NORMALIZED_SPACE  = 0x0020;
    static const wchar_t CHAR_SEMICOLON         = 0x003b;
    static const wchar_t CHAR_NUMBER_SIGN       = 0x0023;
    static const wchar_t CHAR_X                 = 0x0078;
    static const wchar_t CHAR_EQUAL_SIGN        = 0x003d;
    static const wchar_t CHAR_QUOTE             = 0x0022;
    static const wchar_t CHAR_TAB               = 0x0009;
    static const wchar_t CHAR_QUESTION_MARK     = 0x003f;
    static const wchar_t CHAR_EXCLAMATION_MARK  = 0x0021;
    static const wchar_t CHAR_SLASH             = 0x002f;
    static const wchar_t CHAR_BACKSLASH         = 0x005c;
    static const wchar_t CHAR_MINUS             = 0x002d;
    static const wchar_t CHAR_LESS_THAN         = 0x003c;
    static const wchar_t CHAR_GREATER_THAN      = 0x003e;
    static const wchar_t CHAR_AMPERSAND         = 0x0026;

    // This one is used in parseAttribute to count the back slashes
    // and to determine if a quote/doublequote is escaped or not
    static bool notABackSlash(wchar_t c) {
        return (c!=CHAR_BACKSLASH);
    }

    static bool isSpace(wchar_t c) {
        return (DOMUtils::isLineBreakingSpace(c) || DOMUtils::isWhiteSpace(c));
    }
}; // anonymous

DocumentPtr DOMReader::readDocument(const std::wstring& filename) {
    // Open an input stream with a UTF-8 locale
    std::locale utf8_locale(std::locale(),new dom::utf8::utf8_codecvt_facet());
    boost::filesystem::wpath_traits::imbue(utf8_locale);
    boost::filesystem::wpath filepath(filename);

    // Read the whole content
    // FIXME: I could use an istream_iterator instead of this
    // "fileContent" string, if I get rid of the few random access
    // manipulations below: the following makes a copy that could/
    // shoud be avoided...
    boost::filesystem::wifstream ifs(filepath);
    std::wostringstream oss;
    oss << ifs.rdbuf();
    std::wstring fileContent = oss.str();
    ifs.close();

    // Create a new document object
    m_oDocument = DOMImplementation::createDocument();
    m_oDocument->setEncoding(L"utf-8");

    m_oNodeStack.push_front(m_oDocument);
    bool lastEscape      = false;
    bool insideLTGT      = false;	// we're inside a < ... > block
    bool insideLitteral  = false;	// we're inside a " ... " block
    std::wstring readBuffer;
    std::wstring::const_iterator it = fileContent.begin();
    while (it != fileContent.end()) {
        std::wstring::const_iterator currentPosition = it++;
        wchar_t currentChar = *currentPosition;
        bool lt	    = (currentChar==CHAR_LESS_THAN);
        bool gt	    = (currentChar==CHAR_GREATER_THAN);
        bool quote  = (currentChar==CHAR_QUOTE);
        bool escape = (currentChar==CHAR_BACKSLASH);

        // Opening a < ... > block
        if (lt && !insideLitteral) {
            // Validity check
            if (insideLTGT) {
                // Opening a new tag but another is already opened!
                throw DOMException(DOMException::SYNTAX_ERR);
            }
            // If readBuffer is not empty, add it to the current element as a Text child
            if (!readBuffer.empty()) {
                // Context check
                NodePtr currentNode = m_oNodeStack.front();
                if (currentNode->nodeType()!=Node::ELEMENT_NODE) {
                    // Text found at top level
                    throw DOMException(DOMException::NO_DATA_ALLOWED_ERR);
                }
                Element* currentImpl = reinterpret_cast<Element*>(currentNode.get());
                // Add new Text node to current Element
                readBuffer.push_back(0);
                NodePtr newNode = m_oDocument->createTextNode(&readBuffer[0]);
                readBuffer.clear();
                currentImpl->appendChild(newNode);
            }
            // We're inside a < ... > block !
            insideLTGT=true;
            // Restart from here
            continue;
        }

	    // Closing a < ... > block
	    if (gt && !insideLitteral) {
		    // Validity check
            if (!insideLTGT) {
			    // Closing a tag but nothing has been opened
                throw DOMException(DOMException::SYNTAX_ERR);
            }
            DOMUtils::trimSpaces(readBuffer);
            parseAngleBracketBlock(readBuffer);
            readBuffer.clear();
            // We're exiting a < ... > block !
            insideLTGT=false;
            // Restart from here
            continue;
        }

        // Find if we're in a litteral or not
        if (quote) {
            if (insideLTGT && !lastEscape)
                insideLitteral=!insideLitteral;
        }
        lastEscape=(escape && insideLitteral);

		// Are we dealing with some white space?
        // NB: I check this before decoding entities so that
        // any explicit white space gets preserved.
        bool ws	= DOMUtils::isWhiteSpace(currentChar);

        // Translate any found entity into the current character
        bool entity	= (currentChar==CHAR_AMPERSAND);
        if (entity) {
            // Find trailing semicolon
            std::wstring::size_type match = fileContent.find((wchar_t)CHAR_SEMICOLON,currentPosition-fileContent.begin());
            if (match==fileContent.npos) {
                // Entity not closed!
                throw DOMException(DOMException::SYNTAX_ERR);
            }
            // get entity
            std::wstring::size_type len = match + 1 - (currentPosition-fileContent.begin());
            std::wstring entityValue = fileContent.substr(currentPosition-fileContent.begin(),len);
            currentChar=DOMUtils::translateXMLEntity(entityValue);
            it+=len-1; currentPosition+=len;
        }

        // Outside quotes, any group of white space ultimately form one
        // unique space character
        if (ws && !insideLitteral) {
            if (!insideLTGT) continue;
            if (!readBuffer.empty() && (*readBuffer.rbegin()==CHAR_NORMALIZED_SPACE)) continue;
            currentChar = CHAR_NORMALIZED_SPACE;
        }
        // Add current character to our read buffer
        if (entity && (currentChar==CHAR_QUOTE))
            readBuffer.push_back(CHAR_BACKSLASH); // quote character if it's a quote
        readBuffer.push_back(currentChar);
    }
    DOMUtils::trimSpaces(readBuffer);
    if (!readBuffer.empty()) {
		// Garbage at end of XML file
        throw DOMException(DOMException::NO_DATA_ALLOWED_ERR);
    }
    return m_oDocument;
}

void DOMReader::parseAngleBracketBlock(std::wstring& data) {
    if (m_oNodeStack.empty()) {
        // Something terribly wrong happened!
        throw DOMException(DOMException::INVALID_STATE_ERR);
    }
    std::wstring::size_type length = data.length();
    if (!length) {
        throw DOMException(DOMException::SYNTAX_ERR);
    }
    else if (0==data.find(CHAR_QUESTION_MARK) && (length-1)==data.rfind(CHAR_QUESTION_MARK)) {
        // <? ... ?> Block

        data.erase(length-1,1);
        data.erase(0,1);
        DOMUtils::trimSpaces(data);

        parseNewHeader(data);
    }
    else if (0==data.find(L"!--") && (length-2)==data.rfind(L"--")) {
        // <!-- ... --> Block

        data.erase(length-2,2);
        data.erase(0,3);
        DOMUtils::trimSpaces(data);

        // TODO: Add support for COMMENT_NODE
    }
    else if (0==data.find(CHAR_EXCLAMATION_MARK)) {
        // <! > Block

        data.erase(0,1);

        parseNewDTDElement(data);
    }
    else if (length-1==data.rfind(CHAR_SLASH)) {
        // < ... /> Block

        data.erase(length-1,1);
        DOMUtils::trimSpaces(data);

        // create then close tag since it doesn't have any child
        parseNewTag(data);
        m_oNodeStack.pop_front();
    }
    else if (0==data.find(CHAR_SLASH)) {
        // </ ... > Block

        data.erase(0,1);
        DOMUtils::trimSpaces(data);

        closeTag(data);
        return;
    }
    else {
        // < ... > Block

        parseNewTag(data);
    }
}
void DOMReader::parseNewHeader(std::wstring& data) {
    if (m_oNodeStack.empty()) {
        // Stack corruption
        throw DOMException(DOMException::INVALID_STATE_ERR);
    }
    if (0==data.find(L"xml")) {
        if (m_oNodeStack.front()->isSameNode(m_oDocument)) {
            data.erase(0,3);
            DOMUtils::trimSpaces(data);
            parseAttributes(data);
        }
        else {
            // XML header tag found while not at top level
            throw DOMException(DOMException::INVALID_STATE_ERR);
        }
    }
}
void DOMReader::parseNewDTDElement(std::wstring& data) {
    if (!m_oNodeStack.front()->isSameNode(m_oDocument)) {
        // DTD element found while not at top level
        throw DOMException(DOMException::SYNTAX_ERR);
    }
    if (0==data.find(L"DOCTYPE")) {
        data.erase(0,7);
        DOMUtils::trimSpaces(data);
        if (data.empty()) {
            // Empty DOCTYPE
            throw DOMException(DOMException::SYNTAX_ERR);
        }
        m_oDocument->setDocType(data);
    }
}


void DOMReader::parseNewTag(std::wstring& data) {
    if (data.empty()) {
        // Empty tag name
        throw DOMException(DOMException::SYNTAX_ERR);
    }
    std::wstring::iterator it = std::find_if(data.begin(),data.end(),isSpace);
    if (it!=data.end()) {
        // tag has attributes

        std::wstring tagName = data.substr(0,it-data.begin());
        parseNewElement(tagName);

        data.erase(0,it-data.begin());
        DOMUtils::trimSpaces(data);
		parseAttributes(data);

    }
    else {
        parseNewElement(data);
    }
}
void DOMReader::parseNewElement(std::wstring& data) {
    if (m_oNodeStack.empty()) {
        // Stack corruption
        throw DOMException(DOMException::INVALID_STATE_ERR);
    }
    if (data.empty()) {
        // Empty tag name
        throw DOMException(DOMException::SYNTAX_ERR);
    }

    NodePtr node = m_oDocument->createElement(data);
    NodePtr parent = m_oNodeStack.front();
    if (parent->nodeType()!=Node::DOCUMENT_NODE && parent->nodeType()!=Node::ELEMENT_NODE) {
        // Only Element and Document nodes can have children
        throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
    }
    Element* parentImpl = reinterpret_cast<Element*>(parent.get());
    NodePtr added = parentImpl->appendChild(node);
    m_oNodeStack.push_front(added);
}
void DOMReader::parseAttributes(std::wstring& data) {
    if (m_oNodeStack.empty()) {
        // Stack corruption
        throw DOMException(DOMException::INVALID_STATE_ERR);
    }
    while (!data.empty()) {
        // read attribute name
        std::wstring::size_type e = data.find(CHAR_EQUAL_SIGN);
        if (e==data.npos) {
            // XML does not accept no-value attributes
            throw DOMException(DOMException::SYNTAX_ERR);
        }
        std::wstring attributeName = data.substr(0,e);
        DOMUtils::trimSpaces(attributeName);
        data.erase(0,e+1);
        DOMUtils::trimSpaces(data);
        // read attribute value
        if (data.empty()) {
            // Nothing after "="...
            throw DOMException(DOMException::SYNTAX_ERR);
        }
        wchar_t quote = data[0];
        if (quote!=L'"' && quote!=L'\'') {
            // Not a valid quote character...
            throw DOMException(DOMException::INVALID_CHARACTER_ERR);
        }
        std::wstring::size_type q = 0;
        for(;;) {
            // search a matching, unescaped quote
            q = data.find(quote,q+1);
            if (q==data.npos) {
                // No matching quote
                throw DOMException(DOMException::SYNTAX_ERR);
            }
            // there MUST be an EVEN (0 incl) number of back slashes
            // before the closing quote to take it in account,
            // otherwise, it means it is escaped
            std::wstring::size_type p = q;
            while(p>0 && data[p-1]==CHAR_BACKSLASH) --p;
            if (((q-p)&1)==0) break;
        };
        std::wstring attributeValue = data.substr(1,q-1);

        // Remove value from input (incl the trailing quote)
        data.erase(0,q+1);
        DOMUtils::trimSpaces(data);

        // Set attribute
        NodePtr node = m_oNodeStack.front();
        if (node->nodeType()!=Node::ELEMENT_NODE && node->nodeType()!=Node::DOCUMENT_NODE) {
            // Only Elements can hold atributes
            throw DOMException(DOMException::INVALID_STATE_ERR);
        }

        Element* nodeImpl = reinterpret_cast<Element*>(node.get());
        nodeImpl->setAttribute(attributeName, attributeValue);
	}
}
void DOMReader::closeTag(std::wstring& data) {
    if (data.empty()) {
        // Empty tag name
        throw DOMException(DOMException::SYNTAX_ERR);
    }
    if (m_oNodeStack.empty()) {
        // Bad, bad thing!
        throw DOMException(DOMException::INVALID_STATE_ERR);
    }
    NodePtr currentElement = m_oNodeStack.front();
    if (data!=currentElement->nodeName()) {
        // Closing a tag which is not the current one (stack corruption)
        throw DOMException(DOMException::INVALID_STATE_ERR);
    }
    m_oNodeStack.pop_front();
}
