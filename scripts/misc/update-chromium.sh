#!/bin/bash

ARCHIVE="/tmp/chrome-mac.zip"
TMPDIR="/tmp/nbc.$$"
#LATEST=$(curl -s 'http://build.chromium.org/buildbot/snapshots/chromium-rel-mac/LATEST')
LATEST=$(curl -s 'http://build.chromium.org/buildbot/continuous/mac/LATEST/REVISION')

CARGS="-s"
ZARGS=/dev/null
if [ "$1" == "-v" ]
then
	CARGS=""
	ZARGS=&1
fi

echo -n "Downloading Chromium build ${LATEST}… "
[ -f "$ARCHIVE" ] && rm -rf "$ARCHIVE"
mkdir -p "$TMPDIR" && \
(curl $CARGS "http://build.chromium.org/buildbot/snapshots/chromium-rel-mac/${LATEST}/chrome-mac.zip" >"$ARCHIVE" && echo 'OK' || echo 'FAILED' ) && \
echo -n "Installing… " && \
ln -sf /Applications "$TMPDIR"/chrome-mac && \
rm -rf /Applications/Chromium.app && \
unzip "$ARCHIVE" -d "$TMPDIR" >$ZARGS 2>&1 && \
mv /Applications/Chromium.app/Contents/MacOS/Chromium /Applications/Chromium.app/Contents/MacOS/Chromium.real && \
(
cat >/Applications/Chromium.app/Contents/MacOS/Chromium <<EOT
#!/bin/bash
cd /Applications/Chromium.app/Contents/MacOS && /Applications/Chromium.app/Contents/MacOS/Chromium.real --enable-webgl --enable-geolocation --allow-file-access-from-files --enable-internal-flash --enable-nacl \$@
EOT
) && \
chmod 755 /Applications/Chromium.app/Contents/MacOS/Chromium && \
echo 'OK' || echo 'FAILED'

rm -rf "$ARCHIVE"
rm -rf "$TMPDIR"

