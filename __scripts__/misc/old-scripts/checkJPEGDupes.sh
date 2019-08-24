#!/bin/bash

DIR="$1"
mkdir -p _stats_

FILES=0
find "$DIR" -type f | while read FILENAME
do
	FN="$FILENAME"
	SUBPATH="${FILENAME/$DIR/}"
	EXT=${FN##*.}
	if [ "$EXT" == "jpg" ] || [ "$EXT" == "jpeg" ]
	then
		if cat "$FN" >tmp && jhead -q -purejpg tmp >/dev/null 2>&1 ; then FN="tmp"; fi
	fi
	MD5SUM=$(md5sum "$FN")
	MD5SUM=${MD5SUM/ */}
	SHA1SUM=$(sha1sum "$FN")
	SHA1SUM=${SHA1SUM/ */}
	SIGNATURE="$MD5SUM$SHA1SUM"
	echo "$FILENAME" >>"_stats_/$SIGNATURE"
	[ $(( ++FILES % 10 )) == 0 ] && printf "\r%9d files processed…" $FILES
done
echo

