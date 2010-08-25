#!/bin/perl
require 5.8.0;

# THIS FILE IS DEPRECATED!
#
# The TestReport.pm Perl module is meant to get rewritten in Python
# using the available Python XML handling facilities, and won't be
# maintained in the future...
#
###################################################################

use TestsReport;
use strict;
use Env qw($platformscope);
use Env qw($reference_file);
use Env qw($testframework_root);
use Env qw($test_report);
use File::Copy;

# create the generator
my $generator = new TestsReport();
$generator->setTitle($platformscope.' report');

# tell him which stylesheet to use for XSLT transform
$generator->setXSL($testframework_root.'\stylesheets\TestsReport.xsl',
					$testframework_root.'\stylesheets\TestsReport.xsl') or die "Can't find XSL!\n";

# Put some related links
$generator->addURL('Previous page in browser history', 'javascript:history.back()');
$generator->addURL('Autobuilds', 'http://192.168.0.10/autobuilds/index.cgi');
$generator->addURL('XML File', 'results.xml');
$generator->addURL('Standalone Stylesheet', 'standalone/TestsReport.xsl');
$generator->setSucessThreshold(3);

# build the XML and the static HTML pages
$generator->generate($test_report, $reference_file)	or die "Cannot generate report:$!\n";
