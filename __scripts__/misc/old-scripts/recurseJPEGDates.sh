#!/bin/bash
SELF="$0"
TTY=$(tty)
EDITALL="N"
while [ "$1" != "" ]
do
	case "$1" in
	-p | --pipe)
		TTY=""
		shift
		;;
	-a | --all)
		EDITALL="Y"
		shift
		;;
	*)
		echo "Usage: $SELF [-p|--pipe] [-a|--all]
where:

	-p	Report only (no dialog). No colors.
	-a	Treat all pictures. Has no effect if -p is specified.
" >&2
		exit 1
		shift
		;;
	esac
done

COL1="\033[01;32m"
COL0="\033[00m"
if [ ! -e "$TTY" ]
then
	COL1=""
	COL0=""
fi

function changeDate() {
	F="$1"
	D="$2"
	exif -c --ifd=0 --tag=0x0132 --set-value="$D" -o "$F" "$F" >/dev/null 2>&1
	exif -c --ifd=EXIF --tag=0x9003 --set-value="$D" -o "$F" "$F" >/dev/null 2>&1
	exif -c --ifd=EXIF --tag=0x9004 --set-value="$D" -o "$F" "$F" >/dev/null 2>&1
}

function sHHMMtoSeconds() {
	echo "$1" | sed 's/^-\(.*\)/-(\1)/' | sed 's/\([0-9]*\):\([0-9]*\)/\1*3600+\2*60/' | bc
}

PREVSHIFT=""
LASTDATEPLUSONE=""
FILELENGTH=0

find . -type f -iname "*.jpg" | while read FILENAME; do
	if [ ${#FILENAME} -gt $FILELENGTH ]; then FILELENGTH=${#FILENAME}; fi

	DATE="$(exif -m "$FILENAME" 2>/dev/null | grep 'Date and Time' | cut -d\	 -f2 | sort | head -n 1)"
	MTIME=$(date -u -j -f '%s' $(stat -f '%m' "$FILENAME") +'%Y:%m:%d %H:%M:%S')
	FROMDIR=$(echo $( cd "$( pwd )/$( dirname "$FILENAME" )"; pwd )/ | sed 's/.*\/\([0-9][0-9][0-9][0-9]\)\/\([0-9][0-9]\)\/\([0-9][0-9]\)\/.*/\1:\2:\3 12:00:00/')

	MISMATCH="N"
	NODATE="N"
	UPDATELAST="N"
	DATECHANGED="N"
	if [ ${#DATE} != 19 ]
	then
		NODATE="Y"
		echo -e "${COL1}$FILENAME has no date${COL0}"
	elif [ ${#FROMDIR} == 19 ] && [ "${FROMDIR%% *}" != "${DATE%% *}" ]
	then
		MISMATCH="Y"
		echo -e "${COL1}$(printf "%-${FILELENGTH}s" "$FILENAME")${COL0}\t$DATE\t${COL1}(folder says date is${COL0} ${FROMDIR%% *}${COL1})${COL0}"
	else
		echo -e "${COL1}$(printf "%-${FILELENGTH}s" "$FILENAME")${COL0}\t$DATE"
	fi

	if [ "$NODATE$MISMATCH$EDITALL" != "NNN" ] && [ -e "$TTY" ]
	then
		LOOP="Y"
		while [ ${#LOOP} -gt 0 ]
		do
			echo "  [0] View"
			echo "  [1] Skip for now"
			echo "  [2] Set manually"
			if [ "$NODATE" == "N" ]
			then
				if [ ${#PREVSHIFT} -gt 0 ]; then echo "  [3] Shift by $PREVSHIFT"; fi
				echo "  [4] Shift by…"
			fi
			echo "  [5] From mtime: $MTIME"
			if [ ${#FROMDIR} == 19 ]; then echo "  [6] From dir:   $FROMDIR"; fi
			if [ ${#LASTDATEPLUSONE} -gt 0 ]; then echo "  [7] Last date plus 1s: $LASTDATEPLUSONE"; fi
			read <"$TTY" -s -n1 -p ">> " KEY
			echo $KEY
			case $KEY in
			0)	echo "opening…" && open "$FILENAME"
				;;
			1)	echo "Skipping…"
				LOOP=""
				;;
			2)	echo -e -n "${COL1}Date (e.g. ${COL0}2001:12:24 18:41:30${COL1}: ${COL0}"
				read <"$TTY" DATE
				if ( echo $DATE | grep -E '[0-9][0-9][0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9]:[0-9][0-9]:[0-9][0-9]' >/dev/null )
				then
					changeDate "$FILENAME" "$DATE"
					LOOP=""
					DATECHANGED="Y"
				else
					echo "Wrong date format"
				fi
				;;
			3)	if [ "$NODATE" == "N" ] && [ ${#PREVSHIFT} -gt 0 ]
				then
					~/shiftExif.sh $(sHHMMtoSeconds "$PREVSHIFT") "$FILENAME"
					DATECHANGED="Y"
				fi
				;;
			4)	if [ "$NODATE" == "N" ]
				then
					echo "TODO"
				fi
				;;
			5)	changeDate "$FILENAME" "$MTIME"
				LOOP=""
				DATECHANGED="Y"
				;;
			6)	if [ ${#FROMDIR} == 19 ]
				then
					changeDate "$FILENAME" "$FROMDIR"
					LOOP=""
					DATECHANGED="Y"
				fi
				;;
			7)	if [ ${#LASTDATEPLUSONE} -gt 0 ]
				then
					changeDate "$FILENAME" "$LASTDATEPLUSONE"
					LOOP=""
					DATECHANGED="Y"
				fi
				;;
			*)	echo "Try again…"
				;;
			esac
		done
		if [ "$DATECHANGED" == "Y" ]; then UPDATELAST="Y"; fi
	fi

	if [ "$DATECHANGED" == "N" ] && [ "$NODATE" == "N" ]; then UPDATELAST="Y"; fi
	if [ "$UPDATELAST" == "Y" ]
	then
		STAMP=$(exif -m "$FILENAME" 2>/dev/null | grep 'Date and Time' | cut -d\	 -f2 | sort | head -n 1)
		STAMP=$(echo "1 + $(date -u -j -f '%Y:%m:%d %H:%M:%S' "$STAMP" +'%s')" | bc )
		if [ $STAMP -gt 2147483647 ]; then STAMP=2147483647; fi
		LASTDATEPLUSONE=$(date -u -j -f '%s' $STAMP +'%Y:%m:%d %H:%M:%S')
	fi
done

