// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(DOM_READER_INCLUDED)
#define DOM_READER_INCLUDED

#include <string>
#include <vector>
#include <list>
#include "dom_types.hpp"

namespace dom {

/** A DOMReader object is used to load a DOM tree
  * from an XML-formatted file.
  */
class DOMReader {
public:
    /** Loads a DOM tree from an XML-formatted file.
      *
      * There is currently no charset/encoding recognition nor translation
      * done by the parser. XML files are assumed to be encoded in UTF-8.
      * 
      * Reading is done synchronously for now, so DOMReader objects are
      * somehow reusable, but don't expect it to be true in the future when
      * asynchronous loading comes...
      * 
      * @param filename XML-formatted file's name
      * @return A shared pointer pointing at a Document object.
      * @sa DOMException
      */
    DocumentPtr readDocument(const std::wstring& filename);
private:
    /** Parse a &lt;...&gt; block
      * @param data Block content
      * @internal
      */
    void parseAngleBracketBlock(std::wstring& data);
    /** Parse a &lt;?...?&gt; block
      * @param data Block content
      * @internal
      */
    void parseNewHeader(std::wstring& data);
    /** Parse a &lt;!...&gt; block
      * @param data Block content
      * @internal
      */
    void parseNewDTDElement(std::wstring& data);
    /** Parse a &lt;.../&gt; block
      * @param data Block content
      * @internal
      */
    void parseNewTag(std::wstring& data);
    /** Parse a &lt;...&gt; block containing no attributes
      * @param data Block content
      * @internal
      */
    void parseNewElement(std::wstring& data);
    /** Parse attributes from &lt;...&gt; block
      * @param data Block content after the tag name
      * @internal
      */
    void parseAttributes(std::wstring& data);
    /** Parse a &lt;/...&gt; block
      * @param data Block content
      * @internal
      */
    void closeTag(std::wstring& data);
private:
    std::list<NodePtr> m_oNodeStack; ///<! Parser's stack
    DocumentPtr        m_oDocument;  ///<! Current document
};

}; // namespace dom

#endif // DOM_READER_INCLUDED
