#!/bin/bash

if [ ! -r "$1" ]
then
	echo >&2 "Can't open \"$1\" for reading…"
	exit 1
fi

TITLE="MYDVD"
if [ "$2" ]
then
	TITLE="$2"
fi

MPG="${1%.*}_tmp.mpg"
DVD_DIR="${1%.*}_dvd"
ISO="${1%.*}.iso"
mkdir -p "$DVD_DIR"
ffmpeg -i "$1" -aspect 16:9 -target ntsc-dvd -threads 4 "$MPG" && dvdauthor -o "$DVD_DIR" -t "$MPG" && rm "$MPG" && dvdauthor -o “$DVD_DIR” -T && mkisofs -dvd-video -V "$TITLE" -o "$ISO" "$DVD_DIR" && rm -rf "$DVD_DIR"


