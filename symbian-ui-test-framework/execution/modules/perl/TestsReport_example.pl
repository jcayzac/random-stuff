#!/bin/perl
require 5.8.0;

# THIS FILE IS DEPRECATED!
#
# The TestReport.pm Perl module is meant to get rewritten in Python
# using the available Python XML handling facilities, and won't be
# maintained in the future...
#
###################################################################

# Example use of TestsReport.pm
# 
# Requirements:
#
# - The following .exe's must be in the path:
#		- Xalan.exe (>=1.4.0),
#		- CompareBitmap.exe,
#		- bmconv.exe (if there are some .MBM files  in the scanned directory),
#		- bmp2png (ditto),
# - The directory where TestsReport.pm and TestUtils.pm lie must be in Perl's include path.
#   e.g.:
#      perl -Is:\tools\test\common thisfile.pl
# - TestsReport.pm depends on TestUtils.pm, which itself depends on ReTry.pm. Ensure all
#   modules are present.

use TestsReport;
use strict;

my $xtra = "
<toto>
	<name>Titi</name>
	<description>Tutu</description>
</toto>
";

# create the generator
my $generator = new TestsReport();

# tell him which stylesheet to use for XSLT transform
$generator->setXSL('D:\SDKs\S\monaco3\tools\test\common\TestsReport.xsl');

# add some extra XML so you can use it with your own custom .XSL
$generator->addXML($xtra);

# Name the report
$generator->setTitle('My mighty report');

# Put some related links
$generator->addURL('Previous page in browser history', 'javascript:history.back()');
$generator->addURL('Autobuilds', 'http://192.168.0.10/autobuilds/index.cgi');
$generator->addURL('Google', 'http://www.google.com/');

# Set how many tests we want to be displayed each page
$generator->setResultsPerPage(50);

# generate the report in D:\synchro\bla, using D:\synchro\bla\res as actual results,
# and D:\synchro\bla.zip as reference.
# Before the call, D:\synchro\bla must look like this:
#	D:\
#		synchro\
#			bla\
#				res\
#					<everything your tests produced>
#
# After the call, it will look like this:
#	D:\
#		synchro\
#			bla\
#				res\
#					<everything your tests produced>
#					<masks and other support files copied from reference>
#				ref\
#					<reference zipfile unzipped>
#				diff\
#					<difference files>
#				results.xml (raw results in XML format)
#				<the XSL stylesheet>.xsl
#				reference.zip (new reference zipfile)
#				index.html (generated HTML page for results)
#				page_NN.html (additional pages)

$generator->generate('D:\synchro\bla', 'D:\synchro\bla.zip') or die "Can't generate report!\n";



