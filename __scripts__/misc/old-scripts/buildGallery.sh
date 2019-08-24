#!/bin/bash

GALLERY=""
DRY=""
NOHASH=""

while [ "$1" != "" ]
do
	case "$1" in
		-p | --pretend)
			DRY="1"
			shift
			;;
		-n | --nohash)
			NOHASH="1"
			shift
			;;
		*)
			GALLERY="$1"
			shift
			;;
	esac
done

if [ ${#GALLERY} == 0 ]
then
	echo "ERROR: No gallery specified!"
	exit 1
fi

if [ -e "$GALLERY" ]
then
	echo "WARNING: \"$GALLERY\" already exists…" >&2
fi

LOG="$GALLERY/log.txt"
mkdir -p "$GALLERY"
touch "$LOG"
echo "=== Log started on $(date) ===" >>"$LOG"

FMT='%Y%m%d%H%M%S'
find . -type f | while read FILENAME
do
	EXTENSION=$( echo "${FILENAME##*.}" | tr "[:upper:]" "[:lower:]" )
	BASENAME=$( basename "$FILENAME" )
	DATETIME=""
	MD5SUM="nohash"
	SIZE=$( printf '%08x' $(stat -f "%z" "$FILENAME") )
	if [ ${#NOHASH} == 0 ]
	then
		MD5SUM=$( md5sum "$FILENAME" | cut -d\  -f1 )
	fi
	case "$EXTENSION" in
		jpg)
			# exif utility from http://libexif.sourceforge.net/
			# Some pictures don't have a « Date and Time » tag,
			# but may have « Date and Time (Original) » or « Date and Time (Digitized »,
			# so we grep « Date and Time » and take the earliest date…
			DATETIME=$( exif -m "$FILENAME" 2>/dev/null | grep 'Date and Time' | sed 's/.*\([0-9][0-9][0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9]:[0-9][0-9]:[0-9][0-9]\)/\1/g' | sort | head -n 1 | sed 's/[: ]//g' )
			;;
		avi)
			# some AVI store the « digitized » date and time in
			# the IDIT chunk of the RIFF file…
			DUMP=$( strings "$FILENAME" | grep -A 1 IDIT | tail -n 1 )
			DATETIME=$( date -u -j -f '%Y/%m/%d/ %H:%M' "$DUMP" +'%Y%m%d%H%M%S' 2>/dev/null )
			if [ ${#DATETIME} != 14 ]; then DATETIME=$( date -u -j -f '%Y/%m/%d/ %H:%M'      "$DUMP" +"$FMT" 2>/dev/null ); fi
			if [ ${#DATETIME} != 14 ]; then DATETIME=$( date -u -j -f '%Y/%m/%d/ %H:%M:%S'   "$DUMP" +"$FMT" 2>/dev/null ); fi
			if [ ${#DATETIME} != 14 ]; then DATETIME=$( date -u -j -f '%Y/%m/%d %H:%M'       "$DUMP" +"$FMT" 2>/dev/null ); fi
			if [ ${#DATETIME} != 14 ]; then DATETIME=$( date -u -j -f '%Y/%m/%d %H:%M:%S'    "$DUMP" +"$FMT" 2>/dev/null ); fi
			if [ ${#DATETIME} != 14 ]; then DATETIME=$( date -u -j -f '%a %b %d %H:%M:%S %Y' "$DUMP" +"$FMT" 2>/dev/null ); fi
			if [ ${#DATETIME} != 14 ]; then DATETIME=$( python -c 'from email.utils import parsedate; print "%04i%02i%02i%02i%02i%02i"%parsedate("$DUMP")[:6]' 2>/dev/null ); fi
			;;
		mp4)
			# mp4file utility from https://sourceforge.net/projects/mpeg4ip/
			# MP4 store their creation time as seconds from 1904/1/1 00:00:00.
			# That's 2082844800 seconds before Unix's epoch.
			DATETIME=$( mp4file --dump "$FILENAME" | grep creation | cut -d '=' -f2 | cut -d ' ' -f2 | head -n1 )
			if [ ${#DATETIME} \> 0 ]; then DATETIME=$( expr "$DATETIME" \- 2082844800 ); fi
			if [ ${#DATETIME} \> 0 ]; then DATETIME=$( date -j -f "%s" "$DATETIME" +"$FMT" ); fi
			;;
	esac
	STATUS=""
	case "$EXTENSION" in
		jpg|avi|mp4)
			MISSING_DATE=" "
			EXISTS=" "
			if [ ${#DATETIME} != 14 ]
			then
				# Invalid datetime… let's use the Last Modified timestamp instead…
				DATETIME=$( date -u -j -f '%s' $(stat -f "%m" "$FILENAME" ) +"$FMT" )
				MISSING_DATE="M"
			fi
			NEWNAME="$( echo $DATETIME | sed 's/\(....\)\(..\)\(..\)\(......\)/\1\/\2\/\3\/\4/')"
			NEWNAME="${NEWNAME}-${MD5SUM}-${SIZE}.${EXTENSION}"
			if [ "$MISSING_DATE" == "M" ]
			then
				echo "* Using mtime for \"${NEWNAME}\":" >>"$LOG"
				echo "    …was \"$FILENAME\"" >>"$LOG"
			fi
			if [ -e "${GALLERY}/${NEWNAME}" ]
			then
				echo "* ${NEWNAME} already exists, skipping…" >>"$LOG"
				EXISTS="E"
			fi
			echo -e "${MISSING_DATE}${EXISTS} ${NEWNAME}\t${FILENAME}"
			mkdir -p "${GALLERY}/$(dirname ${NEWNAME})"
			if [ ${#DRY} == 0 ]
			then
				# copy the media file into the gallery
				mkdir -p "${GALLERY}/$(dirname ${NEWNAME})"
				cp "${FILENAME}" "${GALLERY}/${NEWNAME}"
				# change its Last Modified date to reflect when it got taken
				touch -c -t "$( echo $DATETIME | sed 's/\(............\)\(..\)/\1.\2/' )" "${GALLERY}/${NEWNAME}"
			fi
			;;
	esac
done

