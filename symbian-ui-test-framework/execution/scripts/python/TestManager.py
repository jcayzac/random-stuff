#
# TestManager.py
#
# This script is responsible for running one test suite.
# Syntax is:
#    TestManager.py <test_suite_description_file.txt>
# or:
#    TestManager.py -changelog
# ...for the changes history.
#
# The following environment must be set prior to calling the script:
#
#   epocdrive
#   epocroot
#   testtarget
#   platformscope
#	perl_path
#   
#
import sys
import os
import time
import re
import string
import traceback
import shutil
import win32process
import win32gui
import win32con
import win32api


#################
### Functions ###
#################

# Print a string and exit
def die(string):
	"""Exits the program with an error code, and writes the corresponding string to stderr"""
	traceback.print_exc(sys.exc_info()[2])
	sys.exit("TestManager: "+string)

# Print an error message
def error(string):
	traceback.print_exc(sys.exc_info()[2])
	print "ERROR: "+string

# Print a warning message
def warning(string):
	traceback.print_exc(sys.exc_info()[2])
	print "WARNING: "+string

# Returns a list of all possible entrypoints in the specified testsuite
def listPossibleEntryPoints(testsuite):
	"""Returns a list of all possible entrypoints"""
	# test if testsuite file exists
	try:
		info = os.stat(testsuite.fileName)
	except:
		die("Cannot find any testsuite at '"+testsuite.fileName+"'...")

	# open the testsuite file
	try:
		f = open(testsuite.fileName, 'r')
		src = f.read().expandtabs(1).replace("\n","")
		f.close()
	except:
		die("Cannot read testsuite at '"+testsuite.fileName+"'...")
	
	# list every functions (entrypoints) in the testsuite
	try:
		prog = re.compile(simkin_method_re, re.S)
		list = prog.findall(src)
		catch=list[0] # will raise an exception if list is null or empty
		return list
	except:
		sys.stderr.write("No entrypoint defined. Giving up...\n")
		sys.exit(0)

# Splits a line containing a test and options separated by :
# Return test name in a variable and options in a map.
def splitParameters(line):
	"""Splits a line containing a test and options separated by :, return test name in a variable and options in a map."""
	options = line.split(":")
	test=options[0].replace("\n","")
	options=options[1:]
	map = {}
	for opt in options:
		string.strip(opt)
		(key,val) = opt.split('=')
		map[key.strip()]=val.strip().replace("\n","")
	return test, map

# Get the test case list to execute from the testsuite file.
def readTestSpecs(specFileName):
	"""Reads options and test cases from the specfile"""
	# BROKEN - TO BE REMOVED
	# If no testsuite filename is passed we use the 'platformscope' variable to get the testsuite name.
	#	if (specFileName == None):
	#		specFileName = "\\tools\\test\\suites\\"+platformscope+'.txt'
	
	# Check if testsuite file really exists.
	try:
		info = os.stat(specFileName)
	except:
		sys.stderr.write("'"+specFileName+"' does not exist. Giving up...\n")
		sys.exit(0)
	
	# Create the testsuite object
	ts = TestSuite()
	try:
		f = open(specFileName, 'r')
		ln = 1
		# pour chaque ligne de la testsuite
		for line in f.readlines():
			try:
				# list all ':' separated elements. The first one is the name of the test.
				# The others parts are options
				test, optionsMap = splitParameters(line)
				# A line starting with a ':' specifies general parameters for the whole bunch of test cases
				if (test==''):
					for opt in optionsMap.keys():
						if (opt=='file'):        ts.fileName = optionsMap[opt]
						elif (opt=='coverage'):  ts.coverageAnalysis = string.lower(optionsMap[opt])
						elif (opt=='timeout'):   ts.timeout = int(optionsMap[opt])
						elif (opt=='verbose'):   ts.verbose = string.lower(optionsMap[opt])
						elif (opt=='reference'): ts.reference = optionsMap[opt]
						elif (opt=='logpath'):   ts.logPath = optionsMap[opt]
						else:
							sys.stderr.write("Warning: illegal option '"+opt+"' in testsuite spec")
				else:
					thisTest = TestSpec()
					thisTest.entrypoint = test
					thisTest.timeout = ts.timeout #FIXME: defaults if global params ain't on 1st line! :(
					for opt in optionsMap.keys():
						if (opt=='continue'):
							if (optionsMap[opt]=='no'): thisTest.continueOnFreeze=False
						elif (opt=='retries'):   thisTest.retries = int(optionsMap[opt])
						elif (opt=='timeout'):   thisTest.timeout = int(optionsMap[opt])
					ts.testSpecs[test]=thisTest
			except:
				die("error")
			++ln
		f.close()
	except:
		die("Cannot read file '"+specFileName+"'")
	return ts

def which(file):
	"""Finds a file if it's in the current PATH environment variable."""
	for path in os.environ['PATH'].split(';'):
		if (os.path.isfile(path+"\\"+file)):
			return (path+"\\"+file)
	return None

