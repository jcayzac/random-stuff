import compiler
import md5, imp, xml.sax.saxutils


class Template:
	def closeBuffer(self,tb):
		# we want all the variable names inside tb
		inside=False
		name=''
		names=[]
		for c in tb:
			if inside:
				if not c.isalnum():
					names.append(name)
					name=''
					inside=False
				else:
					name+=c
		if not inside:
			if c=='$':
				inside=True
		tb=tb.replace('"""','&#x0022;&#x0022;&#x0022;')
		tb=tb.replace('##','&#x0023;&#x0023;')
		if len(names)==0:
			tb='"""%s"""'%tb
		else:
			mappings=[]
			for n in names:
				tb=tb.replace('$%s'%n, '%%(%s)s'%n)
				mappings.append("'%s':xml.sax.saxutils.escape(%s)"%(n,n))
			tb='"""%s""" %% {%s}' % (tb, ','.join(mappings))
		return tb
	def compile(self):
		# is it in memory?
		if sys.modules.has_key(self.modname):
			return
	def escape(self,text):
		return text
	def unescape(self,text):
		return text

	def __init__(self, src, charset='utf-8'):
		self.modname = 'template_%s'%md5.new(src).hexdigest()


