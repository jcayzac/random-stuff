// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(DOM_TEXT_INCLUDED)
#define DOM_TEXT_INCLUDED

#include "dom_node.hpp"

namespace dom {

/** Implementation of the TEXT_NODE DOM node.
  * http://www.w3.org/TR/DOM-Level-2-Core/core.html#ID-1312295772
  */
class Text: public Node {
private:
    Text(const Text&);
public:
    Text(const std::wstring& data);
    virtual ~Text() { }

    // Interface CharacterData
    // http://www.w3.org/TR/DOM-Level-2-Core/core.html#ID-FF21A306
    std::wstring&       data()            { return m_oValue; }
    const std::wstring& data() const      { return m_oValue; }
    unsigned long       length() const    { return (unsigned long)m_oValue.length(); }
    std::wstring        substringData(unsigned long offset, unsigned long count) const;
    void                appendData(const std::wstring& arg);
    void                insertData(unsigned long offset, const std::wstring& arg);
    void                deleteData(unsigned long offset, unsigned long count);
    void                replaceData(unsigned long offset, unsigned long count, const std::wstring& arg);
    //
    TextPtr             replaceWholeText(const std::wstring& content);
    const std::wstring& wholeText() const;

    // From Node
    NodePtr cloneNode(bool deep=false) const;
    bool    textContent(std::wstring& output) const { output=m_oValue; return true; }
    void    setTextContent(const std::wstring& newContent) { m_oValue=newContent; }
private:
    mutable std::wstring m_oTempString;
};

}; // namespace dom

#endif // DOM_TEXT_INCLUDED
