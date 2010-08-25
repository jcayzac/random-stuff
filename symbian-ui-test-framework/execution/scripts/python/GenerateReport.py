#
# GenerateReport.py
#
# This script is responsible for creating the XML report out of the
# tests results.
#
# The following environment must be set prior to calling the script:
#   testframework_root
#	raw_results
#	test_report
# ...and the called BuildTestReport.pl called therin needs the following:
#   platformscope
#   reference_file
#
# This script still makes use of an external Perl module we should
# get rid of. Python has a beautiful API for handling XML, so the
# best advice I can give is to use it...
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


#################################
### Somewhat useful functions ###
#################################


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

def writeHTAccess(path):
	"""Write a .htaccess file which overrides Apache config and makes the current directory browseable"""
	buffer="""
Options MultiViews Indexes Includes FollowSymLinks
DirectoryIndex Results.xml results.xml index.xml index.html
	"""
	try:
		f=open(path+"\\.htaccess", "w")
		f.write(buffer)
		f.close()
	except Exception, inst:
		print "Error while writing \""+path+"\\.htaccess\"!"
		print type(inst)
		print inst.args
		print inst
		pass

############
### Init ###
############

print "GenerateReport.py"
print "================="

# check that environment variables needed are set. Quit if not.
try:
	testframework_root = os.environ['TESTFRAMEWORK_ROOT']
except:
	die("The environment variable 'TESTFRAMEWORK_ROOT' must be defined!")
try:
	raw_results = os.environ['RAW_RESULTS']
except:
	die("The environment variable 'RAW_RESULTS' must be defined!")
try:
	test_report = os.environ['TEST_REPORT']
except:
	die("The environment variable 'TEST_REPORT' must be defined!")

print "Environment summary"
print "-------------------"
print "testframework_root = "+testframework_root
print "raw_results        = "+raw_results
print "test_report        = "+test_report
print " "

##########################
### Results management ###
##########################


# create the result dir if it doesn't exist
try:
	print "Creating directory \""+test_report+"\"..."
	os.system("mkdir \""+test_report+"\"")
except:
	pass

# copy all results in the result folder
if (os.path.exists(raw_results)):
	# copy raw_results/*.* to result_dir/res recursively
	try:
		print "Copying raw results to "+test_report
		shutil.copytree(raw_results, test_report);
	except:
		print "An error occured while copying \""+raw_results+"\" to \""+test_report+"\"."
		pass
	pass

#############################################################
### Additional information generated from the environment ###
#############################################################

print "Creating Info.txt"
# it all goes therein
dynamic_info=""

# collect the info
try:
	info=os.environ['PLATFORMSCOPE'];
	dynamic_info+="[Title]:"+info+"\n"
except:
	pass

try:
	info=os.environ['PROJECT'];
	dynamic_info+="[Project]:"+info+"\n"
except:
	pass

try:
	info=os.environ['PLATFORM'];
	dynamic_info+="[Platform]:"+info+"\n"
except:
	pass

try:
	info=os.environ['BUILDSCOPE'];
	dynamic_info+="[Build scope]:"+info+"\n"
except:
	pass

try:
	info=os.environ['PLATFORMVERSION'];
	dynamic_info+="[Platform version]:"+info+"\n"
except:
	pass

try:
	info=os.environ['CURRENTDATE'];
	p=re.compile(r"(.{4})(.{2})(.{2})")
	m=p.match(info)
	year	= m.group(1)
	month	= m.group(2)
	day		= m.group(3)
	dynamic_info+="[Date]:"+day+"/"+month+"/"+year+"\n"
except:
	pass

# append or create the info file (right into the results directory)
try:
	f=open(test_report+"\\Info.txt", 'a')
	f.write(dynamic_info)
	f.close()
except:
	print "Runtime error while writing to "+test_report+"\\Info.txt"
	pass


#########################
### Standalone report ###
#########################


# Copy the standalone stylesheet
try:
	print "Copying standalone stylesheet"
	shutil.copytree(testframework_root+"\\stylesheets\\standalone", test_report+"\\standalone");
except:
	print "Runtime error while creating "+test_report+"\\standalone"
	pass


####################
### The real one ###
####################


# Call the perl scripty
try:
	print "Generating report"
	cmdline="perl.exe -Is:"+testframework_root+"\\execution\\modules\\perl -Is:"+testframework_root+"\\execution\\scripts\\perl "+testframework_root+"\\execution\\scripts\\perl\\BuildTestsReport.pl"
	print "Executing \""+cmdline+"\"..."
	os.system(cmdline)
except Exception, inst:
	print "Runtime error while creating report:"
	print type(inst)
	print inst.args
	print inst
	pass

# modify Apache settings for the test report directory
print "Modying default Apache policy..."
writeHTAccess(test_report)

print "Report generated."
