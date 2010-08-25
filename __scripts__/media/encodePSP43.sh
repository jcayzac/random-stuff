#!/bin/bash

SRC="$1"
BASE="$(basename "$SRC")"
EXT=${BASE##*.}
BASE=${BASE%%.${EXT}*}
DEST="${BASE} - PSP.mp4"

ffmpeg $2 -i "$SRC" -f mp4 -vcodec libx264 \
-vb 512kb \
-refs 2 -bf 0 -me_method umh -me_range 16 -subq 6 \
-flags2 +mixed_refs -g 320 -keyint_min 25 -level 21 -qmin 10 -qmax 51 -qcomp 0.7 -trellis 2 \
-f psp \
-s 360x272 -padleft 60 -padright 60 -padcolor 000000 \
-acodec libfaac -ab 96kb -ar 48000 -ac 2 "$DEST"

