import os,os.path,sys,math,struct,time

def loadObj(f):
	i=open(f)
	obj=eval(i.read())
	i.close()
	return obj
	
def snapVertexCm(v):
	return (int(v[0]*100.0),int(v[1]*100.0),int(v[2]*100.0))

def processPart(p,vertices,normals,faces):
	# first, register vertices
	nv=len(vertices[0])
	for v in p['vertices']:
		v=snapVertexCm(v)
		try:
			test=vertices[0][v]
		except:
			vertices[0][v]=nv
			vertices[1][nv]=v
			nv+=1
	# then, register normals
	nn=len(normals[0])
	nf=len(faces)
	for (a,b,c) in p['faces']:
		# find real indices
		aa=p['vertices'][a]
		bb=p['vertices'][b]
		cc=p['vertices'][c]
		a=vertices[0][snapVertexCm(p['vertices'][a])]
		b=vertices[0][snapVertexCm(p['vertices'][b])]
		c=vertices[0][snapVertexCm(p['vertices'][c])]
		# compute normal
		ab=[bb[0]-aa[0],bb[1]-aa[1],bb[2]-aa[2]]
		ac=[cc[0]-aa[0],cc[1]-aa[1],cc[2]-aa[2]]
		n=[ab[1]*ac[2] - ab[2]*ac[1], ab[2]*ac[0] - ab[0]*ac[2], ab[0]*ac[1] - ab[1]*ac[0]]
		#n=[-n[0],-n[1],-n[2]]
		nLen=math.sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2])
		if nLen>0.001:
			nLen=1.0/nLen
			n=[n[0]*nLen,n[1]*nLen,n[2]*nLen]
		else:
			n=[0.0,0.0,1.0]
		n=snapVertexCm(n)
		try:
			test=normals[0][n]
		except:
			normals[0][n]=nn
			normals[1][nn]=n
			nn+=1
		# face
		faces.append((a,b,c,normals[0][n]))
		nf+=1

def buildingUuidGenerator():
	i=0
	while True:
		yield i
		i+=1
buildingUuid=buildingUuidGenerator()

def processObj(o):
	vertices=[{},{}]
	normals=[{},{}]
	faces=[]
	for k,part in o['parts'].items():
		if part.has_key('base'):
			processPart(part['base'],vertices,normals,faces)
		if part.has_key('roofs'):
			for r in part['roofs']:
				processPart(r,vertices,normals,faces)
		if part.has_key('pediments'):
			for p in part['pediments']:
				processPart(p,vertices,normals,faces)
		if part.has_key('wall'):
			processPart(part['wall'],vertices,normals,faces)
	# output collada file
	id=buildingUuid.next()
	outputDAEpath="%i.dae"%id
	dae=open(outputDAEpath,'w')
	now=time.strftime('%Y-%m-%dT%H:%M:%SZ')
	print >>dae, """<?xml version="1.0" encoding="UTF-8"?>
<COLLADA xmlns="http://www.collada.org/2005/11/COLLADASchema" version="1.4.1">
<asset>
<created>%s</created>
<modified>%s</modified>
<unit name="meters" meter="1.0"/>
<up_axis>Z_UP</up_axis>
</asset>
<library_geometries>
<geometry id="mesh1-geometry" name="mesh1-geometry">
<mesh>""" % (now,now)
	xyz=[(float(v[0])/100.0,float(v[1])/100.0,float(v[2])/100.0) for v in vertices[1].values()]
	normals=[(float(v[0])/100.0,float(v[1])/100.0,float(v[2])/100.0) for v in normals[1].values()]
	print >>dae, """<source id="mesh1-geometry-position">
<float_array id="mesh1-geometry-position-array" count="%i">%s</float_array>
<technique_common>
<accessor source="#mesh1-geometry-position-array" count="%i" stride="3">
<param name="X" type="float"/>
<param name="Y" type="float"/>
<param name="Z" type="float"/>
</accessor>
</technique_common>
</source>""" % (len(xyz)*3,' '.join(['%.2f %.2f %.2f'%(v[0],v[1],v[2]) for v in xyz]),len(xyz))
	print >>dae, """<source id="mesh1-geometry-normal">
<float_array id="mesh1-geometry-normal-array" count="%i">%s</float_array>
<technique_common>
<accessor source="#mesh1-geometry-normal-array" count="%i" stride="3">
<param name="X" type="float"/>
<param name="Y" type="float"/>
<param name="Z" type="float"/>
</accessor>
</technique_common>
</source>""" % (len(normals)*3,' '.join(['%.2f %.2f %.2f'%(v[0],v[1],v[2]) for v in normals]),len(normals))
	print >>dae, """<vertices id="mesh1-geometry-vertex">
<input semantic="POSITION" source="#mesh1-geometry-position"/>
</vertices>
<triangles count="%i">
<input semantic="VERTEX" source="#mesh1-geometry-vertex" offset="0"/>
<input semantic="NORMAL" source="#mesh1-geometry-normal" offset="1" set="0"/>
<p>%s</p>
</triangles>
</mesh>
</geometry>
</library_geometries>
<library_visual_scenes>
<visual_scene id="SketchUpScene" name="SketchUpScene">
<node id="Model" name="Model">
<node id="mesh1" name="mesh1">
<instance_geometry url="#mesh1-geometry"/>
</node>
</node>
</visual_scene>
</library_visual_scenes>
<scene>
<instance_visual_scene url="#SketchUpScene"/>
</scene>
</COLLADA>""" % (len(faces), ' '.join(['%i %i %i %i %i %i'%(i[0],i[3],i[2],i[3],i[1],i[3]) for i in faces]))
	dae.close()
	# ouput KML to stdout
	print """<Placemark><Model><altitudeMode>relativeToGround</altitudeMode><Location>
<longitude>%f</longitude><latitude>%f</latitude>
</Location>
<Link><href>%s</href></Link>
</Model>
</Placemark>
""" % (o['origin'][0],o['origin'][1],outputDAEpath)


def getAllFiles(path):
	for root, dirs, files in os.walk(path):
		for name in files:
			yield os.path.join(root, name)

def processPath(path):
	if os.path.isfile(path):
		files=[path]
	else:
		files=getAllFiles(path)
	fi=0
	for f in files:
		fi+=1
		o=loadObj(f)
		print >>sys.stderr, "Adding '%s'... (%i)"%(f,fi)
		processObj(o)

if __name__=='__main__':
	print """<?xml version='1.0' encoding='UTF-8'?>
<kml xmlns='http://earth.google.com/kml/2.1'>
<Folder>
"""
	processPath(sys.argv[-1])
	print """</Folder>
</kml>
"""
