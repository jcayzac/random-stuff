# Source this file in other scripts

if [ -z "$LIB" ]
then
	echo >&2 "Library is not named. Please declare LIB before sourcing 'common'."
	exit 1
fi

if [ -z "$VERSION" ]
then
	echo >&2 "Library [$LIB] is not versioned. Please declare VERSION before sourcing 'common'."
	exit 1
fi

# Fail and exit on first error
set -e

SCRIPT="$0"
if [ "${SCRIPT:0:1}" != "/" ]
then
	pushd $(dirname "$SCRIPT") >/dev/null
	SCRIPT_DIR=$(pwd)
	popd >/dev/null
	SCRIPT="$SCRIPT_DIR/${SCRIPT#*/}"
fi


RAMDISK_NAME="temp-for-build"
if [ ! -d "/Volumes/$RAMDISK_NAME" ]
then
	diskutil erasevolume HFS+ "$RAMDISK_NAME" $(hdiutil attach -nomount ram://1165430)
fi

MY_TMP="/Volumes/$RAMDISK_NAME/build-$LIB.$VERSION"
rm -rf "/Volumes/$RAMDISK_NAME"/build-* 2>/dev/null
DISTFILES="$TMPDIR/ios-static-libs-distfiles"
mkdir -p "$MY_TMP" "$DISTFILES"
cd "$MY_TMP"

# Kill process group and delete tmp dir when parent exits
sig_handler() {
	local RC=$? SIG=$1
	if [ "$RC" == "0" ] && [ "$SIG" == "EXIT" ]
	then
		echo "Installed [$LIB] version [$VERSION] in [$PREFIX]"
		echo "$LIB:$VERSION" >>"$PREFIX/.installed"
	fi
	#rm -rf "$MY_TMP"

	# Ctrl-C == DIE DIE DIE
	kill -s ${SIG/INT/KILL} 0
}

for sig in INT TERM EXIT
do
	trap "sig_handler $sig" $sig
done

if [ -z "$1" ]
then
    echo >&2 "Usage: $0 <root>"
    echo >&2 "    Installs library ${LIB} ${VERSION} for iOS in <root>/include and <root>/lib."
    exit 1
fi
PREFIX="$1"
[ -d "$PREFIX/include" ] && [ -d "$PREFIX/lib" ] || mkdir -p "$PREFIX"/{include,lib} 2>/dev/null

# Color macros
f() {
	local esc='['
	text_reset=${esc}0m
	text_bold=${esc}1m
	text_underscore=${esc}4m
	text_blink=${esc}5m
	text_reverse=${esc}7m
	text_concealed=${esc}8m

	local black=0
	local red=1
	local green=2
	local yellow=3
	local blue=4
	local magenta=5
	local cyan=6
	local white=7

	text_black=${esc}3${black}m
	text_red=${esc}3${red}m
	text_green=${esc}3${green}m
	text_yellow=${esc}3${yellow}m
	text_blue=${esc}3${blue}m
	text_magenta=${esc}3${magenta}m
	text_cyan=${esc}3${cyan}m
	text_white=${esc}3${white}m

	bg_black=${esc}4${black}m
	bg_red=${esc}4${red}m
	bg_green=${esc}4${green}m
	bg_yellow=${esc}4${yellow}m
	bg_blue=${esc}4${blue}m
	bg_magenta=${esc}4${magenta}m
	bg_cyan=${esc}4${cyan}m
	bg_white=${esc}4${white}m
}; f; unset f

# Use GNU make if possible
MAKE=make
if which gmake >/dev/null 2>&1
then
	MAKE=gmake
elif which gnumake >/dev/null 2>&1
then
	MAKE=gnumake
fi
MAKE="$MAKE -j4"

CCACHE=""
if which ccache >/dev/null 2>&1
then
	CCACHE="$(which ccache) "
fi

# iOS env
IPHONE_SDKVERSION=$(
	xcodebuild -showsdks 2>/dev/null | \
	sed -n 's#.*-sdk iphoneos\([0-9\.]*\)#\1#p' | \
	sort -r | \
	head -n 1
)

DEV_DIR=$(xcode-select --print-path)
ARM_PLATFORM_DIR="${DEV_DIR}/Platforms/iPhoneOS.platform"
SIM_PLATFORM_DIR="${DEV_DIR}/Platforms/iPhoneSimulator.platform"
ARM_DEV_DIR="${ARM_PLATFORM_DIR}/Developer/usr/bin"
SIM_DEV_DIR="${SIM_PLATFORM_DIR}/Developer/usr/bin"
ARM_SDK="${ARM_PLATFORM_DIR}/Developer/SDKs/iPhoneOS${IPHONE_SDKVERSION}.sdk"
SIM_SDK="${SIM_PLATFORM_DIR}/Developer/SDKs/iPhoneSimulator${IPHONE_SDKVERSION}.sdk"

set_armv7() {
    export CC="${CCACHE}${ARM_DEV_DIR}/gcc-4.2"
    export CXX="${CCACHE}${ARM_DEV_DIR}/g++-4.2"
    export AR="${ARM_DEV_DIR}/ar"
    export NM="${ARM_DEV_DIR}/nm"
    export RANLIB="${ARM_DEV_DIR}/ranlib"
    export CFLAGS="-O3 -pipe -gdwarf-2 -pthread -fvisibility=hidden -arch armv7 -mthumb -D_LITTLE_ENDIAN -isysroot $ARM_SDK"
    export CXXFLAGS="$CFLAGS -fvisibility-inlines-hidden"
}

set_i386() {
    export CC="${CCACHE}${SIM_DEV_DIR}/gcc-4.2"
    export CXX="${CCACHE}${SIM_DEV_DIR}/g++-4.2"
    export AR="${SIM_DEV_DIR}/ar"
    export NM="${SIM_DEV_DIR}/nm"
    export RANLIB="${SIM_DEV_DIR}/ranlib"
    export CFLAGS="-O3 -pipe -gdwarf-2 -pthread -fvisibility=hidden -arch i386 -isysroot $SIM_SDK"
    export CXXFLAGS="$CFLAGS -fvisibility-inlines-hidden"
}

# Download a file
# Usage:
# http_fetch <local file name> <remote HTTP URL>
http_fetch() {
	FILE="$1"
	shift
	MD5SUM="$1"
	shift
	if [ -f "$DISTFILES/$FILE" ]
	then
		if md5sum -s -c - <<< "$MD5SUM  $DISTFILES/$FILE"
		then
			ln -sf "${DISTFILES}/${FILE}"
			return
		fi
	fi

	if which aria2c >/dev/null 2>&1
	then
		aria2c -j5 -x5 -s5 -o "$FILE" $@
	elif which wget >/dev/null 2>&1
	then
		wget -ct0 -O "$FILE" "$1"
	else
		curl -L "$1" >"$FILE"
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
	done <$FIFO || true
	set +e
		wait $PID
		RC=$?
	set -e
	rm $FIFO
	if (( $RC == 0 ))
	then
		printf "\r$TEXT: 100%%\n"
	else
		echo " *FAILED*"
		cat <"$LOG_FILE"
		exit $RC
	fi
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

REQUIRED_VERSION=
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
		echo -n "Dependency check: $LIB needs ${OTHER_LIB}-${OTHER_VERSION}: "
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
			read -sn 1 -p "Install $OTHER_LIB v$OTHER_VERSION [Y/n]? " YESNO
			echo
			if [ "$YESNO" == "y" ] || [ "$YESNO" == "Y" ] || [ -z "$YESNO" ]
			then
				bash "${SCRIPT%build-*}build-$OTHER_LIB" "$PREFIX"
				exec "$SCRIPT" "$PREFIX"
				exit $?
			else
				echo
				echo "Failed (missing requirement)."
				return 1
			fi
		else
			echo "FOUND"
		fi
	done
}

solve_dependencies

cat <<EOT
---------------------------------------------------------------------
             Library name: $LIB
          Library version: $VERSION
                  iOS SDK: $IPHONE_SDKVERSION
Temporary build directory: $MY_TMP
        Install directory: $PREFIX
---------------------------------------------------------------------

EOT

# vim: set filetype=sh fileencodings=utf-8 tabstop=4 shiftwidth=4 noexpandtab :

