#!/bin/bash
# params: "path" "YYYY/MM/DD HH:MM:SS"

if [ ! -r "$1" ]
then
	echo "« $1 » is not readable"
	exit 1
fi
echo "Setting « $1 »'s date to « $2 »…"

OLDDATE=$( mp4file --dump "$1" | grep creationTime | sed 's/.*(0x\(..\)\(..\)\(..\)\(..\))/\\x\1\\x\2\\x\3\\x\4/g' | head -n 1)

NEWDATE=$( date -u -j -f '%Y/%m/%d %H:%M:%S' "$2" +'%s' )
NEWDATE=$( expr "$NEWDATE" \+ 2082844800 )
NEWDATE=$( python -c "print '%08x' % $NEWDATE" | sed 's/\(..\)\(..\)\(..\)\(..\)/\\x\1\\x\2\\x\3\\x\4/' )
echo "old: $OLDDATE, new: $NEWDATE"

if [ ${#OLDDATE} != 16 ] || [ ${#NEWDATE} != 16 ]
then
	echo "Bad date format!"
	echo "Usage: $0 'filepath' 'YYYY/MM/DD HH:MM:SS'"
	exit 1
fi

echo "processing…"
PERLPROG="s/$OLDDATE/$NEWDATE/g"
cat "$1" | perl -pe "$PERLPROG" >"Updated - $1"
if [ ! -r "Updated - $1" ]
then
	echo "Failed to write « Updated - $1 »"
	exit 1
else
	echo "wrote « Updated - $1 »"
fi

