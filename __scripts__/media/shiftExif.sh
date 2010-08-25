#!/bin/bash
SELF=$(basename "$0")

function usage() {
	echo "$SELF [-]<seconds> [pic1.jpg] … [picN.jpg]" >&2
	exit 1
}

echo $1 | grep -E '^-?[0-9]+$' >/dev/null
if [ $? != 0 ]; then usage; fi
SHIFT="$1"
shift

while [ "$1" != "" ]
do
	FILENAME="$1"
	shift
	if [ -w "$FILENAME" ]
	then
		DATE="$(exif -m "$FILENAME" 2>/dev/null | grep 'Date and Time' | cut -d\	 -f2 | sort | head -n 1)"
		if [ ${#DATE} == 19 ]
		then
			STAMP="$(date -u -j -f '%Y:%m:%d %H:%M:%S' "$DATE" +'%s')"
			STAMP=$( echo "$STAMP + $SHIFT" | bc)
			STATUS=0
			if [ $STAMP -lt 0 ]; then STAMP=0; fi
			if [ $STAMP -gt 2147483647 ]; then STAMP=2147483647; fi
			echo -n -e "$FILENAME:\t$DATE → "
			DATE=$(date -u -j -f '%s' "$STAMP" +'%Y:%m:%d %H:%M:%S')
			echo $DATE
			exif -c --ifd=0 --tag=0x0132 --set-value="$DATE" -o "${FILENAME}.$$" "$FILENAME" >/dev/null 2>&1 && \
			exif -c --ifd=EXIF --tag=0x9003 --set-value="$DATE" -o "${FILENAME}.$$" "${FILENAME}.$$" >/dev/null 2>&1 && \
			exif -c --ifd=EXIF --tag=0x9004 --set-value="$DATE" -o "$FILENAME" "${FILENAME}.$$" >/dev/null 2>&1
			STATUS=$?
			if [ $STATUS != 0 ]
			then
				echo "WARNING: Failed to update $FILENAME" >&2
			fi
			rm "${FILENAME}.$$" 2>/dev/null
		else
			echo "WARNING: Skipping $FILENAME (no EXIF date)…" >&2
		fi
	else
		echo "ERROR: $FILENAME is not writable!" >&2
		exit 1
	fi
done
