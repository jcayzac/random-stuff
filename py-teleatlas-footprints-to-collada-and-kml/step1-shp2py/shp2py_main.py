import os,sys,os.path,math,subprocess
import Blender
import bpy
import BPyMesh
import BPyImage
import BPyMessages


quick_debug=False # True


_SETS=['3dbf','3drp']
_EXTENSIONS=['dbf','prj','shp','shx']

class Attributes:
	BUILDING=0
	FOOTPRINT=1
	ROOF=2
	PEDIMENT=3
	ELEVATION=4
	HEIGHT=5
	COL=6
	GEOM=7

def executeManyCommandLines(cmdlines):
	for cmdline in cmdlines:
		print 'Running command "%s"...'%(' '.join(cmdline))
		program=subprocess.Popen(cmdline,stdout=subprocess.PIPE)
		errors=program.communicate()[1]
		if errors:
			print errors
		retcode=program.returncode
	return True

def parseFile(name):
	print 'Reading "%s"...'%name
	i=open(name,"r")
	indata=False
	current=""
	max=10000
	for x in i:
		x=x.strip()
		if x=='BEGINDATA':
			indata=True
			continue
		if indata==False:
			continue
		if x[0]=='#':
			current+=x[1:]
			continue
		if len(current)>0:
			current=current.replace('|/','|END|')
			if current[-1]=='|':
				current=current[:-1]
			current=current.split('|')
			yield current
			if quick_debug:
				max-=1
				if max==0:
					break
		current=x
	if len(current)>0 and max>0:
		current=current.replace('|/','|END|')
		if current[-1]=='|':
			current=current[:-1]
		current=current.split('|')
		yield current
	i.close()
def parseGeom(x,addz=0.0):
	index=0
	elems=[]
	geom=[]
	while x[index]!='END':
		offset=int(x[index])-1
		elem_type=int(x[index+1])
		interp=int(x[index+2])
		elems.append((offset,elem_type,interp))
		index+=3
	index+=1
	if len(elems)!=1:
		raise 'Assertion failed: only one SDO_ELEM_INFO!'
	if elems[0]!=(0,3,1):
		raise 'Assertion failed: SDO_ELEM_INFO==[1,3,1]! %s'%repr(elems[0])
	while x[index]!='END':
		geom.append([float(x[index]),float(x[index+1]),addz+float(x[index+2])])
		index+=3
	return geom

def create_footprints(file,groups,ids):
	lines=parseFile(file)
	for x in lines:
		id=int(x[0])
		group=int(x[1])
		try:
			n=groups[group]
		except:
			groups[group]=[]
		groups[group].append(id)
		try:
			n=ids[id]
		except:
			ids[id]={}
		ids[id][Attributes.BUILDING]=group
		ids[id][Attributes.FOOTPRINT]=id
		ids[id][Attributes.ELEVATION]=float(x[2])
		ids[id][Attributes.HEIGHT]=float(x[3])
		ids[id][Attributes.COL]=x[4]
		if int(x[5])!=3:
			raise 'Type!=3!'
		ids[id][Attributes.GEOM]=parseGeom(x[7:],ids[id][Attributes.ELEVATION])

def create_roofs(file,ids):
	lines=parseFile(file)
	for x in lines:
		id=int(x[0])
		fpid=int(x[1])
		groundheight=float(x[2])
		if int(x[3])!=3:
			raise 'Type!=3!'
		try:
			n=ids[fpid]
		except:
			ids[fpid]={}
		try:
			n=ids[fpid][Attributes.ROOF]
		except:
			ids[fpid][Attributes.ROOF]=[]
		try:
			addz=ids[fpid][Attributes.ELEVATION]
		except:
			addz=0.0
		ids[fpid][Attributes.ROOF].append(parseGeom(x[5:],groundheight))
