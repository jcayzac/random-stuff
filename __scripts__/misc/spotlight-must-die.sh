#!/usr/bin/env bash

# execute as root
if (( $UID > 0 ))
then
	sudo $0 $@
	exit $?
fi

# disable indexing on all volumes
mdutil -a -i off

# stop spotlight
launchctl unload -w /System/Library/LaunchDaemons/com.apple.metadata.mds.plist

# change to non-executable so it won't be started at boot time
chmod 600 /System/Library/CoreServices/Search.bundle/Contents/MacOS/Search

# remove spotlight icon
killall SystemUIServer

