// Copyright © 2008 Julien Cayzac (julien.cayzac@gmail.com)
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/pool/detail/singleton.hpp>
#include <boost/tokenizer.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <vector>
#include <list>
#include <iostream>
#include "dom_document.hpp"
#include "dom_utils.hpp"
using namespace dom;

typedef boost::tokenizer<boost::char_separator<wchar_t>,std::wstring::const_iterator,std::wstring> DOMTokenizer;

// This object will be wrapped into a singleton, and built before main() is called
class EntityDatabase {
public:
    static wchar_t get(const std::wstring& entity) {
        const std::map<std::wstring,wchar_t>& entities = boost::details::pool::singleton_default<EntityDatabase>::instance().mEntityMap;
        std::map<std::wstring,wchar_t>::const_iterator it=entities.find(entity);
        if (it==entities.end())
            throw DOMException(DOMException::INVALID_CHARACTER_ERR);
        return it->second;
    }
private:
    std::map<std::wstring,wchar_t> mEntityMap;
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
public:
#elif defined(BOOST_MSVC)
    // for some inexplicable reason insertion of "class" generates compile erro
    // on msvc 7.1
    friend boost::details::pool::singleton_default<EntityDatabase>;
#else
    friend class boost::details::pool::singleton_default<EntityDatabase>;
#endif
    EntityDatabase() {
        mEntityMap[L"amp"]         = 0x0026;    // XML 1.0, HTML 2.0 ampersand
        mEntityMap[L"apos"]        = 0x0027;    // XML 1.0, XHTML    apostrophe
        mEntityMap[L"acute"]       = 0x00B4;    // HTML 3.2     acute accent
        mEntityMap[L"agrave"]      = 0x00E0;    // HTML 2.0     Latin small letter a with grave
        mEntityMap[L"aacute"]      = 0x00E1;    // HTML 2.0     Latin small letter a with acute
        mEntityMap[L"acirc"]       = 0x00E2;    // HTML 2.0     Latin small letter a with circumflex
        mEntityMap[L"atilde"]      = 0x00E3;    // HTML 2.0     Latin small letter a with tilde
        mEntityMap[L"auml"]        = 0x00E4;    // HTML 2.0     Latin small letter a with diaeresis
        mEntityMap[L"aring"]       = 0x00E5;    // HTML 2.0     Latin small letter a with ring above
        mEntityMap[L"aelig"]       = 0x00E6;    // HTML 2.0     Latin lowercase ligature ae
        mEntityMap[L"alpha"]       = 0x03B1;    // HTML 4.0     Greek small letter alpha
        mEntityMap[L"alefsym"]     = 0x2135;    // HTML 4.0     alef symbol             
        mEntityMap[L"ang"]         = 0x2220;    // HTML 4.0     angle
        mEntityMap[L"and"]         = 0x2227;    // HTML 4.0     logical and
        mEntityMap[L"asymp"]       = 0x2248;    // HTML 4.0     almost equal to
        mEntityMap[L"brvbar"]      = 0x00A6;    // HTML 3.2     broken bar
        mEntityMap[L"beta"]        = 0x03B2;    // HTML 4.0     Greek small letter beta
        mEntityMap[L"bdquo"]       = 0x201E;    // HTML 4.0     double low-9 quotation mark
        mEntityMap[L"bull"]        = 0x2022;    // HTML 4.0     bullet
        mEntityMap[L"copy"]        = 0x00A9;    // HTML 3.2     copyright sign
        mEntityMap[L"ccedil"]      = 0x00E7;    // HTML 2.0     Latin small letter c with cedilla
        mEntityMap[L"cent"]        = 0x00A2;    // HTML 3.2     cent sign
        mEntityMap[L"curren"]      = 0x00A4;    // HTML 3.2     currency sign
        mEntityMap[L"cedil"]       = 0x00B8;    // HTML 3.2     cedilla
        mEntityMap[L"circ"]        = 0x02C6;    // HTML 4.0     modifier letter circumflex accent
        mEntityMap[L"chi"]         = 0x03C7;    // HTML 4.0     Greek small letter chi
        mEntityMap[L"crarr"]       = 0x21B5;    // HTML 4.0     downwards arrow with corner leftwards
        mEntityMap[L"cap"]         = 0x2229;    // HTML 4.0     intersection
        mEntityMap[L"cup"]         = 0x222A;    // HTML 4.0     union
        mEntityMap[L"cong"]        = 0x2245;    // HTML 4.0     congruent to
        mEntityMap[L"clubs"]       = 0x2663;    // HTML 4.0     black club suit
        mEntityMap[L"deg"]         = 0x00B0;    // HTML 3.2     degree sign
        mEntityMap[L"divide"]      = 0x00F7;    // HTML 3.2     division sign
        mEntityMap[L"delta"]       = 0x03B4;    // HTML 4.0     Greek small letter delta
        mEntityMap[L"dagger"]      = 0x2020;    // HTML 4.0     dagger
        mEntityMap[L"darr"]        = 0x2193;    // HTML 4.0     downwards arrow
        mEntityMap[L"dArr"]        = 0x21D3;    // HTML 4.0     downwards double arrow
        mEntityMap[L"diams"]       = 0x2666;    // HTML 4.0     black diamond suit
        mEntityMap[L"egrave"]      = 0x00E8;    // HTML 2.0     Latin small letter e with grave
        mEntityMap[L"eacute"]      = 0x00E9;    // HTML 2.0     Latin small letter e with acute
        mEntityMap[L"ecirc"]       = 0x00EA;    // HTML 2.0     Latin small letter e with circumflex
        mEntityMap[L"euml"]        = 0x00EB;    // HTML 2.0     Latin small letter e with diaeresis
        mEntityMap[L"eth"]         = 0x00F0;    // HTML 2.0     Latin small letter eth
        mEntityMap[L"epsilon"]     = 0x03B5;    // HTML 4.0     Greek small letter epsilon
        mEntityMap[L"eta"]         = 0x03B7;    // HTML 4.0     Greek small letter eta
        mEntityMap[L"ensp"]        = 0x2002;    // HTML 4.0     en space [1]
        mEntityMap[L"emsp"]        = 0x2003;    // HTML 4.0     em space [2]
        mEntityMap[L"euro"]        = 0x20AC;    // HTML 4.0     euro sign
        mEntityMap[L"exist"]       = 0x2203;    // HTML 4.0     there exists
        mEntityMap[L"empty"]       = 0x2205;    // HTML 4.0     empty set
        mEntityMap[L"equiv"]       = 0x2261;    // HTML 4.0     identical to (equivalent to)
        mEntityMap[L"frac14"]      = 0x00BC;    // HTML 3.2     vulgar fraction one quarter
        mEntityMap[L"frac12"]      = 0x00BD;    // HTML 3.2     vulgar fraction one half
        mEntityMap[L"frac34"]      = 0x00BE;    // HTML 3.2     vulgar fraction three quarters
        mEntityMap[L"fnof"]        = 0x0192;    // HTML 4.0     Latin small letter f with hook
        mEntityMap[L"frasl"]       = 0x2044;    // HTML 4.0     fraction slash
        mEntityMap[L"forall"]      = 0x2200;    // HTML 4.0     for all
        mEntityMap[L"gt"]          = 0x003e;    // XML 1.0, HTML 2.0    greater-than sign
        mEntityMap[L"gamma"]       = 0x03B3;    // HTML 4.0     Greek small letter gamma
        mEntityMap[L"ge"]          = 0x2265;    // HTML 4.0     greater-than or equal to
        mEntityMap[L"hellip"]      = 0x2026;    // HTML 4.0     horizontal ellipsis
        mEntityMap[L"harr"]        = 0x2194;    // HTML 4.0     left right arrow
        mEntityMap[L"hArr"]        = 0x21D4;    // HTML 4.0     left right double arrow
        mEntityMap[L"hearts"]      = 0x2665;    // HTML 4.0     black heart suit
        mEntityMap[L"igrave"]      = 0x00EC;    // HTML 2.0     Latin small letter i with grave
        mEntityMap[L"iacute"]      = 0x00ED;    // HTML 2.0     Latin small letter i with acute
        mEntityMap[L"icirc"]       = 0x00EE;    // HTML 2.0     Latin small letter i with circumflex
        mEntityMap[L"iuml"]        = 0x00EF;    // HTML 2.0     Latin small letter i with diaeresis
        mEntityMap[L"iexcl"]       = 0x00A1;    // HTML 3.2     inverted exclamation mark
        mEntityMap[L"iquest"]      = 0x00BF;    // HTML 3.2     inverted question mark
        mEntityMap[L"iota"]        = 0x03B9;    // HTML 4.0     Greek small letter iota
        mEntityMap[L"image"]       = 0x2111;    // HTML 4.0     black-letter capital i
        mEntityMap[L"isin"]        = 0x2208;    // HTML 4.0     element of
        mEntityMap[L"infin"]       = 0x221E;    // HTML 4.0     infinity
        mEntityMap[L"int"]         = 0x222B;    // HTML 4.0     integral
        mEntityMap[L"kappa"]       = 0x03BA;    // HTML 4.0     Greek small letter kappa
        mEntityMap[L"lt"]          = 0x003c;    // XML 1.0, HTML 2.0 less-than sign
        mEntityMap[L"laquo"]       = 0x00AB;    // HTML 3.2     left-pointing double angle quotation mark
        mEntityMap[L"lambda"]      = 0x03BB;    // HTML 4.0     Greek small letter lambda
        mEntityMap[L"lrm"]         = 0x200E;    // HTML 4.0     left-to-right mark
        mEntityMap[L"lsquo"]       = 0x2018;    // HTML 4.0     left single quotation mark
        mEntityMap[L"ldquo"]       = 0x201C;    // HTML 4.0     left double quotation mark
        mEntityMap[L"larr"]        = 0x2190;    // HTML 4.0     leftwards arrow             
        mEntityMap[L"lsaquo"]      = 0x2039;    // HTML 4.0     single left-pointing angle quotation mark
        mEntityMap[L"lArr"]        = 0x21D0;    // HTML 4.0     leftwards double arrow
        mEntityMap[L"lowast"]      = 0x2217;    // HTML 4.0     asterisk operator
        mEntityMap[L"le"]          = 0x2264;    // HTML 4.0     less-than or equal to
        mEntityMap[L"lceil"]       = 0x2308;    // HTML 4.0     left ceiling
        mEntityMap[L"lfloor"]      = 0x230A;    // HTML 4.0     left floor
        mEntityMap[L"lang"]        = 0x2329;    // HTML 4.0     left-pointing angle bracket
        mEntityMap[L"loz"]         = 0x25CA;    // HTML 4.0     lozenge
        mEntityMap[L"macr"]        = 0x00AF;    // HTML 3.2     macron
        mEntityMap[L"micro"]       = 0x00B5;    // HTML 3.2     micro sign
        mEntityMap[L"middot"]      = 0x00B7;    // HTML 3.2     middle dot
        mEntityMap[L"mu"]          = 0x03BC;    // HTML 4.0     Greek small letter mu
        mEntityMap[L"mdash"]       = 0x2014;    // HTML 4.0     em dash
        mEntityMap[L"minus"]       = 0x2212;    // HTML 4.0     minus sign
        mEntityMap[L"nbsp"]        = 0x00A0;    // HTML 3.2     non-breaking space
        mEntityMap[L"ntilde"]      = 0x00F1;    // HTML 2.0     Latin small letter n with tilde
        mEntityMap[L"not"]         = 0x00AC;    // HTML 3.2     not sign
        mEntityMap[L"nu"]          = 0x03BD;    // HTML 4.0     Greek small letter nu
        mEntityMap[L"ndash"]       = 0x2013;    // HTML 4.0     en dash
        mEntityMap[L"nabla"]       = 0x2207;    // HTML 4.0     nabla
        mEntityMap[L"notin"]       = 0x2209;    // HTML 4.0     not an element of
        mEntityMap[L"ni"]          = 0x220B;    // HTML 4.0     contains as member
        mEntityMap[L"ne"]          = 0x2260;    // HTML 4.0     not equal to
        mEntityMap[L"nsub"]        = 0x2284;    // HTML 4.0     not a subset of
        mEntityMap[L"ograve"]      = 0x00F2;    // HTML 2.0     Latin small letter o with grave
        mEntityMap[L"oacute"]      = 0x00F3;    // HTML 2.0     Latin small letter o with acute
        mEntityMap[L"ocirc"]       = 0x00F4;    // HTML 2.0     Latin small letter o with circumflex
        mEntityMap[L"otilde"]      = 0x00F5;    // HTML 2.0     Latin small letter o with tilde
        mEntityMap[L"ouml"]        = 0x00F6;    // HTML 2.0     Latin small letter o with diaeresis
        mEntityMap[L"ordf"]        = 0x00AA;    // HTML 3.2     feminine ordinal indicator
        mEntityMap[L"ordm"]        = 0x00BA;    // HTML 3.2     masculine ordinal indicator
        mEntityMap[L"oslash"]      = 0x00F8;    // HTML 2.0     Latin small letter o with stroke
        mEntityMap[L"oelig"]       = 0x0153;    // HTML 4.0     Latin small ligature oe
        mEntityMap[L"omicron"]     = 0x03BF;    // HTML 4.0     Greek small letter omicron
        mEntityMap[L"omega"]       = 0x03C9;    // HTML 4.0     Greek small letter omega
        mEntityMap[L"oline"]       = 0x203E;    // HTML 4.0     overline
        mEntityMap[L"or"]          = 0x2228;    // HTML 4.0     logical or
        mEntityMap[L"oplus"]       = 0x2295;    // HTML 4.0     circled plus
        mEntityMap[L"otimes"]      = 0x2297;    // HTML 4.0     circled times
        mEntityMap[L"pound"]       = 0x00A3;    // HTML 3.2     pound sign
        mEntityMap[L"plusmn"]      = 0x00B1;    // HTML 3.2     plus-minus sign
        mEntityMap[L"para"]        = 0x00B6;    // HTML 3.2     pilcrow sign
        mEntityMap[L"pi"]          = 0x03C0;    // HTML 4.0     Greek small letter pi
        mEntityMap[L"phi"]         = 0x03C6;    // HTML 4.0     Greek small letter phi
        mEntityMap[L"psi"]         = 0x03C8;    // HTML 4.0     Greek small letter psi
        mEntityMap[L"piv"]         = 0x03D6;    // HTML 4.0     Greek pi symbol
        mEntityMap[L"permil"]      = 0x2030;    // HTML 4.0     per mille sign
        mEntityMap[L"prime"]       = 0x2032;    // HTML 4.0     prime
        mEntityMap[L"part"]        = 0x2202;    // HTML 4.0     partial differential
        mEntityMap[L"prod"]        = 0x220F;    // HTML 4.0     n-ary product
        mEntityMap[L"prop"]        = 0x221D;    // HTML 4.0     proportional to
        mEntityMap[L"perp"]        = 0x22A5;    // HTML 4.0     up tack (perpendicular sign in math)
        mEntityMap[L"quot"]        = 0x0022;    // XML 1.0, HTML 2.0 quotation mark
        mEntityMap[L"reg"]         = 0x00AE;    // HTML 3.2     registered sign
        mEntityMap[L"raquo"]       = 0x00BB;    // HTML 3.2     right-pointing double angle quotation mark
        mEntityMap[L"rho"]         = 0x03C1;    // HTML 4.0     Greek small letter rho
        mEntityMap[L"rlm"]         = 0x200F;    // HTML 4.0     right-to-left mark
        mEntityMap[L"rsquo"]       = 0x2019;    // HTML 4.0     right single quotation mark
        mEntityMap[L"rdquo"]       = 0x201D;    // HTML 4.0     right double quotation mark
        mEntityMap[L"rsaquo"]      = 0x203A;    // HTML 4.0     single right-pointing angle quotation mark
        mEntityMap[L"real"]        = 0x211C;    // HTML 4.0     black-letter capital r     
        mEntityMap[L"rarr"]        = 0x2192;    // HTML 4.0     rightwards arrow
        mEntityMap[L"rArr"]        = 0x21D2;    // HTML 4.0     rightwards double arrow
        mEntityMap[L"radic"]       = 0x221A;    // HTML 4.0     square root
        mEntityMap[L"rceil"]       = 0x2309;    // HTML 4.0     right ceiling
        mEntityMap[L"rfloor"]      = 0x230B;    // HTML 4.0     right floor
        mEntityMap[L"rang"]        = 0x232A;    // HTML 4.0     right-pointing angle bracket
        mEntityMap[L"shy"]         = 0x00AD;    // HTML 3.2     soft hyphen
        mEntityMap[L"sup2"]        = 0x00B2;    // HTML 3.2     superscript two
        mEntityMap[L"sup3"]        = 0x00B3;    // HTML 3.2     superscript three
        mEntityMap[L"sup1"]        = 0x00B9;    // HTML 3.2     superscript one
        mEntityMap[L"sect"]        = 0x00A7;    // HTML 3.2     section sign
        mEntityMap[L"szlig"]       = 0x00DF;    // HTML 2.0     Latin small letter sharp s (German Eszett)
        mEntityMap[L"scaron"]      = 0x0161;    // HTML 4.0     Latin small letter s with caron
        mEntityMap[L"sigmaf"]      = 0x03C2;    // HTML 4.0     Greek small letter final sigma
        mEntityMap[L"sigma"]       = 0x03C3;    // HTML 4.0     Greek small letter sigma
        mEntityMap[L"sbquo"]       = 0x201A;    // HTML 4.0     single low-9 quotation mark
        mEntityMap[L"sum"]         = 0x2211;    // HTML 4.0     n-ary summation
        mEntityMap[L"sim"]         = 0x223C;    // HTML 4.0     tilde operator
        mEntityMap[L"sub"]         = 0x2282;    // HTML 4.0     subset of
        mEntityMap[L"sup"]         = 0x2283;    // HTML 4.0     superset of
        mEntityMap[L"sube"]        = 0x2286;    // HTML 4.0     subset of or equal to
        mEntityMap[L"supe"]        = 0x2287;    // HTML 4.0     superset of or equal to
        mEntityMap[L"sdot"]        = 0x22C5;    // HTML 4.0     dot operator
        mEntityMap[L"spades"]      = 0x2660;    // HTML 4.0     black spade suit
        mEntityMap[L"times"]       = 0x00D7;    // HTML 3.2     multiplication sign
        mEntityMap[L"thorn"]       = 0x00FE;    // HTML 2.0     Latin small letter thorn
        mEntityMap[L"tilde"]       = 0x02DC;    // HTML 4.0     small tilde
        mEntityMap[L"theta"]       = 0x03B8;    // HTML 4.0     Greek small letter theta
        mEntityMap[L"tau"]         = 0x03C4;    // HTML 4.0     Greek small letter tau
        mEntityMap[L"thetasym"]    = 0x03D1;    // HTML 4.0     Greek theta symbol
        mEntityMap[L"thinsp"]      = 0x2009;    // HTML 4.0     thin space [3]
        mEntityMap[L"trade"]       = 0x2122;    // HTML 4.0     trademark sign             
        mEntityMap[L"there4"]      = 0x2234;    // HTML 4.0     therefore
        mEntityMap[L"uml"]         = 0x00A8;    // HTML 3.2     diaeresis
        mEntityMap[L"ugrave"]      = 0x00F9;    // HTML 2.0     Latin small letter u with grave
        mEntityMap[L"uacute"]      = 0x00FA;    // HTML 2.0     Latin small letter u with acute
        mEntityMap[L"ucirc"]       = 0x00FB;    // HTML 2.0     Latin small letter u with circumflex
        mEntityMap[L"uuml"]        = 0x00FC;    // HTML 2.0     Latin small letter u with diaeresis
        mEntityMap[L"upsilon"]     = 0x03C5;    // HTML 4.0     Greek small letter upsilon
        mEntityMap[L"upsih"]       = 0x03D2;    // HTML 4.0     Greek upsilon with hook symbol
        mEntityMap[L"uarr"]        = 0x2191;    // HTML 4.0     upwards arrow             
        mEntityMap[L"uArr"]        = 0x21D1;    // HTML 4.0     upwards double arrow
        mEntityMap[L"weierp"]      = 0x2118;    // HTML 4.0     script capital p (Weierstrass p)
        mEntityMap[L"xi"]          = 0x03BE;    // HTML 4.0     Greek small letter xi
        mEntityMap[L"yen"]         = 0x00A5;    // HTML 3.2     yen sign
        mEntityMap[L"yacute"]      = 0x00FD;    // HTML 2.0     Latin small letter y with acute
        mEntityMap[L"yuml"]        = 0x00FF;    // HTML 2.0     Latin small letter y with diaeresis
        mEntityMap[L"zeta"]        = 0x03B6;    // HTML 4.0     Greek small letter zeta
        mEntityMap[L"zwnj"]        = 0x200C;    // HTML 4.0     zero width non-joiner
        mEntityMap[L"zwj"]         = 0x200D;    // HTML 4.0     zero width joiner
        mEntityMap[L"Agrave"]      = 0x00C0;    // HTML 2.0     Latin capital letter a with grave
        mEntityMap[L"Aacute"]      = 0x00C1;    // HTML 2.0     Latin capital letter a with acute
        mEntityMap[L"Acirc"]       = 0x00C2;    // HTML 2.0     Latin capital letter a with circumflex
        mEntityMap[L"Atilde"]      = 0x00C3;    // HTML 2.0     Latin capital letter a with tilde
        mEntityMap[L"Auml"]        = 0x00C4;    // HTML 2.0     Latin capital letter a with diaeresis
        mEntityMap[L"Aring"]       = 0x00C5;    // HTML 2.0     Latin capital letter a with ring above
        mEntityMap[L"AElig"]       = 0x00C6;    // HTML 2.0     Latin capital letter ae
        mEntityMap[L"Alpha"]       = 0x0391;    // HTML 4.0     Greek capital letter alpha
        mEntityMap[L"Beta"]        = 0x0392;    // HTML 4.0     Greek capital letter beta
        mEntityMap[L"Ccedil"]      = 0x00C7;    // HTML 2.0     Latin capital letter c with cedilla
        mEntityMap[L"Chi"]         = 0x03A7;    // HTML 4.0     Greek capital letter chi
        mEntityMap[L"Delta"]       = 0x0394;    // HTML 4.0     Greek capital letter delta
        mEntityMap[L"Dagger"]      = 0x2021;    // HTML 4.0     double dagger
        mEntityMap[L"Egrave"]      = 0x00C8;    // HTML 2.0     Latin capital letter e with grave
        mEntityMap[L"Eacute"]      = 0x00C9;    // HTML 2.0     Latin capital letter e with acute
        mEntityMap[L"Ecirc"]       = 0x00CA;    // HTML 2.0     Latin capital letter e with circumflex
        mEntityMap[L"Euml"]        = 0x00CB;    // HTML 2.0     Latin capital letter e with diaeresis
        mEntityMap[L"ETH"]         = 0x00D0;    // HTML 2.0     Latin capital letter eth
        mEntityMap[L"Epsilon"]     = 0x0395;    // HTML 4.0     Greek capital letter epsilon
        mEntityMap[L"Eta"]         = 0x0397;    // HTML 4.0     Greek capital letter eta
        mEntityMap[L"Gamma"]       = 0x0393;    // HTML 4.0     Greek capital letter gamma
        mEntityMap[L"Igrave"]      = 0x00CC;    // HTML 2.0     Latin capital letter i with grave
        mEntityMap[L"Iacute"]      = 0x00CD;    // HTML 2.0     Latin capital letter i with acute
        mEntityMap[L"Icirc"]       = 0x00CE;    // HTML 2.0     Latin capital letter i with circumflex
        mEntityMap[L"Iuml"]        = 0x00CF;    // HTML 2.0     Latin capital letter i with diaeresis
        mEntityMap[L"Iota"]        = 0x0399;    // HTML 4.0     Greek capital letter iota
        mEntityMap[L"Kappa"]       = 0x039A;    // HTML 4.0     Greek capital letter kappa
        mEntityMap[L"Lambda"]      = 0x039B;    // HTML 4.0     Greek capital letter lambda
        mEntityMap[L"Mu"]          = 0x039C;    // HTML 4.0     Greek capital letter mu
        mEntityMap[L"Ntilde"]      = 0x00D1;    // HTML 2.0     Latin capital letter n with tilde
        mEntityMap[L"Nu"]          = 0x039D;    // HTML 4.0     Greek capital letter nu
        mEntityMap[L"Ograve"]      = 0x00D2;    // HTML 2.0     Latin capital letter o with grave
        mEntityMap[L"Oacute"]      = 0x00D3;    // HTML 2.0     Latin capital letter o with acute
        mEntityMap[L"Ocirc"]       = 0x00D4;    // HTML 2.0     Latin capital letter o with circumflex
        mEntityMap[L"Otilde"]      = 0x00D5;    // HTML 2.0     Latin capital letter o with tilde
        mEntityMap[L"Ouml"]        = 0x00D6;    // HTML 2.0     Latin capital letter o with diaeresis
        mEntityMap[L"Oslash"]      = 0x00D8;    // HTML 2.0     Latin capital letter o with stroke
        mEntityMap[L"OElig"]       = 0x0152;    // HTML 4.0     Latin capital ligature oe
        mEntityMap[L"Omicron"]     = 0x039F;    // HTML 4.0     Greek capital letter omicron
        mEntityMap[L"Omega"]       = 0x03A9;    // HTML 4.0     Greek capital letter omega
        mEntityMap[L"Pi"]          = 0x03A0;    // HTML 4.0     Greek capital letter pi
        mEntityMap[L"Phi"]         = 0x03A6;    // HTML 4.0     Greek capital letter phi
        mEntityMap[L"Psi"]         = 0x03A8;    // HTML 4.0     Greek capital letter psi
        mEntityMap[L"Prime"]       = 0x2033;    // HTML 4.0     double prime
        mEntityMap[L"Rho"]         = 0x03A1;    // HTML 4.0     Greek capital letter rho
        mEntityMap[L"Scaron"]      = 0x0160;    // HTML 4.0     Latin capital letter s with caron
        mEntityMap[L"Sigma"]       = 0x03A3;    // HTML 4.0     Greek capital letter sigma
        mEntityMap[L"THORN"]       = 0x00DE;    // HTML 2.0     Latin capital letter thorn
        mEntityMap[L"Theta"]       = 0x0398;    // HTML 4.0     Greek capital letter theta
        mEntityMap[L"Tau"]         = 0x03A4;    // HTML 4.0     Greek capital letter tau
        mEntityMap[L"Ugrave"]      = 0x00D9;    // HTML 2.0     Latin capital letter u with grave
        mEntityMap[L"Uacute"]      = 0x00DA;    // HTML 2.0     Latin capital letter u with acute
        mEntityMap[L"Ucirc"]       = 0x00DB;    // HTML 2.0     Latin capital letter u with circumflex
        mEntityMap[L"Uuml"]        = 0x00DC;    // HTML 2.0     Latin capital letter u with diaeresis
        mEntityMap[L"Upsilon"]     = 0x03A5;    // HTML 4.0     Greek capital letter upsilon
        mEntityMap[L"Xi"]          = 0x039E;    // HTML 4.0     Greek capital letter xi
        mEntityMap[L"Yacute"]      = 0x00DD;    // HTML 2.0     Latin capital letter y with acute
        mEntityMap[L"Yuml"]        = 0x0178;    // HTML 4.0     Latin capital letter y with diaeresis
        mEntityMap[L"Zeta"]        = 0x0396;    // HTML 4.0     Greek capital letter zeta
    }
};

