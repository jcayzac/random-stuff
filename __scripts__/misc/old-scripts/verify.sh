#!/bin/bash

find "$1" -iname "*.jpg" | while read FN
do
	DIR1="${FN#*./}"; DIR1="${DIR1%/*}"
	DATE=$(exif -m "$FN" 2>/dev/null |grep 'Date and Time'|tail -n 1)
	DATE=${DATE//[^0-9]/}
	if [ ${#DATE} == 14 ]
	then
		DIR2="${DATE:0:4}/${DATE:4:2}/${DATE:6:2}"
		if [ "$DIR1" != "$DIR2" ]
		then
			echo "$FN: dir-from-date is $DIR2, not $DIR1"
		fi
	fi
done

find "$1" -iname "*.mp4" | while read FN
do
	DIR1="${FN#*./}"; DIR1="${DIR1%/*}"
	DIR2=$(date -juf %s $(($(mp4file --dump "$FN"|grep eationT|head -n 1|sed 's/.* = \([0-9][0-9]*\).*/\1/')-0x7C25B080)) +"%Y/%m/%d")
	if [ ${#DIR2} == 10 ]
	then
		if [ "$DIR1" != "$DIR2" ]
		then
			echo "$FN: dir-from-date is $DIR2, not $DIR1"
		fi
	fi
done
