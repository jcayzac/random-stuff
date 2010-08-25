#!/bin/perl
require 5.008;
use strict;

use Encode;
use Env qw($EPOCDRIVE $EPOCROOT $platformscope $tests_root $testframework_root);

sub err_usage {
	my $errstr = shift;
	print STDERR "Usage: cover.pl\n";
	if ($errstr) {
		die "ERROR: $errstr\n";
	}
}

# collect objects
my $output;
my @objects;
my $SPECS=$tests_root."\\".$platformscope."\\cppcoverage.txt";
-f $SPECS or die "Can't open spec file \"".$SPECS."\"!\n";
open S, "<".$SPECS or die "Can't read spec file \"".$SPECS."\"!\n";
while (<S>) {
	s/#.+$//;
	s/^(\s|\t)+//;
	s/(\s|\t)+$//;
	/^o:(.+)$/ and do {
		# output file
		$output = $1;
		next;
	};
	length >0 and push @objects, $EPOCDRIVE.$EPOCROOT.$_;
};
close S;
scalar @objects or err_usage("No object specified in spec file.\n");
defined $output or err_usage("No output file specified in spec file.\n");
$output=~s/xml$/cov/;
$output=$EPOCDRIVE.$EPOCROOT.$output;

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
		}
		if (($res ne 0) && (!defined $ignore_errors)) {
			die "Command '$cmd' failed with returncode $res";
		}
	}
	print "\n";
}

sub clear {
	# clear old files (ignore errors)
	execute (<<EOT
del xenia.pbi
del xenia.pbo
del xenia.pbt
EOT
, 1);
}

sub prep {
	# prepare objects
	execute("prep /NOLOGO /LV /OI xenia /OT xenia ".join(' ', @objects));
}

sub run {
	# run the profiler
	my $emulator = $EPOCDRIVE.$EPOCROOT.'epoc32\release\wins\udeb\epoc.exe';
	execute <<EOT
profile /NOLOGO /I xenia /O xenia $emulator
prep /NOLOGO /M xenia /OI xenia
EOT
;
}

sub updatecov {
	# append program listing to (existing) report
	execute <<EOT
touch $output
plist xenia >>$output
EOT
;
}

# main
chdir $EPOCDRIVE.$EPOCROOT;
clear();
prep();
run();
updatecov();
clear();
