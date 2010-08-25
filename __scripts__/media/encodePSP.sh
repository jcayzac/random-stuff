#!/bin/bash

SRC="$1"
BASE="$(basename "$SRC")"
EXT=${BASE##*.}
BASE=${BASE%%.${EXT}*}
DEST="${BASE} - PSP.mp4"

nice -n 10 /Applications/HandBrakeCLI                                                      \
                --verbose                                                       \
                --input "$SRC"                                                  \
                --maxWidth 480                                                  \
                --maxHeight 272                                                 \
                --encoder x264                                                  \
                --x264opts cabac=0:ref=2:me=umh:bframes=0:8x8dct=0:subme=6      \
                --vb 512                                                        \
                --aencoder faac                                                 \
                --ab 96                                                         \
                --arate 48                                                      \
                --output "$DEST"

#                --decomb                                                        \
#                --output "$DEST"

