#!/bin/perl
require 5.8.0;

use Env qw($platformscope $tests_root); 
my $SPECS=$tests_root."\\".$platformscope."\\cppcoverage.txt";
-f $SPECS or print STDERR "No coverage spec for build scope $platformscope\n";
