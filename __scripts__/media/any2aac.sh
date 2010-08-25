#!/bin/bash
SRC="$1"
ffmpeg -i "$SRC" -y -acodec libfaac -ac 2 -ar 48000 -ab 192 "${SRC%.*}.aac"
exit $?
