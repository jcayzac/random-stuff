#!/usr/bin/env bash

# build our cookie…
COOKIE=$( sqlite3 -list -separator = ~/Library/Application\ Support/Chromium/Default/Cookies "select name,value from cookies where host_key='.flickr.com'" | tr '\n' ';' )

MAXLENGTH=50

function docurl() {
	curl -b "$COOKIE" --retry 500 -s "$1"
	STATUS=$?
	if [ $STATUS != 0 ]; then
		echo >&2 "FAILED $1"
		exit $STATUS
	else
		echo >&2 "OK     $1"
	fi
}

function getPhotosIDs() {
	YEAR=$1
	MONTH=$2
	DAY=$3
	grep -E "<a href=\"\/photos\/jcayzac\/[0-9]*\/in\/datetaken\/\"" tmp.$$ | sed "s/^.*\/photos\/jcayzac\/\([0-9]*\)\/in\/datetaken\/.*/\1/" | while read PHOTO_ID; do
		mkdir -p "$YEAR/$MONTH/$DAY"
		if [ ! -e "$YEAR/$MONTH/$DAY/$PHOTO_ID" ]; then echo "+ $YEAR/$MONTH/$DAY/$PHOTO_ID"; fi
		touch "$YEAR/$MONTH/$DAY/$PHOTO_ID"
	done
}

docurl "http://www.flickr.com/photos/jcayzac/archives/date-taken/calendar/" | grep -E '<a href="\/photos\/jcayzac\/archives\/date-taken\/[0-9][0-9][0-9][0-9]\/calendar\/">[0-9][0-9][0-9][0-9]<\/a>' | sed 's/^.*\/photos\/jcayzac\/archives\/date-taken\/\([0-9][0-9][0-9][0-9]\)\/calendar\/.*/\1/' | sort -u | while read YEAR; do
	sleep 5
	docurl "http://www.flickr.com/photos/jcayzac/archives/date-taken/${YEAR}/calendar/" | grep -E "<a href=\"\/photos\/jcayzac\/archives\/date-taken\/${YEAR}\/[0-9][0-9]\/[0-9][0-9]\/\">" | sed "s/^.*\/photos\/jcayzac\/archives\/date-taken\/${YEAR}\/\([0-9][0-9]\)\/\([0-9][0-9]\)\/.*/\1\/\2/" | while read MONTH_AND_DAY; do
		sleep 5
		DAY=${MONTH_AND_DAY##*/}
		MONTH=${MONTH_AND_DAY%%/*}
		PAGE=1
		docurl "http://www.flickr.com/photos/jcayzac/archives/date-taken/${YEAR}/${MONTH_AND_DAY}" >tmp.$$
		getPhotosIDs $YEAR $MONTH $DAY
		while grep 'next &rarr;<\/a>' tmp.$$ >/dev/null
		do
			sleep 5
			PAGE=$(expr $PAGE \+ 1)
			docurl "http://www.flickr.com/photos/jcayzac/archives/date-taken/${YEAR}/${MONTH_AND_DAY}/page${PAGE}/" >tmp.$$
			getPhotosIDs $YEAR $MONTH $DAY
		done
	done
done

