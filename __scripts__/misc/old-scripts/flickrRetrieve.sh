#!/usr/bin/env bash

COOKIE=$( sqlite3 -list -separator = ~/Library/Application\ Support/Chromium/Default/Cookies "select name,value from cookies where host_key='.flickr.com'" | tr '\n' ';' )

rm curl.config
echo "cookie = \"$COOKIE\"" >curl.config
echo "retry = \"500\"" >>curl.config
echo >>curl.config
find -E . -type f -regex '.*/[0-9]+' | while read FILENAME
do
	ID=${FILENAME##*/} # same as ID=$(basename "$FILENAME"), only faster
	DIR=${FILENAME%*$ID} # same as DIR=$(dirname "$FILENAME"), only faster
	if [ -e "${FILENAME}.jpg" ] || \
	   [ -e "${FILENAME}.mp4" ] || \
	   [ -e "${FILENAME}.avi" ] || \
	   [ -e "${FILENAME}.png" ] || \
       [ -e "${FILENAME}.gif" ] || \
	   [ -e "${FILENAME}.mov" ]
	then
		rm "${FILENAME}" "${FILENAME}.dump" >/dev/null 2>&1
		echo "${FILENAME} already downloaded, skipping…"
	elif [ -r "${FILENAME}.dump" ]
	then
		URL=$(sed -n "s/.*\(http:\/\/farm..static.flickr.com\/[0-9][0-9]*\/${ID}_[0-9]*[^\"]*_d[^\"]*\).*/\1/p" "${FILENAME}.dump")
		if [ ${#URL} -gt 0 ]
		then
			echo "${FILENAME}: will fetch ${URL}…"
			EXT=$(echo "${URL##*.}" | tr "[:upper:]" "[:lower:]")
			echo "url = \"$URL\"" >>curl.config
			echo "output = \"${FILENAME}.${EXT}\"" >>curl.config
			echo >>curl.config
		else
			echo "${FILENAME}: bad dump, re-querying…"
			rm "${FILENAME}.dump"
			touch "${FILENAME}"
			echo "url = \"http://www.flickr.com/photos/jcayzac/${ID}/sizes/o/\"" >>curl.config
			echo "output = \"${FILENAME}.dump\"" >>curl.config
			echo >>curl.config
		fi
	else
		echo "${FILENAME}: requesting dump…"
		echo "url = \"http://www.flickr.com/photos/jcayzac/${ID}/sizes/o/\"" >>curl.config
		echo "output = \"${FILENAME}.dump\"" >>curl.config
		echo >>curl.config
	fi
done

