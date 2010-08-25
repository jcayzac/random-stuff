# Bost Minidom

## Purpose
My aim is to provide C++ developers with a portable lightweight DOM implementation depending only on the STL and Boost libraries.

A few months ago, I had to write such a thing from scratch for Windows Mobile and SymbianOS. A preliminary implementation hence exists, but it heavily depends upon closed-source components that are owned by my employer.

## Features
The old version supported asynchronous loading of XML document, automatic decoding of various charsets (utf-8, utf-16/ucs-2, ucs4, latin1, latin9, win1252 and plain ASCII), and XPath queries.

For simplicity, this new version will only support synchronous operations. I will re-introduce asynchronous loading once I add support for DOM events.

## Limitations
The main limitation comes from a design decision, in order to keep a simple memory management and get all the benefits of automatic memory reclaiming without having to use/write/depend upon a garbage collector. As a result, you must store the Document object somewhere while you want to manipulate its nodes and only keeping a child Node won't suffice.

## Status
* Right now, creating and manipulating DOM nodes should work. However, using the library for other things beside testing is not recommended right now (think of it as pre-alpha code).
* The DOMReader class has just been posted in trunk, and I have yet to test it. The old class worked and this is a port/boostifization/cleanup so there shouldn't be any big problem with it.
* The library compiles with Visual C++ 2005 SP1 on Windows and with GCC 4.1.2 on a Linux Gentoo box, so it might as well compile when using another compiler. Portability is a priority, but testing all the compilers is obviously not one at this stage of development.

