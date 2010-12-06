Patch for building [Boost](http://www.boost.org/) for iOS
================================

This is a patch for building Boost 1.45.0 for iOS 4.2.

Supported architectures are:
* armv7 (device)
* x86 (simulator)

Omitted libraries:
* python
* graph
* graph_parallel
* mpi
* wave
* program_options

The iostreams library is built, but without BZip2 compression support.

Installation
===
* Download boost and uncompress it somewhere. 
* cd my-boost-directory
* patch -p1 < boost_1_45_0.ios.patch
* ./bootstrap.sh --without-libraries=python,graph,graph_parallel,mpi,wave,program_options
* ./bjam --prefix=${HOME}/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.2.sdk/usr toolset=darwin-4.2.1~iphone architecture=arm target-os=iphone macosx-version=iphone-4.2 link=static threading=multi --layout=versioned --variant=release install 
* ./bjam --prefix=${HOME}/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator4.2.sdk/usr toolset=darwin-4.2.1~iphonesim architecture=x86 target-os=iphone macosx-version=iphone-4.2 link=static threading=multi --layout=versioned --variant=release install