def setEntrypoint(ep):
	"""Sets which test case ASE should start upon next boot."""
	try:
		os.remove(ini_file)
	except:
		pass
	ini = file(ini_file, 'w')
	# Suite_file path must start form emulator c drive. c:\mydocs\ase.ini
	ini.write("bootfile = "+suite_file_emulator_path+"\nentrypoint = "+ep+"\n")
	ini.close()

def finished():
	"""Tells if the current emulator session is done or not."""
	try:
		if (os.path.isfile(exitfile)):
			return True
		else:
			return False
	except:
		return False

def closewindow(name):
	"""Closes a win32 window"""
	win = win32gui.FindWindow(None, name)
	win32gui.SendMessage(win, 16)
	
def printVersion():
	"""Prints the script's version"""
	print """
	TestManager v2."""+version+""":
	======================="""

def printEndTest():
	"""Print some dummy string at the end of the tests"""
	print """

	End of TestManager v2."""+version+"""
	======================="""

def tryAndPass(exp):
	"""Traps any exception when evaluating the specified Python expression"""
	try:
		eval(exp)
	except:
		pass

###############
### Classes ###
###############

class TestSpec:
	"""Contains info about each test spec"""
	def __init__(self):
		# The test ASE will run upon next boot
		self.entrypoint       = None
		# Should we continue if he emulator freeze, or stop everything?
		self.continueOnFreeze = True
		# How long should we wait for test's termination?
		self.timeout          = 600
		# How many times should we retry the current test?
		self.retries          = 0
	def __str__(self):
		if (self.entrypoint):
			return "['entrypoint': '"+str(self.entrypoint)\
			+"', 'continue': '"+str(self.continueOnFreeze)\
			+"', 'timeout': '"+str(self.timeout)\
			+"', 'retries': '"+str(self.retries)\
			+"' ]"
		else:
			return "[]"

class TestSuite:
	"""Define a test suite"""
	def __init__(self):
		self.testSpecs        = {}
		self.fileName         = None
		self.reference        = None
		self.timeout          = 600
		self.coverageAnalysis = 'no'
		self.verbose          = 'no'
		self.logPath          = epocdrive+epocroot+"epoc32\\"+testtarget+"\\c\\testlogs"
	def __str__(self):
		res = "["
		if (self.fileName):
			res += "'filename': '"+self.fileName+"', "
		if (self.reference):
			res += "'reference': '"+self.reference+"', "
		res+="'verbose': '"+self.verbose+"', "
		res+="'coverageAnalysis': '"+self.coverageAnalysis\
		+"', 'testSpecs': ("
		for ts in self.testSpecs:
			res+=str(ts)+", "
		res+="'') ]"
		return res

############################################################################################
########################################## INIT ############################################
############################################################################################

version = "20040913"
history = """
20040913: Added an optional <testspec> parameter. Added -changelog
20040903: First release.
"""

simkin_method_re = '([a-zA-Z0-9_]+)\\s*\\[\\s*\\([^\\\\)]*\\)\\s*\\{[^\\]]*\\]'
specFileName = None

# check that environment variables needed are set. Quit if not.
try:
	epocdrive  = os.environ['EPOCDRIVE']
	epocroot   = os.environ['EPOCROOT']
	testtarget = os.environ['TESTTARGET']
	platformscope = os.environ['PLATFORMSCOPE']
	perlpath   = os.environ['PERL_PATH']
	testframework_root = os.environ['TESTFRAMEWORK_ROOT']
except:
	die("The environment variables 'EPOCDRIVE', 'EPOCROOT', 'TESTTARGET', 'PLATFORMSCOPE', 'PERL_PATH', 'TESTFRAMEWORK_ROOT' and 'REPOSITORYPATH' must be defined!")

printVersion()

# read parameters
if len(sys.argv) > 1:
	arg = string.lower(sys.argv[1])
	if (arg == "-changelog"):
		print history
	else:
		specFileName = arg

os.path.exists(specFileName) or die("Cannot read test suite \""+specFileName+"\"!\n");

############################################################################################
########################################## MAIN ############################################
############################################################################################

################### BUILDS TEST SUITE ###############
# read specs
testsuite = readTestSpecs(specFileName)

# get possible entrypoints
entrypoints = listPossibleEntryPoints(testsuite)

# remove spec'ed tests not in suite
total=len(testsuite.testSpecs)
for ep in testsuite.testSpecs.keys():
	if not ep in entrypoints:
		del testsuite.testSpecs[ep]
net=len(testsuite.testSpecs)

# override some settings with environment
cov_ov = ""
try:
	testsuite.coverageAnalysis = string.lower(os.environ['USE_CPP_CODE_COVERAGE'])
	cov_ov = " (overridden by environment)"
except:
	pass

# suite folder from the test manager point of view
suite_folder 		 = epocdrive+epocroot+"epoc32\\"+testtarget+"\\c\\mydocs"
# suite file form the test manager point of view
suite_file   		 = suite_folder+"\\boot.ase"
# suite file form the emulator point of view
suite_file_emulator_path = "c:\\mydocs\\boot.ase"

