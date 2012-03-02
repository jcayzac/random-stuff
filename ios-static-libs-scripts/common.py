#!/usr/bin/env python
import os
import pipes
import subprocess
import sys



SOURCEFORGE_MIRRORS = ['jaist', 'cdnetworks-kr-1', 'cdnetworks-kr-2']
ORIG_PWD=os.getcwd()
ORIG_PATH=os.environ['PATH']
SCRIPT=__FILE__

RAMDISK_NAME="temp-for-build"
if [ ! -d "/Volumes/$RAMDISK_NAME" ]
then
	# Use 1GB for build (should be enough for boost and icu)
	diskutil erasevolume HFS+ "$RAMDISK_NAME" $(hdiutil attach -nomount ram://2330860)
fi

MY_TMP="/Volumes/$RAMDISK_NAME/build-$LIB.$VERSION"
readonly MY_TMP
rm -rf "/Volumes/$RAMDISK_NAME"/build-* 2>/dev/null
DISTFILES="$TMPDIR/ios-static-libs-distfiles"
readonly DISTFILES
mkdir -p "$MY_TMP" "$DISTFILES"
cd "$MY_TMP"

# Kill process group and delete tmp dir when parent exits
sig_handler() {
	local RC=$? SIG=$1
	cd "$ORIG_PWD"
	export PATH="$ORIG_PATH"
	if [ "$SIG" == "EXIT" ]
	then
		case "$RC" in
		0)
			msg "${text_green}Installed [$LIB] version [$VERSION] in [$PREFIX]"
			echo
			echo "$LIB:$VERSION" >>"$PREFIX/.installed"
			;;
		*)
			msg "${text_white}${bg_red}ERROR: [$BASH_COMMAND] exited with return code $RC"
			;;
		esac
	fi

	while [ -d "$MY_TMP" ]
	do
		# This might fail if a background job is creating files
		rm -rf $MY_TMP >/dev/null 2>&1 || true
	done

	# Ctrl-C == DIE DIE DIE
	kill -s ${SIG/INT/KILL} 0
}

for sig in INT TERM EXIT
do
	trap "sig_handler $sig" $sig
done

if [ -z "$1" ]
then
	msg "Usage: $0 <root>"
	msg "	Installs library ${LIB} ${VERSION} for iOS in <root>/include and <root>/lib."
	exit 1
fi
PREFIX="$1"
readonly PREFIX
[ -d "$PREFIX/include" ] && [ -d "$PREFIX/lib" ] || mkdir -p "$PREFIX"/{include,lib} 2>/dev/null

# Use GNU make if possible
MAKE=make
if which gmake >/dev/null 2>&1
then
	MAKE=gmake
elif which gnumake >/dev/null 2>&1
then
	MAKE=gnumake
fi
MAKE="$MAKE -j4 -S"

CCACHE=""
if which ccache >/dev/null 2>&1
then
	CCACHE="$(which ccache) "
fi

# iOS env
if ! which xcodebuild >/dev/null 2>&1
then
	msg "Can't find xcodebuild in your PATH! Aborting."
	exit 1
fi

# It's needed for some builds (boost, ...)
IPHONE_SDKVERSION=$(xcodebuild 2>/dev/null -sdk iphoneos -version SDKVersion)

