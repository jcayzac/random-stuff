#!/bin/bash

BASE=${1%.*}
WAVE="${BASE}.wav"
OGG="${BASE}.ogg"

[ ! -r "$WAVE" ] && afconvert -f WAVE -d LEI16@44100 "$1" "$WAVE"
~/oggenc "$WAVE"


