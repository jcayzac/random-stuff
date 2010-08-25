// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(DOM_UTILS_INCLUDED)
#define DOM_UTILS_INCLUDED

#include <string>

namespace dom {

/** @brief An utility class providing static and hopefully useful methods.
  *
  * Right now, this class only offer two services:<ul>
  *   <li>Translating XML entities to their corresponding unicode characters;</li>
  *   <li>Dealing with unicode characters falling in the <i>space</i> category;</li>
  * </ul>
  *
  * @sa http://www.w3.org/MarkUp/html3/latin1.html
  * @sa http://www.w3.org/TR/html4/sgml/entities.html
  * @sa http://unicode.org/Public/UNIDATA/PropList.txt
  *
  */
class DOMUtils {
public:
    /** Translate an XML/HTML entity, such as "&amp;eacute;" or "&amp;#67;"
      * or "&amp;#xe3;", and return its character value.
      * @param entity Name of the entity
      * @return The corresponding character
      */
	static wchar_t translateXMLEntity(const std::wstring& entity);

    /** Check if a given character is a line breaking space.
      * @param c Some character.
      * @return <code>true</code> if c is a line breaking space, <code>false</code> otherwise.
      */
    static bool isLineBreakingSpace(wchar_t c);

    /** Check if a given character is a breaking white space.
      * @param c Some character.
      * @return <code>true</code> if c is a breaking white space, <code>false</code> otherwise.
      */
    static bool isBreakingWhiteSpace(wchar_t c);

    /** Check if a given character is a non breaking white space.
      * @param c Some character.
      * @return <code>true</code> if c is a non breaking white space, <code>false</code> otherwise.
      */
    static bool isNonBreakingWhiteSpace(wchar_t c);

    /** Check if a given character is a white space.
      * @param c Some character.
      * @return <code>true</code> if c is a white space, <code>false</code> otherwise.
      */
    static bool isWhiteSpace(wchar_t c);

    /** Remove leading and trailing white space from a string.
      * @param s Some string.
      * @note Trimming is done in-place.
      */
    static void trimSpaces(std::wstring& s);

};

}; // namespace dom

#endif // DOM_UTILS_INCLUDED