wchar_t DOMUtils::translateXMLEntity(const std::wstring& entity) {
    if (entity.find(L"&")!=0 || entity.rfind(L";")!=entity.length()-1)
        throw DOMException(DOMException::INVALID_CHARACTER_ERR);
    if (0==entity.find(L"&#x")) {
        // Hexadecimal entity
        std::wistringstream iss(entity.substr(3,entity.length()-4));
        wchar_t us;
        iss >> std::hex >> us;
        return us;
    }
    else if (0==entity.find(L"&#")) {
        // Decimal entity
        std::wistringstream iss(entity.substr(2,entity.length()-3));
        wchar_t us;
        iss >> us;
        return us;
    }
    else {
        // Named entity
        return EntityDatabase::get(entity.substr(1,entity.length()-2));
    }
}

bool DOMUtils::isLineBreakingSpace(wchar_t c) {
    static const wchar_t LINE_BREAKING[] = {
        0x000a,	// LINE FEED
        0x000c,	// FORM FEED
        0x0085,	// NEXT LINE
        0x2028, // LINE SEPARATOR
        0x2029,	// PARAGRAPH SEPARATOR
    };
    const wchar_t* begin = &LINE_BREAKING[0];
    const wchar_t* end   = &LINE_BREAKING[sizeof(LINE_BREAKING)/sizeof(wchar_t)];
    return (end!=std::find(begin,end,c));
}

