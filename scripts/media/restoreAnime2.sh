#!/bin/bash

SRC="$1"
FILENAME="${SRC##*/}"
EXTENSION="${SRC##*.}"
BASE="${SRC%.*}"

echo -en "\033[01;32mDetecting framerate…\033[00m "
FPS=$(mplayer -endpos 0 -vo null -ao null "$SRC" 2>/dev/null | grep VIDEO)
FPS=${FPS%% fps*}
FPS=${FPS##* }
echo $FPS

echo -en "\033[01;32mDetecting settings for cropping…\033[00m "
CROP=$(mplayer -nosound -ss 300 -endpos 5 -vf cropdetect -vo null "$SRC" 2>/dev/null | grep 'vf crop=' | tail -n 1)
CROP=${CROP##*=}
CROP=${CROP%%)*}
CROP="crop=${CROP},"
echo $CROP

echo -e "\033[01;32mScaling/Cropping/Filtering original video…\033[00m"
ffmpeg -i "$SRC" -y -deinterlace -nr 30 -f mp4 -vcodec libx264 -crf 24 -threads 0 -flags +loop -cmp +chroma -deblockalpha 2 -deblockbeta 2 -refs 3 -bf 3 -coder 1 -me_method hex -me_range 18 -subq 7 -partitions +parti4x4+parti8x8+partp8x8+partb8x8 -g 320 -keyint_min 25 -level 41 -qmin 10 -qmax 51 -qcomp 0.7 -trellis 1 -sc_threshold 40 -i_qfactor 0.71 -flags2 +mixed_refs+dct8x8+wpred+bpyramid -padcolor 000000 -padtop 0 -padbottom 0 -padleft 0 -padright 0 -acodec libfaac -ab 192kb -ar 48000 -ac 2 -timestamp now "${BASE} - restored.mp4"
exit $?

