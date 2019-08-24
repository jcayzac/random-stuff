#!/bin/bash

TTY=$(tty)
#echo "* Pass 1"
#CNT=0
#mkdir -p _stats_
#find . -iname "*.jpg" -type f | while read FN
#do
#	LOWERNAME=$(echo "${FN##*/}" | tr "[:upper:]" "[:lower:]")
#	echo "$FN" >> "_stats_/$LOWERNAME"
#	[ $(( ++CNT % 10 )) == 0 ] && printf "\r%9d files processed…" $CNT
#done

echo "* Pass 2"
CNT=0
find _stats_ -type f | while read FN
do
	unset FILES;
	while read L
	do
		if [ -r "$L" ]
		then
			FILES[${#FILES[@]}]="$L"
		fi
	done < "$FN"

	# only one file named like this
	if [ ${#FILES[@]} == 1 ]
	then
		if [ "${FILES[0]%./root*}" == "" ]
		then
			echo rm "$FN"
		fi
	fi
	[ $(( ++CNT % 10 )) == 0 ] && printf "\r%9d files processed…" $CNT
done
exit 0

while [ false ]
do
	if [ $(( ${#FILES[@]} > 1)) == 1 ]
	then
		N=0
		PREV_DATE=""
		DATE_CHANGES=0
		unset CHDUP
		for (( I=0; I<${#FILES[@]}; ++I))
		do
			DATA=$(exif -m "${FILES[$I]}" 2>/dev/null |grep 'Date and Time'|head -n 1) || DATA=""
			D=${DATA//[^0-9:]/}
			D="${D:0:10} ${D:10:8}"
			if [ ${#D} != 19 ]
			then
				D="[     no date     ]"
			else
				N=$I
				if [ "$PREV_DATE" != "$D" ]
				then
					(( ++DATE_CHANGES ))
					PREV_DATE="$D"
				fi
			fi
			CHDUP[${#CHDUP[@]}]="[$I] $D ${FILES[$I]}"
		done

		if [ "$DATE_CHANGES" != "1" ]
		then
			echo -e "\nDuplicates:"
			for (( I=0; I<${#CHDUP[@]}; ++I))
			do
				echo -en "\t"
				echo "${CHDUP[$I]}"
			done
			read <"$TTY" -p "Choice: " N
			N=${N//[^0-9]/}
			if [ ${#N} != 0 ]
			then
				echo "Keeping only ${FILES[$N]}…"
			fi
		fi

		if [ ${#N} != 0 ]
		then
			for (( I=0; I<${#FILES[@]}; ++I))
			do
				if [ "$I" != "$N" ]
				then
					rm "${FILES[$I]}"
				fi
			done
		fi
	fi
	[ $(( ++CNT % 10 )) == 0 ] && printf "\r%9d files processed…" $CNT
done

