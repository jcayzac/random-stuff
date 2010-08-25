import os,sys,re, struct,array,stat,math
import os.path
import getopt

try:
	opts, args = getopt.getopt(sys.argv[1:], '', ["show=","input="])
	print opts
except getopt.error, msg:
	print msg
	sys.exit(2)

dbShow=[0,1]
dbFile='geosignal.db'
for (k,v) in opts:
	if k=='--input':
		dbFile=v
	elif v=='--show':
		values=v.split(',')
		dbShow=[]
		for x in values:
			if x=='gsm':
				dbShow.append(0)
			elif x=='wifi':
				dbShow.append(1)
			else:
				print "--show accept only 'gsm' or 'wifi' comma-separated values"
				sys.exit(3)
if len(dbShow)==0:
	print "Empty dataset requested"
	sys.exit(3)

KIconWifi='http://maps.google.com/mapfiles/kml/paddle/W.png'
KIconCell='http://maps.google.com/mapfiles/kml/paddle/grn-stars.png'
KIconDot='http://maps.google.com/mapfiles/kml/pal4/icon25.png'

KMLSpotBegin="""<Placemark>
<description>%(desc)s</description>
<Style><IconStyle><scale>0.75</scale><Icon><href>%(icon)s</href></Icon></IconStyle></Style>
<Point>
<coordinates>
%(x)f,%(y)f,0
</coordinates>
</Point>
</Placemark>
<Placemark>
<description>%(desc)s</description>
<Style><LineStyle><color>%(color)s</color><width>%(thickness)i</width></LineStyle></Style>
<LineString>
	<tessellate>1</tessellate>
	<altitudeMode>clampToGround</altitudeMode>
	<coordinates>
"""

KMLSpotEnd="""			</coordinates>
		</LineString>
	</Placemark>
"""

KMLHeader="""<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://earth.google.com/kml/2.1">
<Document>
"""
KMLFooter="""</Document>
</kml>
"""

DAL=111226.29991434248924368723
DALR=.00000899067936962857
def wgs84toSpherical(lon,lat):
	if lat<-85.0: lat=-85.0
	elif lat>85.0: lat=85.0
	x=lon*DAL*math.cos(math.radians(lat))
	y=lat*DAL
	return (x,y)
def sphericalToWgs84(x,y):
	lat=y*DALR
	lon=x*DALR
	lon=lon/math.cos(math.radians(lat))
	return (lon,lat)

def printCircleCoordinates(lon,lat,radiusMeters,o):
	(x0,y0)=wgs84toSpherical(lon,lat)
	for degree in xrange(0,360,2):
		rad=math.radians(degree)
		x=x0+math.cos(rad)*radiusMeters
		y=y0+math.sin(rad)*radiusMeters
		(x,y)=sphericalToWgs84(x,y)
		o.write("%f,%f,0\n"%(x,y))
def printAP(vals,o):
	o.write(KMLSpotBegin%vals)
	printCircleCoordinates(vals['centerx'],vals['centery'],vals['radius'],o)
	o.write(KMLSpotEnd)
	o.write("""<Placemark>
<description>%(desc)s</description>
<Style><LineStyle><color>ffff00ff</color><width>2</width></LineStyle></Style>
<LineString>
	<tessellate>1</tessellate>
	<altitudeMode>clampToGround</altitudeMode>
	<coordinates>
	%(x)f,%(y)f,0
	%(centerx)f,%(centery)f,0
	</coordinates>
	</LineString>
</Placemark>
"""%vals)

