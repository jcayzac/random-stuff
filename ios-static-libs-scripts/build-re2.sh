#!/usr/bin/env bash
COMMON_CXXFLAGS="-O3 -pipe -gdwarf-2 -pthread -fvisibility=hidden -fvisibility-inlines-hidden"
MAKE_FLAGS="-j4 -s"

set -e

if [ -z "$1" ]
then
    echo >&2 "Usage: $0 <root>"
    echo >&2 "    Installs the RE2 library for iOS in <root>/include"
    echo >&2 "                                    and <root>/lib."
    exit 1
fi
PREFIX="$1"
[ -d "$PREFIX/include" ] && [ -d "$PREFIX/lib" ] || mkdir -p "$PREFIX"/{include,lib} 2>/dev/null

MY_TMP="$TMPDIR/re2.$$"
echo "Building RE2 in [$MY_TMP]"
mkdir -p "$MY_TMP"
trap "rm -rf \"$MY_TMP\"" INT TERM EXIT
cd "$MY_TMP"

if ! hg clone https://code.google.com/p/re2 >/dev/null 2>&1
then
    echo >&2 "Failed to clone [https://code.google.com/p/re2] into [$TMPDIR/re2.$$/re2]"
    exit 1
fi

cd re2

IPHONE_SDKVERSION=$(
    xcodebuild -showsdks 2>/dev/null | \
    sed -n 's#.*-sdk iphoneos\([0-9\.]*\)#\1#p' | \
    sort -r | \
    head -n 1
)
echo "Building for iOS ${IPHONE_SDKVERSION}."

DEV_DIR=$("xcode-select" --print-path)

# ARM builds
ARM_PLATFORM_DIR="${DEV_DIR}/Platforms/iPhoneOS.platform"
ARM_DEV_DIR="${ARM_PLATFORM_DIR}/Developer/usr/bin"
ARM_SDK="${ARM_PLATFORM_DIR}/Developer/SDKs/iPhoneOS${IPHONE_SDKVERSION}.sdk"

echo -n "  Building ARMv6 library... "
make $MAKE_FLAGS clean
make $MAKE_FLAGS -s CC="${ARM_DEV_DIR}/g++-4.2" AR="${ARM_DEV_DIR}/ar" NM="${ARM_DEV_DIR}/nm" \
CXXFLAGS="$COMMON_CXXFLAGS \
-arch armv6 -marm -D_LITTLE_ENDIAN -isysroot ${ARM_SDK}" \
obj/libre2.a
mv obj/libre2.a "$PREFIX"/lib/libre2-armv6.a
echo OK

echo -n "  Building ARMv7 library... "
make $MAKE_FLAGS clean
make $MAKE_FLAGS CC="${ARM_DEV_DIR}/g++-4.2" AR="${ARM_DEV_DIR}/ar" NM="${ARM_DEV_DIR}/nm" \
CXXFLAGS="$COMMON_CXXFLAGS \
-arch armv7 -mthumb -D_LITTLE_ENDIAN -isysroot ${ARM_SDK}" \
obj/libre2.a
mv obj/libre2.a "$PREFIX"/lib/libre2-armv7.a
echo OK

# i386 build
SIM_PLATFORM_DIR="${DEV_DIR}/Platforms/iPhoneSimulator.platform"
SIM_DEV_DIR="${SIM_PLATFORM_DIR}/Developer/usr/bin"
SIM_SDK="${SIM_PLATFORM_DIR}/Developer/SDKs/iPhoneSimulator${IPHONE_SDKVERSION}.sdk"

echo -n "  Building i386 library... "
make $MAKE_FLAGS clean
make $MAKE_FLAGS CC="${SIM_DEV_DIR}/g++-4.2" AR="${SIM_DEV_DIR}/ar" NM="${SIM_DEV_DIR}/nm" \
CXXFLAGS="$COMMON_CXXFLAGS \
-arch i386 -isysroot ${SIM_SDK}" \
obj/libre2.a
mv obj/libre2.a "$PREFIX"/lib/libre2-i386.a
echo OK

echo -n "Creating fat library... "
lipo -create -output "$PREFIX"/lib/libre2.a \
    -arch armv6 "$PREFIX"/lib/libre2-armv6.a \
    -arch armv7 "$PREFIX"/lib/libre2-armv7.a \
    -arch i386  "$PREFIX"/lib/libre2-i386.a
rm "$PREFIX"/lib/libre2-{armv6,armv7,i386}.a
echo OK

echo -n "Moving headers into place... "
cp re2/{re2,set,stringpiece,variadic_function}.h "$PREFIX"/include/
echo OK

echo "Done. Installed in [$PREFIX]"
