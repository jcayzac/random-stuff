#!/bin/perl
require 5.008;

use strict;
use Env qw(EPOCDRIVE EPOCROOT);

sub err_usage {
	my $errstr = shift;
	print STDERR "Usage: bldprofile <enable|disable>\n";
	if ($errstr) {
		die "ERROR: $errstr\n";
	}
}

my $generator="$EPOCDRIVE$EPOCROOT".'epoc32\tools\cl_win.pm';

my $arg=shift;
$arg or do { err_usage("No argument specified."); };

-f $generator or do {
	err_usage("Can't find \"$generator\"!");
};

for($arg) {
	/^enable$/i  and do {  enable_profiling(); last; };
	/^disable$/i and do { disable_profiling(); last; };
	err_usage("Illegal argument: $arg"); last;
}

sub enable_profiling {
	my @content;
	read_generator(\@content);
	foreach (1 .. scalar @content) {
		my $line=$content[$_];
		$line=~/\/debug/ and do {
			$line=~/\/profile/ or do {
				$line="\" /debug /profile \"\n";
			};
		};
		$content[$_]=$line;
	}
	write_generator(\@content);
	print "Profiling: enabled.\n";
}

sub disable_profiling {
	my @content;
	read_generator(\@content);
	foreach (1 .. scalar @content) {
		$content[$_]=~s/\/profile//g;
	}
	write_generator(\@content);
	print "Profiling: disabled.\n";
}

sub read_generator {
	my $ref=shift;
	open FILE, "<$generator" or err_usage("Can't open \"$generator\"!");
	@{$ref}=<FILE>;
	close FILE;
}

sub write_generator {
	my $ref=shift;
	open FILE, ">$generator" or err_usage("Can't open \"$generator\"!");
	print FILE join('', @{$ref});
	close FILE;
}
