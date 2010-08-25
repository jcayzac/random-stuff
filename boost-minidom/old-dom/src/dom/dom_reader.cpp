// $Id: dom_reader.cpp 8630 2007-12-04 17:40:17Z jcayzac $
#include <llsdk/inc/dom/dom.h>
#include <llsdk/inc/dom/xml_entities.h>
#include <llsdk/inc/dom/dom_reader.h>
#include <llsdk/inc/text/textreader_latin9.h>
#include <llsdk/inc/text/textreader_windows1252.h>
#include <llsdk/inc/text/textreader_utf8.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

static const mobilesdk_char CHAR_NORMALIZED_SPACE	= 0x0020;
static const mobilesdk_char CHAR_SEMICOLON			= 0x003b;
static const mobilesdk_char CHAR_NUMBER_SIGN		= 0x0023;
static const mobilesdk_char CHAR_X					= 0x0078;
static const mobilesdk_char CHAR_EQUAL_SIGN			= 0x003d;
static const mobilesdk_char CHAR_QUOTE				= 0x0022;
static const mobilesdk_char CHAR_TAB				= 0x0009;
static const mobilesdk_char CHAR_QUESTION_MARK		= 0x003f;
static const mobilesdk_char CHAR_EXCLAMATION_MARK	= 0x0021;
static const mobilesdk_char CHAR_SLASH				= 0x002f;
static const mobilesdk_char CHAR_BACKSLASH			= 0x005c;
static const mobilesdk_char CHAR_MINUS				= 0x002d;
static const mobilesdk_char CHAR_LESS_THAN			= 0x003c;
static const mobilesdk_char CHAR_GREATER_THAN		= 0x003e;
static const mobilesdk_char CHAR_AMPERSAND			= 0x0026;

static msdk::CString getStringFromBuffer(const mobilesdk_char* data, unsigned long length) {
	msdk::vector<mobilesdk_char> tmp;
	if (tmp.reserve(length+1))
		return msdk::CString();
	for (unsigned long c=0; c<length; ++c)
		tmp.push_back(data[c]);
	tmp.push_back(0);
	return msdk::CString(&tmp[0]);
}

// Parser

CDOMReader::CDOMReader(const msdk::shared_ptr<msdk::IInputStream>& in, text::TEncoding encoding)
: m_bConstructed(false)
, m_bInValue(false)
, m_pAsyncListener(0) {
	m_bConstructed = construct(in,encoding);
}

CDOMReader::~CDOMReader() {
	stopTransaction(false);
}

bool CDOMReader::construct(const msdk::shared_ptr<msdk::IInputStream>& in, text::TEncoding encoding) {
	m_oTextReader.Reset(text::CTextReader::NewTextReader(in,encoding));
	if (!m_oTextReader) return false;
	return true;
}

bool CDOMReader::CloseTag(mobilesdk_char* data, unsigned long length) {
	text::TrimSpaces(data,length);
	if (!length)
		return FatalError("Empty tag name");
	msdk::CString tagName=getStringFromBuffer(data,length);
	Node currentElement = m_oNodeStack.front();
	const msdk::CString& currentName = currentElement->nodeName();
	if (currentName!=tagName)
		return FatalError("Closing a tag which is not the current one (stack corruption)");
	m_oNodeStack.pop_front();
	return true;
}

