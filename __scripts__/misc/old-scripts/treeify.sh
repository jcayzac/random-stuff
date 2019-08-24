#!/bin/bash

find "$1" -iname "*.jpg" | while read FN
do
	BASENAME=${FN%.*}
	BASENAME=${BASENAME##*/}
	DATE=$(exif -m "$FN" 2>/dev/null |grep 'Date and Time'|tail -n 1)
	DATE=${DATE//[^0-9]/}
	if [ ${#DATE} == 14 ]
	then
		DIR2="${DATE:0:4}/${DATE:4:2}/${DATE:6:2}"
		mkdir -p "$DIR2"
		mv "$FN" "${DIR2}/${BASENAME}.jpg"
	fi
done

find "$1" -iname "*.mp4" | while read FN
do
	BASENAME=${FN%.*}
	BASENAME=${BASENAME##*/}
	DATE=$(date -juf %s $(($(mp4file --dump "$FN"|grep eationT|head -n 1|sed 's/.* = \([0-9][0-9]*\).*/\1/')-0x7C25B080)) +"%Y/%m/%d")
	if [ ${#DATE} == 10 ]
	then
		mkdir -p "$DATE"
		mv "$FN" "$DATE/${BASENAME}.mp4"
	fi
done
