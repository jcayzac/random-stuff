#!/bin/bash
# Usage: cd <source-dir> ; /path/to/span.sh <destination-dir>

Z="/tmp/span"
CNT=0
MAX=$(find . -type f | wc -l)
rm -rf "$Z"
mkdir -p "$Z"
find . -type f -mindepth 2 -not -iname ".DS_Store" -exec stat -f "%z %SN" "{}" \; | while read DATA
do
	echo "${DATA#* ./}" >"$( printf "${Z}/%020d-%020d" ${DATA%% *} $CNT)"
	[ $(( ++CNT % 10 )) == 0 ] && printf "\r%9d/$MAX files processed…" $CNT
done
echo -e "\r$MAX/$MAX files processed…          "

DVD_NUMBER=0
MAX_SIZE=4660000000
ROOT="."
if [ "$1" ]
then
	ROOT="$1"
fi
while [ "$(ls -A "$Z" )" ]
do
	DVD_SIZE=0
	(( ++DVD_NUMBER ))
	DVD_ROOT="${ROOT}/dvd${DVD_NUMBER}"
	mkdir -p "$DVD_ROOT"
	date +'Created on %Y-%m-%d %H:%M:%S' >"${DVD_ROOT}/creation_date.txt"
	ls -r "$Z" | while read FN
	do
		FN="${FN#$Z*}"
		SIZE="${FN%-*}"
		SIZE=${SIZE#${SIZE%%[1-9]*}*}
		WOULD_BE=$(( $DVD_SIZE + $SIZE ))
		if [ $(( $WOULD_BE < $MAX_SIZE )) == 1 ]
		then
			FILENAME="$(cat "$Z/$FN")"
			DIRNAME="${FILENAME%/*}"
			DVD_SIZE=$WOULD_BE
			IN_PERCENTS=$(( (DVD_SIZE*100)/MAX_SIZE ))
			printf "\rCreating DVD #${DVD_NUMBER}… %3d%%" $IN_PERCENTS
			mkdir -p "${DVD_ROOT}/${DIRNAME}"
			install -p "$FILENAME" "${DVD_ROOT}/${DIRNAME}/"
			rm "$Z/$FN"
		fi
	done
	echo -e "\rCreating DVD #${DVD_NUMBER}… 100%"
done