def create_pediments(file,ids):
	lines=parseFile(file)
	for x in lines:
		id=int(x[0])
		fpid=int(x[1])
		groundheight=float(x[3])
		if int(x[4])!=3:
			raise 'Type!=3! for pediment #%i'%id
		try:
			n=ids[fpid]
		except:
			ids[fpid]={}
		try:
			n=ids[fpid][Attributes.PEDIMENT]
		except:
			ids[fpid][Attributes.PEDIMENT]=[]
		try:
			addz=ids[fpid][Attributes.ELEVATION]
		except:
			addz=0.0
		ids[fpid][Attributes.PEDIMENT].append(parseGeom(x[6:],groundheight))

def process(database,datapath,groups,ids):
	print 'Importing database "%s"...'%database
	dbname=os.path.basename(database)
	cmdlines=[
		['shp2sdo.exe', '%s_3dbf'%database, '%s_3dbf'%dbname, '-g', 'geom', '-d', '-x', '(-180,180)', '-y', '(-90,90)', '-s', '8307', '-t', '0,5', '-v'],
		['shp2sdo.exe', '%s_3drp'%database, '%s_3drp'%dbname, '-g', 'geom', '-d', '-x', '(-180,180)', '-y', '(-90,90)', '-s', '8307', '-t', '0,5', '-v'],
		['shp2sdo.exe', '%s_3dpf'%database, '%s_3dpf'%dbname, '-g', 'geom', '-d', '-x', '(-180,180)', '-y', '(-90,90)', '-s', '8307', '-t', '0,5', '-v'],
	]
	bf='%s_3dbf.ctl'%dbname
	rp='%s_3drp.ctl'%dbname
	pf='%s_3dpf.ctl'%dbname
	if (not os.path.isfile(bf)) or (not os.path.isfile(rp)) or (not os.path.isfile(pf)):
		print ".CTL files not found, generating new ones..."
		executeManyCommandLines(cmdlines)
	create_footprints('%s_3dbf.ctl'%dbname,groups,ids)
	create_roofs('%s_3drp.ctl'%dbname,ids)
	create_pediments('%s_3dpf.ctl'%dbname,ids)

def load_everything(datapath):
	print "Loading everything under directory %s"%datapath
	databases={}
	for name in os.listdir(datapath):
		fullpath=os.path.sep.join([datapath,name])
		dbname=fullpath[:-9]
		try:
			score=databases[dbname]
		except:
			databases[dbname]=0
		for s in _SETS:
			for e in _EXTENSIONS:
				tail='_%s.%s'%(s,e)
				if name[-9:]==tail:
					databases[dbname]+=1
	for k in databases.keys():
		if databases[k]<len(_SETS)*len(_EXTENSIONS):
			del databases[k]
			continue
		print 'Found database "%s"'%os.path.basename(dbname)
	groups={}
	ids={}
	for k in databases.keys():
		process(k,datapath,groups,ids)
	return (groups,ids)

def outputMesh(o,m,t,step=1):
	faces=[]
	for f in m.faces:
		faces.append([])
		for v1 in f.verts:
			index=None
			for i,v2 in enumerate(m.verts):
				if v1.co.x==v2.co.x and v1.co.y==v2.co.y and v1.co.z==v2.co.z \
				and v1.no.x==v2.no.x and v1.no.y==v2.no.y and v1.no.z==v2.no.z:
					index=i
					break
			if index==None:
				raise Exception('Cannot find vertex!')
			faces[-1].append(index)

	indent='	'*t
	print >>o, indent+'"vertices":['
	for v in (x.co for x in m.verts):
		print >>o, indent+'	(%f,%f,%f),'%(v.x,v.y,v.z)
	print >>o, indent+'],'
	print >>o, indent+'"normals":['
	for v in (x.no for x in m.verts):
		print >>o, indent+'	(%f,%f,%f),'%(v.x,v.y,v.z)
	print >>o, indent+'],'
	print >>o, indent+'"faces":['
	for f in faces:
		print >>o, indent+'	%s,'%repr(f[::step])
	print >>o, indent+'],'

zzMesh=None

