#!/bin/perl
require 5.8.0;

#
# Code coverage analysis final steps: report creation
# (to be done after the tests are done)
#

use Encode;
use Env qw($EPOCDRIVE $EPOCROOT $platformscope $repositorypath $tests_root $testframework_root $test_report); 
use Cwd;
use TestUtils;

my $SPECS	= $tests_root."\\".$platformscope."\\cppcoverage.txt";
my $MAKEXML	= $testframework_root."\\CPPCoverage\\makexml.pl";
my $FILES_DIR = $testframework_root."\\CPPCoverage\\files";
my @FILES	= (
	"home.gif",
	"CPPCoverage.xsl",
	"index.html"
	);

# multiline system() wrapper with error checking
sub execute {
	my $cmds			= shift;
	my $ignore_errors	= shift; # die on error if undef

	my @cmda = split(/\n/, $cmds);
	foreach my $cmd (@cmda) {
		print "shell> $cmd\n";
		my $res = (0xffff & system($cmd));
		if ($res > 0x80) {
			$res >>= 8;
		};
		if (($res ne 0) && (!defined $ignore_errors)) {
			die "Command '$cmd' failed with returncode $res";
		}
	}
	print "\n";
}



# get result dir
my $resultdir	= $repositorypath."\\CppCoverage";
$resultdir=~/^\\/ and do {
	my $path = cwd();
	(my $unit) = split(/:/, $path);
	$resultdir = $unit.':'.$resultdir;
	$resultdir=~s/.:\\\\/\\\\/;
};

# get output file
-f $SPECS and do {
	my $output;
	my $covfile;
	open S, "<".$SPECS or die "Can't read spec file \"".$SPECS."\"!\n";
	while (<S>) {
		s/#.+$//;
		s/^(\s|\t)+//;
		s/(\s|\t)+$//;
		/^o:(.+)$/ and do {
			# output file
			$output = $1;
			print "Code coverage analysis output: $output\n";
			last;
		};
	};
	close S;
	defined $output or die "No output file specified in spec file.\n";

	$covfile=$EPOCDRIVE.$EPOCROOT.$output;
	$covfile=~s/xml$/cov/;
	$output=$resultdir."\\".$output;
	
	-f $covfile and do {
		# create dir in resultpath
		TestUtils::createdir($resultdir) or die;
		execute("$MAKEXML $covfile $output");
		# copy utility files: home.gif, coverage.xsl and index.html
		for my $file (@FILES) {
			TestUtils::copyforce($FILES_DIR."\\".$file, $resultdir."\\".$file) or die;
		};
	};
};
