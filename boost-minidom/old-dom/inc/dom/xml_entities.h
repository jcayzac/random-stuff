// $Id: xml_entities.h 8630 2007-12-04 17:40:17Z jcayzac $
#if !defined(XML_ENTITIES_INCLUDED)
#define XML_ENTITIES_INCLUDED

#include <llsdk/inc/text/unicode.h>
#include <llsdk/inc/memory/memory.h>

namespace dom {

static mobilesdk_char GetXMLEntity(mobilesdk_char*& data, unsigned long& lengthToGo) {
	if (!(data && lengthToGo)) return 0;
	msdk::CString entity;
	// Read entity
	mobilesdk_char c[2];
	c[1]=0;
	for(;;) {
		c[0] = *(data++); --lengthToGo;
		if (c[0]==0x003b) break;
		if (text::IsWhiteSpace(c[0])) break; // allow malformed entities (missing semicolon)
		if (!lengthToGo) return 0; // bad entity !
		entity+=c;
	}
	// Parse entity
	int length=entity.Length();
	if (length) {
		if (entity[0]==0x0023) {
			// Numerical entity
			if (length==1) return 0;
			if (entity[1]==0x0078) {
				// Hexadecimal
				if (length==2) return 0;
				msdk::CString hexValue(entity.Get()+2);
				return (mobilesdk_char)hexValue.ToULong(16);
			}
			else {
				// Decimal
				msdk::CString decimalValue(entity.Get()+1);
				return (mobilesdk_char)decimalValue.ToULong();
			}
		}
		else {
			// entities
			switch(entity[0]) {
			case L'a':
				if (entity=="amp")		return 0x0026;	// XML 1.0, HTML 2.0	ampersand
				if (entity=="apos")		return 0x0027;	// XML 1.0, XHTML		apostrophe
				if (entity=="acute")	return 0x00B4;	// HTML 3.2 	acute accent
				if (entity=="agrave")	return 0x00E0;	// HTML 2.0 	Latin small letter a with grave
				if (entity=="aacute")	return 0x00E1;	// HTML 2.0 	Latin small letter a with acute
				if (entity=="acirc")	return 0x00E2;	// HTML 2.0 	Latin small letter a with circumflex
				if (entity=="atilde")	return 0x00E3;	// HTML 2.0 	Latin small letter a with tilde
				if (entity=="auml")		return 0x00E4;	// HTML 2.0 	Latin small letter a with diaeresis
				if (entity=="aring")	return 0x00E5;	// HTML 2.0 	Latin small letter a with ring above
				if (entity=="aelig")	return 0x00E6;	// HTML 2.0 	Latin lowercase ligature ae
				if (entity=="alpha")	return 0x03B1;	// HTML 4.0 	Greek small letter alpha
				if (entity=="alefsym")	return 0x2135;	// HTML 4.0 	alef symbol			 
				if (entity=="ang")		return 0x2220;	// HTML 4.0 	angle
				if (entity=="and")		return 0x2227;	// HTML 4.0 	logical and
				if (entity=="asymp")	return 0x2248;	// HTML 4.0 	almost equal to
				break;
			case L'b':
				if (entity=="brvbar")	return 0x00A6;	// HTML 3.2 	broken bar
				if (entity=="beta")		return 0x03B2;	// HTML 4.0 	Greek small letter beta
				if (entity=="bdquo")	return 0x201E;	// HTML 4.0 	double low-9 quotation mark
				if (entity=="bull")		return 0x2022;	// HTML 4.0 	bullet
				break;
			case L'c':
				if (entity=="copy")		return 0x00A9;	// HTML 3.2 	copyright sign
				if (entity=="ccedil")	return 0x00E7;	// HTML 2.0 	Latin small letter c with cedilla
				if (entity=="cent")		return 0x00A2;	// HTML 3.2 	cent sign
				if (entity=="curren")	return 0x00A4;	// HTML 3.2 	currency sign
				if (entity=="cedil")	return 0x00B8;	// HTML 3.2 	cedilla
				if (entity=="circ")		return 0x02C6;	// HTML 4.0 	modifier letter circumflex accent
				if (entity=="chi")		return 0x03C7;	// HTML 4.0 	Greek small letter chi
				if (entity=="crarr")	return 0x21B5;	// HTML 4.0 	downwards arrow with corner leftwards
				if (entity=="cap")		return 0x2229;	// HTML 4.0 	intersection
				if (entity=="cup")		return 0x222A;	// HTML 4.0 	union
				if (entity=="cong")		return 0x2245;	// HTML 4.0 	congruent to
				if (entity=="clubs")	return 0x2663;	// HTML 4.0 	black club suit
				break;
			case L'd':
				if (entity=="deg")		return 0x00B0;	// HTML 3.2 	degree sign
				if (entity=="divide")	return 0x00F7;	// HTML 3.2 	division sign
				if (entity=="delta")	return 0x03B4;	// HTML 4.0 	Greek small letter delta
				if (entity=="dagger")	return 0x2020;	// HTML 4.0 	dagger
				if (entity=="darr")		return 0x2193;	// HTML 4.0 	downwards arrow
				if (entity=="dArr")		return 0x21D3;	// HTML 4.0 	downwards double arrow
				if (entity=="diams")	return 0x2666;	// HTML 4.0 	black diamond suit
				break;
			case L'e':
				if (entity=="egrave")	return 0x00E8;	// HTML 2.0 	Latin small letter e with grave
				if (entity=="eacute")	return 0x00E9;	// HTML 2.0 	Latin small letter e with acute
				if (entity=="ecirc")	return 0x00EA;	// HTML 2.0 	Latin small letter e with circumflex
				if (entity=="euml")		return 0x00EB;	// HTML 2.0 	Latin small letter e with diaeresis
				if (entity=="eth")		return 0x00F0;	// HTML 2.0 	Latin small letter eth
				if (entity=="epsilon")	return 0x03B5;	// HTML 4.0 	Greek small letter epsilon
				if (entity=="eta")		return 0x03B7;	// HTML 4.0 	Greek small letter eta
				if (entity=="ensp")		return 0x2002;	// HTML 4.0 	en space [1]
				if (entity=="emsp")		return 0x2003;	// HTML 4.0 	em space [2]
				if (entity=="euro")		return 0x20AC;	// HTML 4.0 	euro sign
				if (entity=="exist")	return 0x2203;	// HTML 4.0 	there exists
				if (entity=="empty")	return 0x2205;	// HTML 4.0 	empty set
				if (entity=="equiv")	return 0x2261;	// HTML 4.0 	identical to (equivalent to)
				break;
			case L'f':
				if (entity=="frac14")	return 0x00BC;	// HTML 3.2 	vulgar fraction one quarter
				if (entity=="frac12")	return 0x00BD;	// HTML 3.2 	vulgar fraction one half
				if (entity=="frac34")	return 0x00BE;	// HTML 3.2 	vulgar fraction three quarters
				if (entity=="fnof")		return 0x0192;	// HTML 4.0 	Latin small letter f with hook
				if (entity=="frasl")	return 0x2044;	// HTML 4.0 	fraction slash
				if (entity=="forall")	return 0x2200;	// HTML 4.0 	for all
				break;
			case L'g':
				if (entity=="gt")		return 0x003e;	// XML 1.0, HTML 2.0	greater-than sign
				if (entity=="gamma")	return 0x03B3;	// HTML 4.0 	Greek small letter gamma
				if (entity=="ge")		return 0x2265;	// HTML 4.0 	greater-than or equal to
				break;
			case L'h':
				if (entity=="hellip")	return 0x2026;	// HTML 4.0 	horizontal ellipsis
				if (entity=="harr")		return 0x2194;	// HTML 4.0 	left right arrow
				if (entity=="hArr")		return 0x21D4;	// HTML 4.0 	left right double arrow
				if (entity=="hearts")	return 0x2665;	// HTML 4.0 	black heart suit
				break;
			case L'i':
				if (entity=="igrave")	return 0x00EC;	// HTML 2.0 	Latin small letter i with grave
				if (entity=="iacute")	return 0x00ED;	// HTML 2.0 	Latin small letter i with acute
				if (entity=="icirc")	return 0x00EE;	// HTML 2.0 	Latin small letter i with circumflex
				if (entity=="iuml")		return 0x00EF;	// HTML 2.0 	Latin small letter i with diaeresis
				if (entity=="iexcl")	return 0x00A1;	// HTML 3.2 	inverted exclamation mark
				if (entity=="iquest")	return 0x00BF;	// HTML 3.2 	inverted question mark
				if (entity=="iota")		return 0x03B9;	// HTML 4.0 	Greek small letter iota
				if (entity=="image")	return 0x2111;	// HTML 4.0 	black-letter capital i
				if (entity=="isin")		return 0x2208;	// HTML 4.0 	element of
				if (entity=="infin")	return 0x221E;	// HTML 4.0 	infinity
				if (entity=="int")		return 0x222B;	// HTML 4.0 	integral
			case L'k':
				if (entity=="kappa")	return 0x03BA;	// HTML 4.0 	Greek small letter kappa
				break;
			case L'l':
				if (entity=="lt")		return 0x003c;	// XML 1.0, HTML 2.0	less-than sign
				if (entity=="laquo")	return 0x00AB;	// HTML 3.2 	left-pointing double angle quotation mark
				if (entity=="lambda")	return 0x03BB;	// HTML 4.0 	Greek small letter lambda
				if (entity=="lrm")		return 0x200E;	// HTML 4.0 	left-to-right mark
				if (entity=="lsquo")	return 0x2018;	// HTML 4.0 	left single quotation mark
				if (entity=="ldquo")	return 0x201C;	// HTML 4.0 	left double quotation mark
				if (entity=="larr")		return 0x2190;	// HTML 4.0 	leftwards arrow			 
				if (entity=="lsaquo")	return 0x2039;	// HTML 4.0 	single left-pointing angle quotation mark
				if (entity=="lArr")		return 0x21D0;	// HTML 4.0 	leftwards double arrow
				if (entity=="lowast")	return 0x2217;	// HTML 4.0 	asterisk operator
				if (entity=="le")		return 0x2264;	// HTML 4.0 	less-than or equal to
				if (entity=="lceil")	return 0x2308;	// HTML 4.0 	left ceiling
				if (entity=="lfloor")	return 0x230A;	// HTML 4.0 	left floor
				if (entity=="lang")		return 0x2329;	// HTML 4.0 	left-pointing angle bracket
				if (entity=="loz")		return 0x25CA;	// HTML 4.0 	lozenge
				break;
			case L'm':
				if (entity=="macr")		return 0x00AF;	// HTML 3.2 	macron
				if (entity=="micro")	return 0x00B5;	// HTML 3.2 	micro sign
				if (entity=="middot")	return 0x00B7;	// HTML 3.2 	middle dot
				if (entity=="mu")		return 0x03BC;	// HTML 4.0 	Greek small letter mu
				if (entity=="mdash")	return 0x2014;	// HTML 4.0 	em dash
				if (entity=="minus")	return 0x2212;	// HTML 4.0 	minus sign
				break;
			case L'n':
				if (entity=="nbsp")		return 0x00A0;	// HTML 3.2 	non-breaking space
				if (entity=="ntilde")	return 0x00F1;	// HTML 2.0 	Latin small letter n with tilde
				if (entity=="not")		return 0x00AC;	// HTML 3.2 	not sign
				if (entity=="nu")		return 0x03BD;	// HTML 4.0 	Greek small letter nu
				if (entity=="ndash")	return 0x2013;	// HTML 4.0 	en dash
				if (entity=="nabla")	return 0x2207;	// HTML 4.0 	nabla
				if (entity=="notin")	return 0x2209;	// HTML 4.0 	not an element of
				if (entity=="ni")		return 0x220B;	// HTML 4.0 	contains as member
				if (entity=="ne")		return 0x2260;	// HTML 4.0 	not equal to
				if (entity=="nsub")		return 0x2284;	// HTML 4.0 	not a subset of
				break;
			case L'o':
				if (entity=="ograve")	return 0x00F2;	// HTML 2.0 	Latin small letter o with grave
				if (entity=="oacute")	return 0x00F3;	// HTML 2.0 	Latin small letter o with acute
				if (entity=="ocirc")	return 0x00F4;	// HTML 2.0 	Latin small letter o with circumflex
				if (entity=="otilde")	return 0x00F5;	// HTML 2.0 	Latin small letter o with tilde
				if (entity=="ouml")		return 0x00F6;	// HTML 2.0 	Latin small letter o with diaeresis
				if (entity=="ordf")		return 0x00AA;	// HTML 3.2 	feminine ordinal indicator
				if (entity=="ordm")		return 0x00BA;	// HTML 3.2 	masculine ordinal indicator
				if (entity=="oslash")	return 0x00F8;	// HTML 2.0 	Latin small letter o with stroke
				if (entity=="oelig")	return 0x0153;	// HTML 4.0 	Latin small ligature oe
				if (entity=="omicron")	return 0x03BF;	// HTML 4.0 	Greek small letter omicron
				if (entity=="omega")	return 0x03C9;	// HTML 4.0 	Greek small letter omega
				if (entity=="oline")	return 0x203E;	// HTML 4.0 	overline
				if (entity=="or")		return 0x2228;	// HTML 4.0 	logical or
				if (entity=="oplus")	return 0x2295;	// HTML 4.0 	circled plus
				if (entity=="otimes")	return 0x2297;	// HTML 4.0 	circled times
				break;
			case L'p':
				if (entity=="pound")	return 0x00A3;	// HTML 3.2 	pound sign
				if (entity=="plusmn")	return 0x00B1;	// HTML 3.2 	plus-minus sign
				if (entity=="para")		return 0x00B6;	// HTML 3.2 	pilcrow sign
				if (entity=="pi")		return 0x03C0;	// HTML 4.0 	Greek small letter pi
				if (entity=="phi")		return 0x03C6;	// HTML 4.0 	Greek small letter phi
				if (entity=="psi")		return 0x03C8;	// HTML 4.0 	Greek small letter psi
				if (entity=="piv")		return 0x03D6;	// HTML 4.0 	Greek pi symbol
				if (entity=="permil")	return 0x2030;	// HTML 4.0 	per mille sign
				if (entity=="prime")	return 0x2032;	// HTML 4.0 	prime
				if (entity=="part")		return 0x2202;	// HTML 4.0 	partial differential
				if (entity=="prod")		return 0x220F;	// HTML 4.0 	n-ary product
				if (entity=="prop")		return 0x221D;	// HTML 4.0 	proportional to
				if (entity=="perp")		return 0x22A5;	// HTML 4.0 	up tack (perpendicular sign in math)
				break;
			case L'q':
				if (entity=="quot")		return 0x0022;	// XML 1.0, HTML 2.0	quotation mark
				break;
			case L'r':
				if (entity=="reg")		return 0x00AE;	// HTML 3.2 	registered sign
				if (entity=="raquo")	return 0x00BB;	// HTML 3.2 	right-pointing double angle quotation mark
				if (entity=="rho")		return 0x03C1;	// HTML 4.0 	Greek small letter rho
				if (entity=="rlm")		return 0x200F;	// HTML 4.0 	right-to-left mark
				if (entity=="rsquo")	return 0x2019;	// HTML 4.0 	right single quotation mark
				if (entity=="rdquo")	return 0x201D;	// HTML 4.0 	right double quotation mark
				if (entity=="rsaquo")	return 0x203A;	// HTML 4.0 	single right-pointing angle quotation mark
				if (entity=="real")		return 0x211C;	// HTML 4.0 	black-letter capital r	 
				if (entity=="rarr")		return 0x2192;	// HTML 4.0 	rightwards arrow
				if (entity=="rArr")		return 0x21D2;	// HTML 4.0 	rightwards double arrow
				if (entity=="radic")	return 0x221A;	// HTML 4.0 	square root
				if (entity=="rceil")	return 0x2309;	// HTML 4.0 	right ceiling
				if (entity=="rfloor")	return 0x230B;	// HTML 4.0 	right floor
				if (entity=="rang")		return 0x232A;	// HTML 4.0 	right-pointing angle bracket
				break;
			case L's':
				if (entity=="shy")		return 0x00AD;	// HTML 3.2 	soft hyphen
				if (entity=="sup2")		return 0x00B2;	// HTML 3.2 	superscript two
				if (entity=="sup3")		return 0x00B3;	// HTML 3.2 	superscript three
				if (entity=="sup1")		return 0x00B9;	// HTML 3.2 	superscript one
				if (entity=="sect")		return 0x00A7;	// HTML 3.2 	section sign
				if (entity=="szlig")	return 0x00DF;	// HTML 2.0 	Latin small letter sharp s (German Eszett)
				if (entity=="scaron")	return 0x0161;	// HTML 4.0 	Latin small letter s with caron
				if (entity=="sigmaf")	return 0x03C2;	// HTML 4.0 	Greek small letter final sigma
				if (entity=="sigma")	return 0x03C3;	// HTML 4.0 	Greek small letter sigma
				if (entity=="sbquo")	return 0x201A;	// HTML 4.0 	single low-9 quotation mark
				if (entity=="sum")		return 0x2211;	// HTML 4.0 	n-ary summation
				if (entity=="sim")		return 0x223C;	// HTML 4.0 	tilde operator
				if (entity=="sub")		return 0x2282;	// HTML 4.0 	subset of
				if (entity=="sup")		return 0x2283;	// HTML 4.0 	superset of
				if (entity=="sube")		return 0x2286;	// HTML 4.0 	subset of or equal to
				if (entity=="supe")		return 0x2287;	// HTML 4.0 	superset of or equal to
				if (entity=="sdot")		return 0x22C5;	// HTML 4.0 	dot operator
				if (entity=="spades")	return 0x2660;	// HTML 4.0 	black spade suit
				break;
			case L't':
				if (entity=="times")	return 0x00D7;	// HTML 3.2 	multiplication sign
				if (entity=="thorn")	return 0x00FE;	// HTML 2.0 	Latin small letter thorn
				if (entity=="tilde")	return 0x02DC;	// HTML 4.0 	small tilde
				if (entity=="theta")	return 0x03B8;	// HTML 4.0 	Greek small letter theta
				if (entity=="tau")		return 0x03C4;	// HTML 4.0 	Greek small letter tau
				if (entity=="thetasym")	return 0x03D1;	// HTML 4.0 	Greek theta symbol
				if (entity=="thinsp")	return 0x2009;	// HTML 4.0 	thin space [3]
				if (entity=="trade")	return 0x2122;	// HTML 4.0 	trademark sign			 
				if (entity=="there4")	return 0x2234;	// HTML 4.0 	therefore
				break;
			case L'u':
				if (entity=="uml")		return 0x00A8;	// HTML 3.2 	diaeresis
				if (entity=="ugrave")	return 0x00F9;	// HTML 2.0 	Latin small letter u with grave
				if (entity=="uacute")	return 0x00FA;	// HTML 2.0 	Latin small letter u with acute
				if (entity=="ucirc")	return 0x00FB;	// HTML 2.0 	Latin small letter u with circumflex
				if (entity=="uuml")		return 0x00FC;	// HTML 2.0 	Latin small letter u with diaeresis
				if (entity=="upsilon")	return 0x03C5;	// HTML 4.0 	Greek small letter upsilon
				if (entity=="upsih")	return 0x03D2;	// HTML 4.0 	Greek upsilon with hook symbol
				if (entity=="uarr")		return 0x2191;	// HTML 4.0 	upwards arrow			 
				if (entity=="uArr")		return 0x21D1;	// HTML 4.0 	upwards double arrow
				break;
			case L'w':
				if (entity=="weierp")	return 0x2118;	// HTML 4.0 	script capital p (Weierstrass p)
				break;
			case L'x':
				if (entity=="xi")		return 0x03BE;	// HTML 4.0 	Greek small letter xi
				break;
			case L'y':
				if (entity=="yen")		return 0x00A5;	// HTML 3.2 	yen sign
				if (entity=="yacute")	return 0x00FD;	// HTML 2.0 	Latin small letter y with acute
				if (entity=="yuml")		return 0x00FF;	// HTML 2.0 	Latin small letter y with diaeresis
				break;
			case L'z':
				if (entity=="zeta")		return 0x03B6;	// HTML 4.0 	Greek small letter zeta
				if (entity=="zwnj")		return 0x200C;	// HTML 4.0 	zero width non-joiner
				if (entity=="zwj")		return 0x200D;	// HTML 4.0 	zero width joiner
				break;
			case L'A':
				if (entity=="Agrave")	return 0x00C0;	// HTML 2.0 	Latin capital letter a with grave
				if (entity=="Aacute")	return 0x00C1;	// HTML 2.0 	Latin capital letter a with acute
				if (entity=="Acirc")	return 0x00C2;	// HTML 2.0 	Latin capital letter a with circumflex
				if (entity=="Atilde")	return 0x00C3;	// HTML 2.0 	Latin capital letter a with tilde
				if (entity=="Auml")		return 0x00C4;	// HTML 2.0 	Latin capital letter a with diaeresis
				if (entity=="Aring")	return 0x00C5;	// HTML 2.0 	Latin capital letter a with ring above
				if (entity=="AElig")	return 0x00C6;	// HTML 2.0 	Latin capital letter ae
				if (entity=="Alpha")	return 0x0391;	// HTML 4.0 	Greek capital letter alpha
				break;
			case L'B':
				if (entity=="Beta")		return 0x0392;	// HTML 4.0 	Greek capital letter beta
				break;
			case L'C':
				if (entity=="Ccedil")	return 0x00C7;	// HTML 2.0 	Latin capital letter c with cedilla
				if (entity=="Chi")		return 0x03A7;	// HTML 4.0 	Greek capital letter chi
				break;
			case L'D':
				if (entity=="Delta")	return 0x0394;	// HTML 4.0 	Greek capital letter delta
				if (entity=="Dagger")	return 0x2021;	// HTML 4.0 	double dagger
				break;
			case L'E':
				if (entity=="Egrave")	return 0x00C8;	// HTML 2.0 	Latin capital letter e with grave
				if (entity=="Eacute")	return 0x00C9;	// HTML 2.0 	Latin capital letter e with acute
				if (entity=="Ecirc")	return 0x00CA;	// HTML 2.0 	Latin capital letter e with circumflex
				if (entity=="Euml")		return 0x00CB;	// HTML 2.0 	Latin capital letter e with diaeresis
				if (entity=="ETH")		return 0x00D0;	// HTML 2.0 	Latin capital letter eth
				if (entity=="Epsilon")	return 0x0395;	// HTML 4.0 	Greek capital letter epsilon
				if (entity=="Eta")		return 0x0397;	// HTML 4.0 	Greek capital letter eta
				break;
			case L'G':
				if (entity=="Gamma")	return 0x0393;	// HTML 4.0 	Greek capital letter gamma
				break;
			case L'I':
				if (entity=="Igrave")	return 0x00CC;	// HTML 2.0 	Latin capital letter i with grave
				if (entity=="Iacute")	return 0x00CD;	// HTML 2.0 	Latin capital letter i with acute
				if (entity=="Icirc")	return 0x00CE;	// HTML 2.0 	Latin capital letter i with circumflex
				if (entity=="Iuml")		return 0x00CF;	// HTML 2.0 	Latin capital letter i with diaeresis
				if (entity=="Iota")		return 0x0399;	// HTML 4.0 	Greek capital letter iota
				break;
			case L'K':
				if (entity=="Kappa")	return 0x039A;	// HTML 4.0 	Greek capital letter kappa
				break;
			case L'L':
				if (entity=="Lambda")	return 0x039B;	// HTML 4.0 	Greek capital letter lambda
				break;
			case L'M':
				if (entity=="Mu")		return 0x039C;	// HTML 4.0 	Greek capital letter mu
				break;
			case L'N':
				if (entity=="Ntilde")	return 0x00D1;	// HTML 2.0 	Latin capital letter n with tilde
				if (entity=="Nu")		return 0x039D;	// HTML 4.0 	Greek capital letter nu
				break;
			case L'O':
				if (entity=="Ograve")	return 0x00D2;	// HTML 2.0 	Latin capital letter o with grave
				if (entity=="Oacute")	return 0x00D3;	// HTML 2.0 	Latin capital letter o with acute
				if (entity=="Ocirc")	return 0x00D4;	// HTML 2.0 	Latin capital letter o with circumflex
				if (entity=="Otilde")	return 0x00D5;	// HTML 2.0 	Latin capital letter o with tilde
				if (entity=="Ouml")		return 0x00D6;	// HTML 2.0 	Latin capital letter o with diaeresis
				if (entity=="Oslash")	return 0x00D8;	// HTML 2.0 	Latin capital letter o with stroke
				if (entity=="OElig")	return 0x0152;	// HTML 4.0 	Latin capital ligature oe
				if (entity=="Omicron")	return 0x039F;	// HTML 4.0 	Greek capital letter omicron
				if (entity=="Omega")	return 0x03A9;	// HTML 4.0 	Greek capital letter omega
				break;
			case L'P':
				if (entity=="Pi")		return 0x03A0;	// HTML 4.0 	Greek capital letter pi
				if (entity=="Phi")		return 0x03A6;	// HTML 4.0 	Greek capital letter phi
				if (entity=="Psi")		return 0x03A8;	// HTML 4.0 	Greek capital letter psi
				if (entity=="Prime")	return 0x2033;	// HTML 4.0 	double prime
				break;
			case L'R':
				if (entity=="Rho")		return 0x03A1;	// HTML 4.0 	Greek capital letter rho
				break;
			case L'S':
				if (entity=="Scaron")	return 0x0160;	// HTML 4.0 	Latin capital letter s with caron
				if (entity=="Sigma")	return 0x03A3;	// HTML 4.0 	Greek capital letter sigma
				break;
			case L'T':
				if (entity=="THORN")	return 0x00DE;	// HTML 2.0 	Latin capital letter thorn
				if (entity=="Theta")	return 0x0398;	// HTML 4.0 	Greek capital letter theta
				if (entity=="Tau")		return 0x03A4;	// HTML 4.0 	Greek capital letter tau
				break;
			case L'U':
				if (entity=="Ugrave")	return 0x00D9;	// HTML 2.0 	Latin capital letter u with grave
				if (entity=="Uacute")	return 0x00DA;	// HTML 2.0 	Latin capital letter u with acute
				if (entity=="Ucirc")	return 0x00DB;	// HTML 2.0 	Latin capital letter u with circumflex
				if (entity=="Uuml")		return 0x00DC;	// HTML 2.0 	Latin capital letter u with diaeresis
				if (entity=="Upsilon")	return 0x03A5;	// HTML 4.0 	Greek capital letter upsilon
				break;
			case L'X':
				if (entity=="Xi")		return 0x039E;	// HTML 4.0 	Greek capital letter xi
				break;
			case L'Y':
				if (entity=="Yacute")	return 0x00DD;	// HTML 2.0 	Latin capital letter y with acute
				if (entity=="Yuml")		return 0x0178;	// HTML 4.0 	Latin capital letter y with diaeresis
				break;
			case L'Z':
				if (entity=="Zeta")		return 0x0396;	// HTML 4.0 	Greek capital letter zeta
				break;
			default: break;
			}
		}
	}
	return 0; // bad entity!
}


} // namespace dom

#include <llsdk/inc/memory/memory_end.h>

#endif // XML_ENTITIES_INCLUDED