bool CDOMReader::ParseNewElement(mobilesdk_char* data, unsigned long length) {
	text::TrimSpaces(data,length);
	if (!length)
		return FatalError("Empty tag name");
	msdk::CString tagName=getStringFromBuffer(data,length);

	Node node = m_oDocument->createElement(tagName);
	if (node) {
		Node parent = m_oNodeStack.front();
		if (parent->nodeType()==NodeImpl::DOCUMENT_NODE || parent->nodeType()==NodeImpl::ELEMENT_NODE) {
			ElementImpl* parentImpl = reinterpret_cast<ElementImpl*>(parent.get());
			Node added = parentImpl->appendChild(node);
			if (added) {
				if (m_oNodeStack.push_front(added)==0)
					return true;
				return FatalError("Memory allocation failure");
			}
			return FatalError("Could not append node to its parent Element");
		}
		return FatalError("Only Element and Document nodes can have children");
	}
	return FatalError("Could not create new element");
}
bool CDOMReader::ParseAttributes(mobilesdk_char* data, unsigned long length) {
	if (m_oNodeStack.empty())
		return FatalError("Stack corruption");
	text::TrimSpaces(data,length);
	if (!length)
		return FatalError("Empty attribute set");
	unsigned long pos=0;
	unsigned long nameStart=0;
	unsigned long nameLength=0;
	unsigned long valueStart=0;
	bool escaped=false;
	while(pos<length) {
		if (pos && (nameLength==0) && (data[pos]==CHAR_EQUAL_SIGN)) {
			// before pos lies the attribute name
			nameLength=pos-nameStart-1;
			while (nameLength && data[nameLength]==CHAR_NORMALIZED_SPACE) --nameLength;
			++nameLength;
			++pos;
			continue;
		}
		if (data[pos]==CHAR_QUOTE) {
			if (nameLength) {
				if (valueStart==0) {
					// after lies the value
					valueStart=pos+1;
					if (valueStart>=length-1)
						return FatalError("Missing attribute value");
				}
				else if (!escaped) {
					// end of value!
					unsigned long valueLength = pos-valueStart;
					// name:	data[nameStart], nameLength
					// value:	data[valueStart], valueLength
					msdk::CString name		= getStringFromBuffer(&data[nameStart],nameLength);
					if (name.IsEmpty())
						return FatalError("Attribute name is empty");

					msdk::CString valueTmp	= getStringFromBuffer(&data[valueStart],valueLength);
					msdk::CString value;
					bool esc=false;
					for (unsigned long c=0; c<valueTmp.Length(); ++c) {
						if (valueTmp[c]==CHAR_BACKSLASH) {
							if (!esc) {
								esc=true;
								continue;
							}
						}
						mobilesdk_char a[2]; a[0]=valueTmp[c]; a[1]=0;
						value+=a;
					}
					
					Node node = m_oNodeStack.front();
					if (node->nodeType()!=NodeImpl::ELEMENT_NODE && node->nodeType()!=NodeImpl::DOCUMENT_NODE)
						return FatalError("Only Elements can hold atributes");
					ElementImpl* nodeImpl = reinterpret_cast<ElementImpl*>(node.get());
					nodeImpl->setAttribute(name, value);

					// If it's the document and attribute is "encoding"
					if (node->isSameNode(m_oDocument) && name=="encoding") {
						RedecodeTextBuffer(value);
					}

					// jump to next name...
					++pos;
					while(pos<length && data[pos]==CHAR_NORMALIZED_SPACE) ++pos;
					if (pos<length) {
						nameStart	= pos;
						nameLength	= 0;
						valueStart	= 0;
						++pos;
						continue;
					}
				}
			}
		}
		escaped=(data[pos]==CHAR_BACKSLASH);
		++pos;
	}
	return true;
}
bool CDOMReader::ParseNewHeader(mobilesdk_char* data, unsigned long length) {
	if (m_oNodeStack.empty())
		return FatalError("Stack corruption");
	if (m_oNodeStack.front()->isSameNode(m_oDocument)) {
		// skip "xml"
		if (length>=3)
			return ParseAttributes(data+3, length);
	}
	return FatalError("XML header tag found while not at top level");
}
bool CDOMReader::ParseNewTag(mobilesdk_char* data, unsigned long length) {
	text::TrimSpaces(data,length);
	if (!length)
		return FatalError("Empty tag name");
	unsigned long breakPos=0;
	mobilesdk_char* origData   = data;
	unsigned long   origLength = length;
	bool parsedElement=false;
	while(length && (breakPos<length)) {
		if (data[breakPos]==CHAR_NORMALIZED_SPACE) {
			data[breakPos]=0;
			// parse element
			if (!ParseNewElement(data,breakPos))
				return false;
			length-=breakPos;
			data=&data[breakPos];
			if (length) {
				--length;
				++data;
				if (!ParseAttributes(data,length))
					return false;
			}
			parsedElement=true;
			break;
		}
		else ++breakPos;
	}
	if (!parsedElement)
		return ParseNewElement(origData,origLength);
	return true;
}

bool CDOMReader::ParseNewDTDElement(mobilesdk_char* data, unsigned long length) {
	if (!m_oNodeStack.front()->isSameNode(m_oDocument))
		return FatalError("DTD element found while not at top level");
	// Trim spaces
	text::TrimSpaces(data,length);
	if (!length)
		return FatalError("Empty DTD element");
	if (length<8) return true;
	if (data[0]!=L'D' || 
		data[1]!=L'O' || 
		data[2]!=L'C' || 
		data[3]!=L'T' || 
		data[4]!=L'Y' || 
		data[5]!=L'P' || 
		data[6]!=L'E' ||
		data[7]!=CHAR_NORMALIZED_SPACE) return true;
	data+=8;
	length-=8;
	// Ok, we've found a doctype!
	// 'html PUBLIC "url" "url" ...'
	m_oDocument->setDocType(getStringFromBuffer(data,length));
	return true;
}