def processBuilding(building, groups, ids,path):
	#
	global zzMesh
	if zzMesh==None:
		zzMesh=bpy.data.meshes.new('zz')
	# 
	o=open(os.sep.join([path,'building_%i.bld'%building]),'w')
	# Compute building position!
	avgX=0.0
	avgY=0.0
	minX=9999.0
	maxX=-9999.0
	minY=9999.0
	maxY=-9999.0
	count=0
	for id in groups[building]:
		for (x,y,z) in ids[id][Attributes.GEOM]:
			avgX+=x
			avgY+=y
			count+=1
			if x>maxX: maxX=x
			if x<minX: minX=x
			if y>maxY: maxY=y
			if y<minY: minY=y
	icountf=1.0/float(count)
	avgX*=icountf
	avgY*=icountf
	print >>o, '{'
	print >>o, '	"origin":(%f,%f),'%(avgX,avgY)
	print >>o, '	"bounds":((%f,%f),(%f,%f)),'%(minX,minY,maxX,maxY)
	print >>o, '	"parts":{'
	# Convert geometry to meters
	angleToDistance=111226.3
	yFactor=math.cos(avgY*0.017453292519943295769236907684886)
	baseindex=0
	for id in groups[building]:
		print >>o, '		%i:{'%id
		# First, base geometry
		l=len(ids[id][Attributes.GEOM])
		for i in xrange(0,l):
			(x,y,z)=ids[id][Attributes.GEOM][i]
			ids[id][Attributes.GEOM][i]=[(x-avgX)*angleToDistance*yFactor, (y-avgY)*angleToDistance, z]
		baseindex+=1
		basegeom=ids[id][Attributes.GEOM]
		faces=BPyMesh.ngon(basegeom[::-1], range(0,len(basegeom)))
		zzMesh.verts.delete(zzMesh.verts)
		zzMesh.verts.extend(basegeom[::-1])
		zzMesh.faces.extend(faces)
		zzMesh.calcNormals()
		print >>o, '			"base":{'
		outputMesh(o,zzMesh,4)
		print >>o, '			},'
		# Then, roofs
		topgeom=[]
		if ids[id].has_key(Attributes.ROOF):
			print >>o, '			"roofs":['
			l=len(ids[id][Attributes.ROOF])
			for i in xrange(0,l):
				ll=len(ids[id][Attributes.ROOF][i])
				for j in xrange(0,ll):
					(x,y,z)=ids[id][Attributes.ROOF][i][j]
					ids[id][Attributes.ROOF][i][j]=[(x-avgX)*angleToDistance*yFactor, (y-avgY)*angleToDistance, z]
				rgeom=ids[id][Attributes.ROOF][i]
				topgeom.extend(rgeom)
				faces=BPyMesh.ngon(rgeom, range(0,len(rgeom)))
				zzMesh.verts.delete(zzMesh.verts)
				zzMesh.verts.extend(rgeom)
				zzMesh.faces.extend(faces)
				zzMesh.calcNormals()
				#
				print >>o, '				{'
				outputMesh(o,zzMesh,5)
				print >>o, '				},'
			print >>o, '			],'
		# Last, pediments
		if ids[id].has_key(Attributes.PEDIMENT):
			print >>o, '			"pediments":['
			l=len(ids[id][Attributes.PEDIMENT])
			for i in xrange(0,l):
				ll=len(ids[id][Attributes.PEDIMENT][i])
				for j in xrange(0,ll):
					(x,y,z)=ids[id][Attributes.PEDIMENT][i][j]
					ids[id][Attributes.PEDIMENT][i][j]=[(x-avgX)*angleToDistance*yFactor, (y-avgY)*angleToDistance, z]
				pgeom=ids[id][Attributes.PEDIMENT][i]
				topgeom.extend(pgeom)
				faces=BPyMesh.ngon(pgeom, range(0,len(pgeom)))
				zzMesh.verts.delete(zzMesh.verts)
				zzMesh.verts.extend(pgeom)
				zzMesh.faces.extend(faces)
				zzMesh.calcNormals()
				#
				print >>o, '				{'
				outputMesh(o,zzMesh,5)
				print >>o, '				},'
			print >>o, '			],'
		# And while we're here, construct some walls...
		upper=[]
		for (x,y,z) in basegeom:
			minz=99999.0
			for (rx,ry,rz) in topgeom:
				(dx,dy)=(x-rx,y-ry)
				if dx<0: dx=-dx
				if dy<0: dy=-dy
				if dx<.001 and dy<.001:
					if rz<minz: minz=rz
			if minz<99999.0:
				upper.append([x,y,minz])
		L=len(upper)
		if L==len(basegeom):
			zzMesh.verts.delete(zzMesh.verts)
			wallgeom=upper+basegeom
			zzMesh.verts.extend(wallgeom)
			for f in xrange(0,L-1):
				zzMesh.faces.extend([[f,f+L,f+L+1], [f,f+L+1,f+1]])
			zzMesh.calcNormals()
			print >>o, '			"wall":{'
			outputMesh(o,zzMesh,4)
			print >>o, '			}'
		else:
			print "Cannot generate wall for id=%i (building %i) %i!=%i"%(id,building,L,len(basegeom))
			pass
		#
		print >>o, '		},'
		del ids[id]
	print >>o, '	},'
	print >>o, '}'
	o.close()

