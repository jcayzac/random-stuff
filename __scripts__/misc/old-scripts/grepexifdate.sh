#!/bin/bash
echo -n "$1"
exif -m "$1" | grep Date | cut -d\	 -f2 | while read LINE; do echo -ne ",${LINE}"; done
echo

