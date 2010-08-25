#!/bin/bash

#defaults
export GCC_VERSION=4.0
export OSX_VERSION=10.5
export SDK_VERSION=2.2.1
export FLAGS_FOR_DEVICE="-O3 -mdynamic-no-pic -funroll-loops -miphoneos-version-min=$SDK_VERSION -fomit-frame-pointer -march=armv6 -mcpu=arm1176jzf-    s -mfloat-abi=softfp -mfpu=vfp"
export FLAGS_FOR_SIMULATOR="-arch i386 -O0 -D__IPHONE_OS_VERSION_MIN_REQUIRED=20000 -mmacosx-version-min=$OSX_VERSION -gdwarf-2"
if [ -f "~/.iphone_build_settings" ]; then
	source ~/.iphone_build_settings
fi

LIB_NAME=$1
shift 1
# $0 sim $@ && $0 dev $@ && lipo -create "${LIB_NAME}-iPhoneOS${SDK_VERSION}.a" "${LIB_NAME}-iPhoneSimulator${SDK_VERSION}.a" -output "${LIB_NAME}-SDK${SDK_VERSION}.a" && rm "${LIB_NAME}-iPhoneOS${SDK_VERSION}.a" "${LIB_NAME}-iPhoneSimulator${SDK_VERSION}.a" && exit 0 || exit 1

if [ "$@" == "" ]; then
	echo "ERROR! No input files!"
	echo "Usage: $0 <library name> file1.cpp [file2.cpp [file3.cpp ...]]"
	exit 1
fi

function compile_for {
	local p=$1
	shift 1
	local PLATFORM_ROOT="/Developer/Platforms/${p}.platform/Developer"
	local BIN_DIR="${PLATFORM_ROOT}/usr/bin"
	local SDK_ROOT="${PLATFORM_ROOT}/SDKs/$p${SDK_VERSION}.sdk"
	local CMD="$BIN_DIR/gcc-$GCC_VERSION -x c++ -fmessage-length=0 -pipe -Wno-trigraphs -fno-rtti -fpascal-strings -fasm-blocks -Wreturn-type -Wunused-variable -fvisibility=hidden -fvisibility-inlines-hidden -fstrict-aliasing -I${SDK_ROOT}/usr/include"
	if [ "$p"  == "iPhoneOS" ]; then
		CMD="$CMD $FLAGS_FOR_DEVICE -arch armv6 -mdynamic-no-pic -miphoneos-version-min=$SDK_VERSION"
	fi
	if [ "$p" == "iPhoneSimulator" ]; then
		CMD="$CMD -arch i386 -O0 -D__IPHONE_OS_VERSION_MIN_REQUIRED=20000 -mmacosx-version-min=$OSX_VERSION -gdwarf-2"
	fi
	$CMD -combine -c -o $LIB_NAME-$p-all.o $@ && $BIN_DIR/ar -q $LIB_NAME-$p-all.a $LIB_NAME-$p-all.o && rm $LIB_NAME-$p-all.o
}

function compile {
	compile_for iPhoneOS $@ && \
	compile_for iPhoneSimulator $@ && \
	lipo -create $LIB_NAME-iPhoneOS-all.a $LIB_NAME-iPhoneSimulator-all.a -output $LIB_NAME.sdk$SDK_VERSION.a && \
	rm $LIB_NAME-*-all.a
}

compile $@