bool CDOMReader::ParseAngleBracketBlock(msdk::vector<mobilesdk_char>& data) {
	if (m_oNodeStack.empty()) return false;
	mobilesdk_char* blockPtr	= &data[0];
	unsigned long length		= data.size();

	text::TrimSpaces(blockPtr,length);
	if (!length)
		return FatalError("Empty < ... > block");

	// Select proper parser
	
	// XML Header parser
	if (length>=2) {
		if ((blockPtr[0]==CHAR_QUESTION_MARK) && (blockPtr[length-1]==CHAR_QUESTION_MARK)) {
			// <? ... ?> Block
			++blockPtr;
			length-=2;
			return ParseNewHeader(blockPtr,length);
		}
	}

	// Comment parser (ignore)
	if (length>=5) {
		if ((blockPtr[0]==CHAR_EXCLAMATION_MARK) &&
							(blockPtr[1]==CHAR_MINUS) &&
							(blockPtr[2]==CHAR_MINUS) &&
							(blockPtr[length-2]==CHAR_MINUS) &&
							(blockPtr[length-1]==CHAR_MINUS)) {
			// <!-- ... --> Block
			return true;
		}
	}

	// DTD element parser (ignore)
	if (length>=1) {
		if (blockPtr[0]==CHAR_EXCLAMATION_MARK) {
			// <! > Block
			++blockPtr;
			--length;
			return ParseNewDTDElement(blockPtr,length);
		}
	}

	// Self-contained element parser
	if (length>=1) {
		if (blockPtr[length-1]==CHAR_SLASH) {
			// < ... /> Block
			--length;
			bool res = ParseNewTag(blockPtr,length);
			if (res) m_oNodeStack.pop_front();
			return res;
		}
	}

	// </ ... > Block
	if (length>=1) {
		if (blockPtr[0]==CHAR_SLASH) {
			++blockPtr;
			--length;
			return CloseTag(blockPtr, length);
		}
	}

	// < ... > Block
	return ParseNewTag(blockPtr,length);
}

Document CDOMReader::readDocument() {
	if (!m_bConstructed) return Document();
	m_pTextDocument.clear();
	// Fill the text buffer completely
	mobilesdk_char c;
	while (m_oTextReader->readCharacter(c))
		m_pTextDocument.push_back(c);
	// Parse buffer
	parseTextBuffer();
	return m_oDocument;
}

bool CDOMReader::parseTextBuffer() {
	m_oNodeStack.clear();
	m_oDocument = DOMImplementation::createDocument();
	m_oDocument->setEncoding(m_oTextReader->encoding());
	m_ulCurrentLineNumber = 0;

	if (!m_oDocument)
		return FatalError("Could not create Document object");
	if (m_oNodeStack.push_front(m_oDocument))
		return FatalError("Could not set Document object as tree root");

	// Parse Document's top level, then create the document Element and call parseElement() on it.
	mobilesdk_char* currentAddress	= &m_pTextDocument[0];
	unsigned long   textLength		= m_pTextDocument.size();
	msdk::vector<mobilesdk_char>	readBuffer;

	bool lastEscape		= false;
	bool insideLTGT		= false;	// we're inside a < ... > block
	bool insideLitteral	= false;	// we're inside a " ... " block
	while (textLength) {
		mobilesdk_char currentChar = *(currentAddress++);
		--textLength;
		if (text::IsLineBreakingSpace(currentChar)) ++m_ulCurrentLineNumber;
		bool lt		= (currentChar==CHAR_LESS_THAN);
		bool gt		= (currentChar==CHAR_GREATER_THAN);
		bool quote	= (currentChar==CHAR_QUOTE);
		bool escape	= (currentChar==CHAR_BACKSLASH);

		// Opening a < ... > block
		if (lt && !insideLitteral) {
			// Validity check
			if (insideLTGT)
				return FatalError("Opening a new tag but another is already opened");

			// If readBuffer is not empty, add it to the current element as a Text child
			if (!readBuffer.empty()) {
				// Context check
				Node currentNode = m_oNodeStack.front();
				if (currentNode->nodeType()!=NodeImpl::ELEMENT_NODE)
					return FatalError("Text found at top level");
				ElementImpl* currentImpl = reinterpret_cast<ElementImpl*>(currentNode.get());
				// Add new Text node to current Element
				readBuffer.push_back(0);
				Node newNode = m_oDocument->createTextNode(&readBuffer[0]);
				readBuffer.clear();
				if (!newNode)
					return FatalError("Cannot create Text node");
				Node added = currentImpl->appendChild(newNode);
				if (!added)
					return FatalError("Cannot append Text node to current Element");
			}
			// We're inside a < ... > block !
			insideLTGT=true;
			// Restart from here
			continue;
		}

		// Closing a < ... > block
		if (gt && !insideLitteral) {
			// Validity check
			if (!insideLTGT)
				return FatalError("Closing a tag but nothing has been opened");
			if (!ParseAngleBracketBlock(readBuffer))
				return FatalError("Failed to parse block");
			readBuffer.clear();
			// We're exiting a < ... > block !
			insideLTGT=false;
			// Restart from here
			continue;
		}

		// Find if we're in a litteral or not
		// (and properly handle escaped quotes)
		if (quote) {
			if (insideLTGT && !lastEscape)
				insideLitteral=!insideLitteral;
		}
		lastEscape=(escape && insideLitteral);

		// Are we dealing with some white space?
		bool ws		= text::IsWhiteSpace(currentChar);

		// Translate any found entity into the current character
		bool entity	= (currentChar==CHAR_AMPERSAND);
		if (entity)
			currentChar=GetXMLEntity(currentAddress,textLength);

		// Outside quotes, any group of white space ultimately for one unique space character
		// (white space entities such as &nbsp; or &#x0020; are not part of any group)
		if (ws && !insideLitteral) {
			if (!insideLTGT) continue;
			if (!readBuffer.empty() && (readBuffer.back()==CHAR_NORMALIZED_SPACE)) continue;
			currentChar = CHAR_NORMALIZED_SPACE;
		}

		// Add current character to our read buffer
		if (entity && (currentChar==CHAR_QUOTE))
			if (readBuffer.push_back(CHAR_BACKSLASH)) // quote character if it's a quote
				return MemoryError();
		if (readBuffer.push_back(currentChar))
			return MemoryError();
	}
	if (readBuffer.size())
		return FatalError("Garbage at end of XML file");
	return m_oDocument;
}

