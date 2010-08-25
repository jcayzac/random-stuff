import os,sys,re, struct,array,stat
import os.path

try:
	traceFile=sys.argv[1]
	print "Processing %s"%traceFile
except:
	print "Usage: %s <traceFile>"%sys.argv[0]
	raise

s=os.stat(traceFile)[stat.ST_SIZE]
if (s>32):
	if (s%32>0):
		print "        !%s is not recognized!"%traceFile
		sys.exit(1)
	i=open(traceFile,'rb')
	o=open('geosignal.db','ab')
	while(s>0):
		s=s-32
		o.write(i.read(32))
	o.close()
	i.close()
sys.exit(0)