set_ios_sdk() {
	export SDK="$1"
	export CC="${CCACHE}$(xcodebuild 2>/dev/null -sdk $SDK -find gcc)"
	export CXX="${CCACHE}$(xcodebuild 2>/dev/null -sdk $SDK -find g++)"
	export AR="$(xcodebuild 2>/dev/null -sdk $SDK -find ar)"
	export AS="$(xcodebuild 2>/dev/null -sdk $SDK -find as)"
	export NM="$(xcodebuild 2>/dev/null -sdk $SDK -find nm)"
	export RANLIB="$(xcodebuild 2>/dev/null -sdk $SDK -find ranlib)"
	export SYSROOT="$(xcodebuild 2>/dev/null -version -sdk $SDK Path)"
	export PLATFORM_DIR="$(xcodebuild 2>/dev/null -version -sdk $SDK PlatformPath)"
	export CFLAGS="-O3 -pipe -gdwarf-2 -pthread -fvisibility=hidden -fstrict-aliasing -ffast-math -isysroot=$SYSROOT -DNDEBUG"
	export CPP="$CC -E"
	export CXXCPP="$CXX -E"
	export PATH="$(xcodebuild 2>/dev/null -version -sdk $SDK PlatformPath)/Developer/usr/bin::/Developer/usr/bin:$ORIG_PATH"
}

set_armv7() {
	set_ios_sdk "iphoneos"
	export CFLAGS="$CFLAGS -arch armv7 -mthumb -mcpu=cortex-a8 -mfloat-abi=softfp -mfpu=neon -ftree-vectorize"
	export CXXFLAGS="$CFLAGS -fvisibility-inlines-hidden"
	export CPPFLAGS="$CFLAGS"
	export HOST="arm-apple-darwin"
}

set_i386() {
	set_ios_sdk "iphonesimulator"
	export CFLAGS="$CFLAGS -arch i386 -march=i686 -msse3"
	export CXXFLAGS="$CFLAGS -fvisibility-inlines-hidden"
	export CPPFLAGS="$CFLAGS"
	export HOST="i686-apple-darwin"
}

set_android() {
	local NDK_BUILD=$(which ndk-build 2>/dev/null)
	if [ -z "$NDK_BUILD" ]
	then
		msg "ndk-build not in PATH"
		exit 1
	fi
	export HOST="arm-linux-androideabi"
	local NDK_PATH="${NDK_BUILD%/*}"
	local TOOLCHAIN=$(find -s "$NDK_PATH/toolchains" -depth 1 -iname "$HOST-*" | tail -n 1)
	if [ ! -d "$TOOLCHAIN" ]
	then
		msg "$HOST toolchain not found"
		exit 1
	fi
	local PLATFORM=$(find -s "$NDK_PATH/platforms" -depth 1 -iname "android-*" | tail -n 1)
	if [ ! -d "$PLATFORM" ]
	then
		msg "platform not found"
		exit 1
	fi

	case "$(uname -m)" in
		i?86) HOST_ARCH=x86
		;;
		amd64) HOST_ARCH=x86_64
		;;
		powerpc) HOST_ARCH=ppc
		;;
	esac

	local HOST_TAG=
	case "$(uname -s)" in
	Darwin)
		HOST_TAG="darwin-$HOST_ARCH"
		;;
	Linux)
		HOST_TAG="linux-$HOST_ARCH"
		;;
	FreeBsd)
		HOST_TAG="freebsd-$HOST_ARCH"
		;;
	CYGWIN*|*_NT-*)
		HOST_TAG=windows
		;;
	esac

	local PREBUILT_BIN="${TOOLCHAIN}/prebuilt/${HOST_TAG}/bin"
	export PATH=$PREBUILT_BIN:$PATH
	export CC="${CCACHE}${HOST}-gcc"
	export CXX="${CCACHE}${HOST}-g++"
	export AR="${HOST}-ar"
	export AS="${HOST}-as"
	export NM="${HOST}-nm"
	export RANLIB="${HOST}-ranlib"
	export CPP="$CC -E"
	export CXXCPP="$CXX -E"
	export SYSROOT="$PLATFORM/arch-arm"
	export CFLAGS="-mandroid -O3 -pipe -pthread -fvisibility=hidden -fstrict-aliasing -ffast-math --sysroot=$SYSROOT -DNDEBUG"
	export CFLAGS="$CFLAGS -mthumb -mfloat-abi=softfp -ftree-vectorize"
	if [ "$NEON" == "1" ]
	then
		export CFLAGS="$CFLAGS -mcpu=cortex-a9 -mfpu=neon -mvectorize-with-neon-quad"
	else
		export CFLAGS="$CFLAGS -march=armv7-a -mfpu=vfpv3-d16"
	fi
	CXXFLAGS0="\
-fvisibility-inlines-hidden \
-fexceptions \
-frtti \
-I$NDK_PATH/sources/cxx-stl/gnu-libstdc++/include \
-I$NDK_PATH/sources/cxx-stl/gnu-libstdc++/libs/armeabi-v7a/include \
"
	unset CPPFLAGS
}

