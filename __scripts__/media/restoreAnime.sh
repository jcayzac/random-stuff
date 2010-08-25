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
mencoder -sws 10 -vf ${CROP}scale=720:-11,pp=ha/va/dr,hqdn3d,hue=0.0:1.2,pp=al,harddup "$SRC" -ovc lavc -lavcopts vcodec=ffv1:vstrict=-1:vhq:psnr -oac copy -o "${BASE} - tmp.avi" && \
echo -e "\033[01;32mEncoding as H.264+AAC MPEG4…\033[00m" && \
ffmpeg -i "${BASE} - tmp.avi" -f mp4 -vcodec libx264 -crf 23 -refs 3 -bf 0 -me_method umh -subq 6 -g 320 -keyint_min 25 -level 41 -qmin 10 -qmax 51 -qcomp 0.7 -trellis 1 -sc_threshold 40 -i_qfactor 0.71 -acodec libfaac -ab 128kb -ar 48000 -ac 2 "${BASE} - restored.mp4" && echo "OK"
exit $?