def writeDataForAP(type,apid,data,o):
	icon=KIconDot
	if type==0:
		icon=KIconCell
	elif type==1:
		icon=KIconWifi
	sums={}
	counts={}
	for (ss,lon,lat) in data:
		if not sums.has_key(ss):
			sums[ss]={}
			sums[ss][0]=0.0
			sums[ss][1]=0.0
		if not counts.has_key(ss):
			counts[ss]=0
		sums[ss][0]=sums[ss][0]+lon
		sums[ss][1]=sums[ss][1]+lat
		counts[ss]=counts[ss]+1
	# compute maximum signal strength for this AP
	maxSS=0
	for ss in counts.keys():
		if ss>maxSS:
			maxSS=ss
	# Consider the upper 10 percents
	upperLon=0.0
	upperLat=0.0
	upperCount=0
	for i in counts.keys()[::-1]:
		upperLon=upperLon+sums[i][0]
		upperLat=upperLat+sums[i][1]
		upperCount=upperCount+counts[i]
		if (maxSS-i>=10):
			break
	upperLon=upperLon/float(upperCount)
	upperLat=upperLat/float(upperCount)
	#
	maxSsLon=sums[maxSS][0]/float(counts[maxSS])
	maxSsLat=sums[maxSS][1]/float(counts[maxSS])
	# Compute mean
	meanLon=0.0
	meanLat=0.0
	meanWeight=0
	for ss in sums.keys():
		meanLon=meanLon+(sums[ss][0]*float(counts[ss]))
		meanLat=meanLat+(sums[ss][1]*float(counts[ss]))
		meanWeight=meanWeight+float(counts[ss]*counts[ss])
	if meanWeight>0.0:
		meanLon=meanLon/meanWeight
		meanLat=meanLat/meanWeight
	# compute average
	avgLon=0.0
	avgLat=0.0
	for ss in sums.keys():
		avgLon=avgLon+sums[ss][0]
		avgLat=avgLat+sums[ss][1]
	avgLon=avgLon/len(data)
	avgLat=avgLat/len(data)
	
	# compute radius from average
	radius2=0.0
	(avgX,avgY)=wgs84toSpherical(avgLon,avgLat)
	for (ss,lon,lat) in data:
		(x,y)=wgs84toSpherical(lon,lat)
		x=x-avgX
		y=y-avgY
		r2=x*x + y*y
		if r2>radius2: radius2=r2
	radius=math.sqrt(radius2)
	# plot everything
	color='ffffffff'
	plotname="%s<br/>\n(%i/%i samples)"%(apid,upperCount,len(data))
	vals={
		'desc':plotname,
		'icon':icon,
		'x':upperLon,
		'y':upperLat,
		'color':color,
		'thickness':2,
		'centerx':avgLon,
		'centery':avgLat,
		'radius':radius,
	}
	printAP(vals,o)
	#o.write(KMLSpotBegin%vals)
	#printCircleCoordinates(avgLon,avgLat,radius,o)
	#o.write(KMLSpotEnd)

s=os.stat(dbFile)[stat.ST_SIZE]
if s>32:
	if (s%32>0):
		print "        !%s is not recognized!"%filename
		sys.exit(1)
	
	i=open(dbFile,'rb')
	o=open('report.kml','w')
	o.write(KMLHeader)
	
	info={}
	while(s>0):
		s=s-32
		(type,id1,id2,aid,cid,lon,lat,ss)=struct.unpack('IIIIIffI',i.read(32))
		id=(id1<<96)+(id2<<64)+(aid<<32)+cid
		if not info.has_key(id):
			info[id]={'type':type,'data':[]}
		info[id]['data'].append((ss,lon,lat))
	i.close()
	
	for id in info.keys():
		idStr=''
		if info[id]['type']==0:
			mcc=(id>>96)&0xffffffff
			mnc=(id>>64)&0xffffffff
			aid=(id>>32)&0xffffffff
			cid=id&0xffffffff
			idStr="%i-%i-%i-%i"%(mcc,mnc,aid,cid)
		elif info[id]['type']==1:
			hi=(id>>96)&0xffffffff
			lo=(id>>64)&0xffffffff
			hmac=[((hi>>16)&0xff),((hi>>8)&0xff),(hi&0xff),((lo>>16)&0xff),((lo>>8)&0xff),(lo&0xff)]
			idStr="%02x:%02x:%02x:%02x:%02x:%02x"%(hmac[0],hmac[1],hmac[2],hmac[3],hmac[4],hmac[5])
		writeDataForAP(info[id]['type'],idStr,info[id]['data'],o)
	o.write(KMLFooter)
	o.close()
print 'Done'