void CDOMReader::RedecodeTextBuffer(const msdk::CString& encodingName) {
	if (m_pTextDocument.empty()) return;
	msdk::CString lowerCased(encodingName);
	lowerCased.ToLowerCase();

	// Latin1 -> Latin9
	if (lowerCased=="latin-9" ||
		lowerCased=="latin9" ||
		lowerCased=="iso-8859-15" ||
		lowerCased=="iso8859-15") {
		text::TLatin9Reader::translateLatin1(&m_pTextDocument[0], m_pTextDocument.size());
		m_oDocument->setEncoding(text::ENCODING_LATIN9);
		return;
	}

	// Latin1 -> Windows-1252
	if (lowerCased=="windows-1252") {
		text::TWindows1252Reader::translateLatin1(&m_pTextDocument[0], m_pTextDocument.size());
		m_oDocument->setEncoding(text::ENCODING_WINDOWS_1252);
		return;
	}

	// Latin1 -> UTF-8
	if (lowerCased=="utf-8" ||
			 lowerCased=="utf8" ) {
		text::TUtf8Reader::translateLatin1(&m_pTextDocument[0], m_pTextDocument.size());
		// Remove pad characters
		while (m_pTextDocument.size()>0 && m_pTextDocument[0]==0)
			m_pTextDocument.pop_back();
		m_oDocument->setEncoding(text::ENCODING_UTF8);
		return;
	}
}

bool CDOMReader::FatalError(const char* message) {
	msdk::CString m;
	if (m_ulCurrentLineNumber)
		m = msdk::CString::Format(msdk_char("[Line %d] "), m_ulCurrentLineNumber);
	m+=msdk::CString(message);
	m_oErrors.push_back(m);
	m_oDocument=Document();
	m_oNodeStack.clear();
	return false;
}

bool CDOMReader::MemoryError() {
	return FatalError("Memory error");
}

bool CDOMReader::startTransaction(IDOMReaderAsyncListener* listener) {
	if (!m_bConstructed) return false;
	if (m_pAsyncListener && (m_pAsyncListener!=listener))
		return false;
	m_pAsyncListener = listener;
	return m_oTextReader->startTransaction(this);
}
void CDOMReader::stopTransaction(bool silent) {
	if (!m_bConstructed) return;
	if (!m_pAsyncListener) return;
	m_oTextReader->stopTransaction(silent);
	m_pAsyncListener=0;
}

bool CDOMReader::OnAsyncCharacters(const mobilesdk_char* buffer, unsigned long length, int returncode) {
	switch (returncode) {
	case ITextReaderAsyncListener::TR_ENDOFFILE:
		// Parse received buffer
		parseTextBuffer();
		if (m_pAsyncListener)
			m_pAsyncListener->OnDOMDocumentComplete(m_oDocument, IDOMReaderAsyncListener::TR_NOERROR);
		break;
	case ITextReaderAsyncListener::TR_STOPPED:
		if (m_pAsyncListener) m_pAsyncListener->OnDOMDocumentComplete(Document(), IDOMReaderAsyncListener::TR_STOPPED);
		break;
	default:
		if (buffer && length) {
			while (length--)
				m_pTextDocument.push_back(*(buffer++));
		}
	}
	return true;
}

bool CDOMReader::next(msdk::list<msdk::CString>& l, msdk::CString& m) {
	if (l.empty()) return false;
	m = l.front();
	l.erase(l.begin());
	return true;
}


} // namespace dom