bool DOMUtils::isNonBreakingWhiteSpace(wchar_t c) {
    static const wchar_t NON_BREAKING_SPACE[] = {
        0x00a0,	// NBSP
        0x202f,	// NARROW NBSP
        0x2060, // WORD JOINER
        0xfeff, // ZERO WIDTH NO-BREAK SPACE (also used as BOM)
    };
    const wchar_t* begin = &NON_BREAKING_SPACE[0];
    const wchar_t* end   = &NON_BREAKING_SPACE[sizeof(NON_BREAKING_SPACE)/sizeof(wchar_t)];
    return (end!=std::find(begin,end,c));
}

bool DOMUtils::isBreakingWhiteSpace(wchar_t c) {
    static const wchar_t BREAKING_SPACE[] = {
        0x0009,	// HORIZONTAL TAB
        0x000b,	// VERTICAL TAB
        0x000d,	// CARRIAGE RETURN
        0x0020,	// SPACE
        0x1680,	// OGHAM SPACE MARK
        0x180e,	// MONGOLIAN VOWEL SEPARATOR
        0x2000,	// EN QUAD
        0x2001,	// EM QUAD
        0x2002,	// EN SPACE
        0x2003,	// EM SPACE
        0x2004,	// THREE-PER-EM SPACE
        0x2005,	// FOUR-PER-EM SPACE
        0x2006,	// SIX-PER-EM SPACE
        0x2007,	// FIGURE SPACE
        0x2008,	// PUNCTUATION SPACE
        0x2009,	// THIN SPACE
        0x200a,	// HAIR SPACE
        0x205f,	// MEDIUM MATHEMATICAL SPACE
        0x3000, // IDEOGRAPHIC SPACE
    };
    const wchar_t* begin = &BREAKING_SPACE[0];
    const wchar_t* end   = &BREAKING_SPACE[sizeof(BREAKING_SPACE)/sizeof(wchar_t)];
    return (end!=std::find(begin,end,c));
}

bool DOMUtils::isWhiteSpace(wchar_t c) {
    return isBreakingWhiteSpace(c) || isNonBreakingWhiteSpace(c);
}

static bool isAnySpace(wchar_t c) {
    return DOMUtils::isWhiteSpace(c) || DOMUtils::isLineBreakingSpace(c);
}

void DOMUtils::trimSpaces(std::wstring& s) {
    boost::trim_if(s,isAnySpace);
}
