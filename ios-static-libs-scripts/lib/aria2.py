#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import subprocess
import xmlrpclib
import uuid
import random

random.seed()

PORT = 12001

aria2_daemon = subprocess.Popen(
	[
		'aria2c', '-D', '--enable-rpc=true',
		'--rpc-listen-port=%i' % PORT
	]
)
aria2_daemon.wait()

aria2 = xmlrpclib.ServerProxy("http://127.0.0.1:%i/rpc" % PORT).aria2
x = aria2.addUri(['http://chromeos.hexxeh.net/ChromeOS-Zero.torrent'],{'dir':'.'})
print x
print aria2.tellStatus(x)