if __name__=='__main__':
	try:
		import psyco
		psyco.full()
	except ImportError:
		print "Psyco not available!"
		pass
	import win32api,win32process,win32con,time
	pid = win32api.GetCurrentProcessId()
	handle = win32api.OpenProcess(win32con.PROCESS_ALL_ACCESS, True, pid)
	win32process.SetPriorityClass(handle, win32process.BELOW_NORMAL_PRIORITY_CLASS)
	#
	try:
		(a,b)=(sys.argv[5],sys.argv[6])
	except:
		print "Usage: shp2py input_path output_path"
		sys.exit(1)
	#
	try:
		os.makedirs(sys.argv[6])
	except:
		pass
	(groups,ids)=load_everything(sys.argv[5])
	# for resuming
	import struct
	try:
		o=open('bldresumeat.bin','r+')
	except:
		o=open('bldresumeat.bin','w+')
	resumeat=o.read()
	if len(resumeat)<4:
		resumeat=0
	else:
		resumeat=struct.unpack('i',resumeat[0:4])[0]
		print "resuming at position %i"%(resumeat+1)
		
	# Process coordinates
	buildings=groups.keys()
	buildings.sort()
	total=len(buildings)
	start=None
	starti=0
	#
	i=0
	l=len(buildings)
	
	#processBuilding(-456365483, groups, ids,sys.argv[6])
	#sys.exit(0)
	
	#
	while l>0:
		building=buildings.pop(0)
		l-=1
		i+=1
		if not i>resumeat:
			# remove items
			for id in groups[building]:
				del ids[id]
			continue
		if start==None:
			start=time.time()
			starti=i
			total=len(buildings)
		percent=((i-starti)*100)/total
		try:
			now=time.time()
			deltaT=now-start
			H=0
			M=0
			S=1+int((100.0-float(percent))*(deltaT/float(percent)))
			if S>3600:
				H=S/3600
				S=S%3600
			if S>60:
				M=S/60
				S=S%60
			HMS='%02is'%S
			if M>0 or H>0:
				HMS='%02im%s'%(M,HMS)
			if H>0:
				HMS='%02ih%s'%(H,HMS)
			HMS='- remaining: %s'%HMS
		except:
			HMS=''
		print "\rProcessing building #%i (%i%% %s)"%(i,percent,HMS),
		processBuilding(building, groups, ids,sys.argv[6])
		if (i%200)==0:
			o.seek(0)
			o.write(struct.pack('i',i))
			o.flush()
	o.close()
	print "ok!"