set_android_neon() {
	TOOLCHAIN="arm-linux-androideabi"
	TRIPLET="arm-linux-androideabi"
	set_android
	export CFLAGS="$CFLAGS \
-mcpu=cortex-a9 \
-mfpu=neon \
-mvectorize-with-neon-quad \
-mthumb \
-mfloat-abi=softfp \
-ftree-vectorize \
"
	export CXXFLAGS="$CFLAGS $CXXFLAGS0"
}

set_android_vfp3() {
	TOOLCHAIN="arm-linux-androideabi"
	TRIPLET="arm-linux-androideabi"
	set_android
	export CFLAGS="$CFLAGS \
-march=armv7-a \
-mfpu=vfpv3-d16 \
-mthumb \
-mfloat-abi=softfp \
-ftree-vectorize \
"
	export CXXFLAGS="$CFLAGS $CXXFLAGS0"
}

set_android_x86() {
	TOOLCHAIN="x86"
	TRIPLET="i686-android-linux"
	set_android
	export CFLAGS="\
-march=i686 \
-msse3 \
"
	export CXXFLAGS="$CFLAGS $CXXFLAGS0"
}

# Download a file
# Usage:
# http_fetch <local file name> <remote HTTP URL>
http_fetch() {
	local FILE="$1"
	shift
	local MD5SUM="$1"
	shift

	if [ -f "$DISTFILES/$FILE" ]
	then
		if md5sum -s -c - <<< "$MD5SUM  $DISTFILES/$FILE"
		then
			ln -sf "${DISTFILES}/${FILE}"
			return
		fi
	fi

	# Expand mirrors
	local URLS=()
	for URL in $@
	do
		case "${URL%%:*}" in
		sourceforge)
			for TMP in $SOURCEFORGE_MIRRORS
			do
				URLS[${#URLS[*]}]=http://${TMP}.dl.sourceforge.net/project/${URL#*:}
			done
			;;
		*)
			URLS[${#URLS[*]}]=$URL
			;;
		esac
	done

	# aria2c and axel can download from multiple mirrors simultaneously
	if which aria2c >/dev/null 2>&1
	then
		aria2c -j5 -x5 -s5 -o "$FILE" ${URLS[@]}
	elif which axel >/dev/null 2>&1
	then
		axel -a -n 5 -o "$FILE" ${URLS[@]}
	elif which wget >/dev/null 2>&1
	then
		wget -ct0 -O "$FILE" "$URLS"
	elif which lwp-download >/dev/null 2>&1
	then
		lwp-download "$URLS" "$FILE"
	elif [ -z "${URLS/#http*/}" ] && which curl >/dev/null 2>&1
	then
		curl -L "$URLS" >"$FILE"
	elif [ -z "${URLS/#ftp*/}" ] && which ftp >/dev/null 2>&1
	then
		ftp -p -o "$FILE" "$URLS"
	else
		msg "Can't find any download tool! I suggest you install aria2:"
		msg
		msg "  sudo port selfupdate"
		msg "  sudo port install aria2"
		exit 1
	fi
	md5sum -s -c - <<<"$MD5SUM  $FILE"
	cp "$FILE" "$DISTFILES"/
}

# Show percentage as command runs in the background
# Usage:
# show_progress <total number of lines> <log file> <title> <command>
show_progress() {
	local LINE_COUNT=$1; shift
	local LOG_FILE="$1"; shift
	local TEXT="$1"; shift
	local FIFO=/tmp/log.$$
	cmd=$(cat)
	mkfifo $FIFO
	set -b
	/usr/bin/env bash -c "$cmd" >>$FIFO 2>"$LOG_FILE" &
	local PID=$!
	local I=0
	while read LINE
	do
		echo >>"$LOG_FILE" $LINE
		local PERCENT=$((I++ * 100 / $LINE_COUNT))
		# LINE_COUNT is not 100% accurate, so 101% etc could show up
		(( PERCENT <= 100 )) && printf "\r$TEXT: $PERCENT%%"
	done <$FIFO
	set +e
		wait $PID
		local RC=$?
	set -e
	rm $FIFO
	(( $RC == 0 ))
	printf "\r$TEXT: 100%%\n"
}

# Spawn commands asynchronously
# Usage:
# spawn [working directory] <<<"command"
spawn() {
	cmd=$(cat)
	cd ${1:-.} && /usr/bin/env bash -c "$cmd" &
}

# Spawn multiple commands concurrently
# Usage:
# spawn_multi [working directory] <<EOT
# command1a && command1b
# command2
# command3a || command3b
# EOT
spawn_multi() {
	local pid=()
	while read CMD
	do
		spawn ${1:-.} <<< "$CMD"
		pid[${#pid[*]}]=$!
	done
	wait ${pid[@]}
}

version_check_failed() {
	for x in $REQUIRED_VERSION
	do
		if [ "$1" \< "$x" ]
		then
			return 0
		fi
		shift
	done
	return 1
}

solve_dependencies() {
	# Parse installed packages
	local -A INSTALLED
	if [ -f "$PREFIX/.installed" ]
	then
		while read LINE
		do
			INSTALLED[${LINE%:*}]=${LINE#*:}
		done <"$PREFIX/.installed"
	fi

	# Parse requirements
	local OTHER_LIB OTHER_VERSION NEED_INSTALL
	for DEP in $DEPEND
	do
		OTHER_LIB="${DEP%:*}"
		OTHER_VERSION="${DEP#*:}"
		NEED_INSTALL=""
		REQUIRED_VERSION=${OTHER_VERSION//./ }
		HAS_VERSION=${INSTALLED[$OTHER_LIB]}
		echo -n "[Dependency check] $LIB needs ${OTHER_LIB} >= ${OTHER_VERSION}: "
		if [ -z "$HAS_VERSION" ]
		then
			NEED_INSTALL=y
			echo "NOT FOUND"
		elif version_check_failed ${HAS_VERSION//./ }
		then
			NEED_INSTALL=y
			echo "OUTDATED (Has v$HAS_VERSION)"
		fi
		if [ ! -z "$NEED_INSTALL" ]
		then
			read -sn 1 -p "${text_bold}Install $OTHER_LIB v$OTHER_VERSION [Y/n]? ${text_reset}" YESNO
			echo
			if [ "$YESNO" == "y" ] || [ "$YESNO" == "Y" ] || [ -z "$YESNO" ]
			then
				cd "$ORIG_PWD"
				bash "${SCRIPT%build-*}build-$OTHER_LIB" "$PREFIX"
				exec "$SCRIPT" "$PREFIX"
				exit $?
			else
				echo
				msg "Failed (missing requirement)."
				return 1
			fi
		else
			echo "FOUND"
		fi
	done
}

solve_dependencies

cat <<EOT
${text_bold}---------------------------------------------------------------------
             Library name: $LIB
          Library version: $VERSION
                  iOS SDK: $IPHONE_SDKVERSION
Temporary build directory: $MY_TMP
        Install directory: $PREFIX
---------------------------------------------------------------------
${text_reset}
EOT

# vim: set filetype=python fileencodings=utf-8 tabstop=4 shiftwidth=4 noexpandtab :

