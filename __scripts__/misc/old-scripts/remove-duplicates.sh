#!/bin/bash
TTY=$(tty)
while read LINE
do
	echo -n '.'
	FIRST="${LINE%    *}"
	SECOND="${LINE#*    }"
	if [ -r "$FIRST" ] && [ -r "$SECOND" ]
	then
		open "$FIRST" "$SECOND"
		sleep 1
		RUN=0
		while [ $RUN == 0 ]
		do
			ps ax | grep Preview | grep -v grep >/dev/null 2>&1
			RUN=$?
			sleep 0.1
		done
	fi
done <duplicates.txt

