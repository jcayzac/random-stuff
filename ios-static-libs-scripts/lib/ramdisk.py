#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import os
import pipes
import subprocess
import sys
import uuid
from contextlib import contextmanager


DEFAULT_SIZE = 1024000


if sys.platform != 'darwin':
	raise OSError('Only MacOS X supported for now')


import plistlib
try:
    read_plist = plistlib.readPlistFromBytes
except AttributeError:
    read_plist = plistlib.readPlistFromString


class RamDisk(object):
	def __init__(self, size=DEFAULT_SIZE, name=None):
		if not name:
			name = 'ramdisk-%s' % uuid.uuid4().hex
		self.device = subprocess.check_output(
			['/usr/bin/hdiutil', 'attach' ,'-nomount' ,'ram://%i' % ((size + 511) / 512)]
		).strip()
		subprocess.check_output(
			['/usr/sbin/diskutil', 'erasevolume', 'hfsx', name, self.device]
		)
		self.path = read_plist(
			subprocess.check_output(
				['/usr/sbin/diskutil', 'info', '-plist', self.device]
			)
		)['MountPoint']

	def close(self):
		subprocess.check_output(
			['/usr/sbin/diskutil', 'eject', self.device]
		)


@contextmanager
def ramdisk(size=DEFAULT_SIZE, name=None):
	x = RamDisk(size, name)
	yield x
	x.close()