# init folder form the test manager point of view
ini_folder   = epocdrive+epocroot+"epoc32\\"+testtarget+"\\c\\system\\data"
# init file form the test manager point of view
ini_file     = ini_folder+"\\ase.ini"

exe_path = epocdrive+epocroot+"epoc32\\release\\"+testtarget+"\\udeb\\epoc.exe"
cmd_line = [ "epoc" ]
if (testsuite.coverageAnalysis == 'yes'):
	exe_path = which('perl.exe')
	cmd_line = ["perl", testframework_root+"\\CPPCoverage\\cover.pl" ]

exitfile = suite_folder+"\\exitfile"


# Dump environment
print """
Environment:
-----------
   + SymbianOS root:       '"""+epocdrive+epocroot+"""'
   + Executable path:      '"""+exe_path+"""'
   + Command line:         """+str(cmd_line)+"""
   + Suite folder:         '"""+suite_folder+"""'
   + Platform scope:       '"""+os.environ['PLATFORMSCOPE']+"""'
   + ase.ini:              '"""+ini_file+"""'
   + Test suite:           '"""+testsuite.fileName+"""'
      + Coverage analysis: """+testsuite.coverageAnalysis+cov_ov+"""
      + Default timeout:   """+str(testsuite.timeout)+"""s
      + Verbose:           """+testsuite.verbose+"""
   + Tests in suite:       """+str(net)+" ("+str(total-net)+""" ignored)"""
# tests in suite
if (testsuite.verbose=='yes'):
	keys = testsuite.testSpecs.keys()
	keys.sort()
	for ep in keys:
		print "      + '"+ep+"'"
		cof=''
		if (testsuite.testSpecs[ep].continueOnFreeze==False):
			cof="         + Continue on freeze:  no\n"
			print cof
		to=''
		if (testsuite.testSpecs[ep].timeout != testsuite.timeout):
			to="         + Timeout:             "+str(testsuite.testSpecs[ep].timeout)+"s\n"
			print to
		rts=''
		if (testsuite.testSpecs[ep].retries != 0):
			rts="         + Retries:             "+str(testsuite.testSpecs[ep].retries)+"\n"
			print rts

# create suite_folder if it doesn't exist
tryAndPass("os.makedirs(suite_folder)")

# create ini_folder if it doesn't exist
tryAndPass("os.makedirs(ini_folder)")

# copy testsuite
try:
	print "copy '"+testsuite.fileName+"' -> '"+suite_file+"'\n"
	if os.path.exists(suite_file):
		win32api.SetFileAttributes(suite_file, win32con.FILE_ATTRIBUTE_NORMAL)
		os.remove(suite_file);
	shutil.copy(testsuite.fileName, suite_file)
except:
	die("Cannot copy file '"+testsuite.fileName+"' to '"+suite_file+"'")

# loop thru all speced tests
keys = testsuite.testSpecs.keys()
keys.sort()
for test in keys:
	print "+ Running test '"+test+"'..."
	setEntrypoint(test)

	while(os.path.isfile(exitfile)):
		time.sleep(1)
		try:
			os.remove(exitfile)
		except:
			pass

	retries = testsuite.testSpecs[test].retries
	while(retries>=0):
		retries = retries-1
		try:
			ph, th, pid, tid = win32process.CreateProcess(exe_path, string.join(cmd_line, ' '), None, None, 1, win32process.IDLE_PRIORITY_CLASS, None, "s:\\", win32process.STARTUPINFO())
			timeout = testsuite.testSpecs[test].timeout
			print "Launched! pid="+str(pid)+", timeout="+str(timeout)+"s"
			exitcode=259
			while((os.path.exists(exitfile)!=True) and (timeout>0) and (exitcode == 259)):
				time.sleep(1)
				exitcode = win32process.GetExitCodeProcess(ph)
				timeout = timeout - 1
			if (exitcode == 259):
				if (testsuite.coverageAnalysis == 'yes'):
					# Close the emulator
					print "Closing the SymbianOS emulator window...\n"
					try:
						closewindow("Symbian OS Emulator - "+testtarget+" udeb")
					except:
						pass
					# Wait for cover.pl to finish
					timeout_seconds = 60
					while((exitcode == 259) or (timeout_seconds > 0)):
						time.sleep(10)
						timeout_seconds = timeout_seconds - 10
						exitcode = win32process.GetExitCodeProcess(ph)
					if (timeout_seconds <= 0):
						print "Processus frozen: killing profile.exe...\n"
						os.system('killprocess profile.exe')
				else:
					win32process.TerminateProcess(ph, 0)
		except:
			error("ERROR: while executing test '"+test+"'")
			if (testsuite.testSpecs[test].continueOnFreeze==False):
				die("Cannot continue since test '"+test+"' failed!")

printEndTest()
