#!/bin/bash

BASE=$(basename "$1")
OUTPUT="${1%.*}.mp4"
TEMPORARY="${1%.*}.tmp"
echo -n "$1 → ${OUTPUT}… "
rm "$OUTPUT" "$TEMPORARY" >/dev/null 2>&1

TIMESTAMP=$( exiftool -s -d '%s' "$1"|grep DateTime|sed 's/.*: //g'|sort|head -n 1 )
if [ -z "$TIMESTAMP" ]
then
	# try to figure out the date by looking at the directory structure
	REALDIR="$( cd "$( pwd )/$( dirname "$1" )"; pwd )/"
	YYYYMMDD=$(echo $REALDIR | sed 's/.*\/\([0-9][0-9][0-9][0-9]\)\/\([0-9][0-9]\)\/\([0-9][0-9]\)\/.*/\1\/\2\/\3 12:00:00/')
	if [ -z "$YYYYMMDD" ]
	then
		TIMESTAMP="0"
		echo "No time found for \"$1\" (will be 1970/01/01 00:00:00)" >&2
	else
		TIMESTAMP=$( date -u -j -f '%Y/%m/%d %H:%M:%S' "$YYYYMMDD" +'%s' )
		echo "Deduced date for \"$1\": $YYYYMMDD" >&2
	fi
fi
YEAR="-metadata year=$( date -u -j -f '%s' $TIMESTAMP +'%Y' )"

# Convert the video to mp4 and save it in a temporary file
ffmpeg -i "$1" -y -f mp4 -vcodec libx264 -crf 22 -threads 0 -flags +loop -cmp +chroma -deblockalpha -1 -deblockbeta -1 -refs 3 -bf 3 -coder 1 -me_method hex -me_range 18 -subq 7 -partitions +parti4x4+parti8x8+partp8x8+partb8x8 -g 320 -keyint_min 25 -level 41 -qmin 10 -qmax 51 -qcomp 0.7 -trellis 1 -sc_threshold 40 -i_qfactor 0.71 -flags2 +mixed_refs+dct8x8+wpred+bpyramid -padcolor 000000 -padtop 0 -padbottom 0 -padleft 0 -padright 0 -acodec libfaac -ab 192kb -ar 48000 -ac 2 $YEAR -metadata comment="$BASE" -timestamp now "$TEMPORARY" >/dev/null 2>&1
STATUS=$?
if [ $STATUS != 0 ]
then
	echo FAILED
	rm "$TEMPORARY" >/dev/null 2>&1
else
	# Attempt to patch the MP4 file with the creation time from the original AVI
	OLDDATE=$( mp4file --dump "$TEMPORARY" | grep creationTime | sed 's/.*(0x\(..\)\(..\)\(..\)\(..\))/\\x\1\\x\2\\x\3\\x\4/g' | head -n 1)
	NEWDATE=$( printf '%08x' $( expr $TIMESTAMP \+ 2082844800 ) | sed 's/\(..\)\(..\)\(..\)\(..\)/\\x\1\\x\2\\x\3\\x\4/' )
	if [ ${#OLDDATE} == 16 ] && [ ${#NEWDATE} == 16 ]
	then
		echo -n "setting date… "
		PERLPROG="s/$OLDDATE/$NEWDATE/g"
		cat "$TEMPORARY" | perl -pe "$PERLPROG" >"$OUTPUT"
		rm "$TEMPORARY"
	else
		mv "$TEMPORARY" "$OUTPUT"
	fi
	echo OK
fi
exit $STATUS

