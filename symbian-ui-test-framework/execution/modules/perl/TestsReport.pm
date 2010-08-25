#####################:.
# Tests report generator
####:.

# THIS FILE IS DEPRECATED!
#
# The TestReport.pm Perl module is meant to get rewritten in Python
# using the available Python XML handling facilities, and won't be
# maintained in the future...
#
###################################################################

package TestsReport;
require 5.8.0;
use Encode;
use TestUtils;
use Data::Dumper;
use Win32::Process;
use Cwd;
use Carp;
use strict;
use Env qw($EPOCDRIVE $EPOCROOT $TEMP $TMP $repositorypath); 

sub new {
	my $class = shift;
	my $self = { @_ };
	bless($self, $class);
	# temporary storage
	$self->{tempdir}='.';
	if ("$TEMP" ne "") {
		$self->{tempdir}="$TEMP";
	} elsif ("$TMP" ne "") {
		$self->{tempdir}="$TMP";
	}
	$self->{title} = 'Test Report';
	$self->{displayresult} = 0;
	$self->{logFile} = 0;
	#
	return($self);
}

# Report's title
sub setTitle {
	my $self	= shift;
	my $title	= shift;
	$self->{title} = $title;
	1;
}

# Report's related URL
sub addURL {
	my $self	= shift;
	my $name	= shift;
	my $url		= shift;
	$self->{urls}{$name} = $url;
	1;
}

# embed own XML inside the generated "results.xml" document.
sub addXML {
	my $self = shift;
	my $xtra = shift;
	$self->{extraxml} = $xtra;
	1;
}

# specify the XML stylesheet
sub setXSL {
	my $self = shift;
	my $xsl  = shift;
	my $xslScreenShots  = shift;
	-f $xsl or do {
		carp "Cannot find XSL \"".$xsl."\"!\n";
		return undef;
	};
	my @path = split(/\\/, $xsl);
	$self->{xsl}	 = pop @path;
	$self->{xslbase} = join("\\", @path);

	if (-f $xslScreenShots) {
		@path = split(/\\/, $xslScreenShots);
		$self->{xslScreenShots}	 = pop @path;
	}
	else {
		$self->{xslScreenShots}	 = $self->{xsl};	
	}
	1;
}


# set number of results per page
sub setResultsPerPage {
	my $self = shift;
	my $results = shift;
	$self->{resultsperpage} = $results;
	1;
}

# set sucess threshold
sub setSucessThreshold {
	my $self = shift;
	my $sucess_threshold = shift;
	$self->{sucess_threshold} = $sucess_threshold;
	1;
}

# generate the report
sub generate {
	my $self			= shift;
	my $home			= shift or return undef;
	my $referencezip	= shift;
	
	$self->loadReportInfo();
	
	$self->{xsl}			or do { carp "Could not find XSL stylesheet.\n"; return undef; };
	$self->{resultsperpage}	or do { $self->{resultsperpage} = 10000; };
	$self->{sucess_threshold}	or do { $self->{sucess_threshold} = 0; };
	
	if ($referencezip)
		{
		$self->{referencezip}	= $referencezip;
		-f $self->{referencezip} or do 
			{
			carp "Cannot find \"".$self->{referencezip}."\"!\n";
			return undef;
			};
		};

	$self->{home}			= $home;
	-d $self->{home} or do {
		carp "Directory \"".$self->{home}."\" does not exist!\n";
		return undef;
	};
	
	$self->{refdir}			= $home."\\ref";

	if ($referencezip)
		{
		TestUtils::deltree($self->{refdir});
		TestUtils::createdir($self->{refdir});
		}
	
	$self->{resdir}			= $home."\\res";
	-d $self->{resdir} or do {
		carp "Cannot find \"".$self->{resdir}."\"\n";
		return undef;
	};
	
	$self->{diffdir}		= $home."\\diff";
	TestUtils::deltree($self->{diffdir});
	TestUtils::createdir($self->{diffdir});
	
	# 1, convert all relevant files
	$self->prepareResults() or return undef;
	
	# 2, unzip reference result (option)
	if ($referencezip)
		{
		$self->prepareRef() or return undef;
		};
	
	# 3, collect
	$self->collectFiles() or return undef;
	
	# 4, compare
	$self->compareFiles() or return undef;
	
	# 5, build result struct
	$self->buildXML() or return undef;
	
	# 6, write XML
	$self->ouputRawXML() or return undef;
	
	# 7, call xalan
	$self->invokeXalan() or return undef;

	1;
}

################### end of public API ###################

# Convert all files meant to be converted...
sub prepareResults {
	my $self	= shift;

	# convert all
	print "------------------------------------------------------------------------------\n";
	print ".oO Converting files to usable formats...\n";
	print "------------------------------------------------------------------------------\n";
	my %handlers;
	$handlers{'\.mbm$'} = \&TestUtils::MBMtoPNG;
	$handlers{'\.log$'} = \&TestUtils::UCS2toUTF8;
	$handlers{'(^\.log)\.txt$'} = \&TestUtils::UCS2toUTF8;
	TestUtils::traversetree($self->{resdir}, \%handlers) or return undef;
	print "done!\n";
	1;
}

# Prepare reference files
sub prepareRef {
	my $self	= shift;

	# unzip reference
	print "------------------------------------------------------------------------------\n";
	print ".oO Unzipping reference files into ".$self->{refdir}."...\n";
	print "------------------------------------------------------------------------------\n";
	system("unzip -o \"".$self->{referencezip}."\" -d \"".$self->{refdir}."\""); # == 0 or return undef;
	print "done!\n";
	1;
}

# Move masks in resdir
sub xferMasks {
	my $self	= shift;

	print "------------------------------------------------------------------------------\n";
	print ".oO Copying screenshots masks from reference directory...\n";
	print "------------------------------------------------------------------------------\n";
	my %handlers;
	$handlers{'\.mask\.png$'} = sub {
		my $file	= shift;
		my $refdir	= shift;
		my $resdir	= shift;
		# build relative path (assumes $file is inside $refdir)
		$file = substr $file, length $refdir."\\";
		# create destination folder if it doesn't exist yet
		my @path = split(/\\/, $file);
		pop @path;
		TestUtils::createdir(join("\\", @path), $resdir);
		# copy file
		TestUtils::copyforce($refdir."\\".$file, $resdir."\\".$file);
		# delete original file
		TestUtils::delfile($refdir."\\".$file);
	};
	TestUtils::traversetree($self->{refdir}, \%handlers, $self->{refdir}, $self->{resdir}) or return undef;
	print "done!\n";
	1;
}

sub createReference {
	my $self	= shift;
	
	print "------------------------------------------------------------------------------\n";
	print ".oO Creating new reference zip in \"".$self->{home}."\"...\n";
	print "------------------------------------------------------------------------------\n";
	-f $self->{home}."\\reference.zip" or TestUtils::delfile($self->{home}."\\reference.zip");

	my $current_dir = cwd();
	chdir $self->{resdir};
	system("zip -9r \"".$self->{home}."\\reference.zip\" .") == 0 or return undef;
	chdir $current_dir;

	print "done!\n";
	1;
}

# Collect files (union between actual results and reference)
sub collectFiles {
	my $self	= shift;
	
	print "------------------------------------------------------------------------------\n";
	print ".oO Collecting files...\n";
	print "------------------------------------------------------------------------------\n";
	defined $self->{files} && undef $self->{files};
	my %handlers;
	$handlers{'\..+$'} = sub {
						my $file	= shift or return undef;
						my $dir		= shift or return undef;
						my $hashref	= shift or return undef;
						# ignore masks in collection
						$file=~/\.mask\.png$/i or $file=~/\.cmt$/i or $file=~/\.log\.txt$/i or do {
							# build relative path (assumes $file is inside $dir)
							$file = substr $file, length $dir."\\";
							defined ${%{$hashref}}{$file} or do {
								print "\tFile \"".$file."\" added to database.\n";
								${%{$hashref}}{$file} = 1;
							};
						};
					};
	# build the union between the two trees
	TestUtils::traversetree($self->{resdir},	\%handlers, $self->{resdir},	\%{$self->{files}}) or return undef;
	TestUtils::traversetree($self->{refdir},	\%handlers, $self->{refdir},	\%{$self->{files}}) or return undef;
	print "done!\n";
	1;
}

# compare files
sub compareFiles {
	my $self	= shift;
	
	print "------------------------------------------------------------------------------\n";
	print ".oO Comparing actual files to reference...\n";
	print "------------------------------------------------------------------------------\n";
	defined $self->{results} && undef $self->{results};
	
	foreach my $file (sort keys %{$self->{files}}) {

		# check if file has disappeared
		-f $self->{resdir}."\\".$file or do {
			${%{$self->{results}}}{$file} = -1;
			next;
		};
		
		# check if file is new
		-f $self->{refdir}."\\".$file or do {
			${%{$self->{results}}}{$file} = -2;
			next;
		};
		
		# if both files exist, check if they're different
		for($file) {
			/\.png$/i and do {
				${%{$self->{results}}}{$file} = $self->comparePNG($self->{resdir}, $self->{refdir}, $self->{diffdir}, $file);
				last;
			};
			/\.(txt|log)$/i and do {
				${%{$self->{results}}}{$file} = $self->compareTEXT($self->{resdir}, $self->{refdir}, $self->{diffdir}, $file);
				last;
			};
			/\.cmt$/i and do {
				push @{$self->{supportfiles}{$file}}, $file;
				last;
			};
			${%{$self->{results}}}{$file} = $self->compareBINARY($self->{resdir}, $self->{refdir}, $self->{diffdir}, $file);

			# check if they are non implemented tests
			if ($self->IsNonImplemented($self->{resdir}, $file)) {
				${%{$self->{nonimplemented}}}{$file} = 1;
			}

		}
	}
	print "done!\n";
	1;
}

# compare two bitmaps
sub comparePNG {
	my $self	= shift;
	
	my $resdir		= shift;
	my $refdir		= shift;
	my $diffdir		= shift;
	my $file		= shift;
	
	print "\tPNG file  \"".$file."\"...\n";
	
	#####################:.
	# build diff file
	####:.
	
	# first, check if a mask exist in the res dir
	my $mask = maskName($refdir, $file);
	$mask and do {
		push @{$self->{supportfiles}{$file}}, $mask;
	};
	
	# next, create diff subdir
	my @path = split(/\\/, $file);
	pop @path;
	TestUtils::createdir(join("\\", @path), $diffdir);
	
	# then, compare
	my $command =	'CompareBitmap'
	.			' "'.$refdir."\\".$file.'"'
	.			' "'.$resdir."\\".$file.'"'
	.			' "'.$diffdir."\\".$file.'"'
	.			' /f1';
	-f $refdir."\\".$mask and do {
		$command .= ' /m "'.$refdir."\\".$mask.'"';
		print "\t\t(will use image \"".$mask."\" as a mask)";
	};
	my $res = (0xffff & system($command));
	print " (".sprintf("%04x",$res).")";
	if ($res > 0x80) {
		$res >>= 8;
	};
	if ($res > 100) {
		$res = 100;
	};
	# return diff value
	print " (".$res."%)\n";
	$res;
	
	sub maskName {
		my $resdir	= shift;
		my $file	= shift;
		$file=~/\.png$/i or return undef;
		
		my @path	= split(/\\/, $file);
		do {
			scalar @path or return undef;
			my $mask = join("\\", @path);
			for ($mask) {
				/\.png$/i and do { $mask=~s/\.png$/\.mask\.png/i; last; };
				$mask .= '.mask.png';
			};
			 -f $resdir."\\".$mask and return $mask;
		} while(pop @path);
		return undef;
	}
}

# compare two text files
sub compareTEXT {
	my $self	= shift;

	my $resdir		= shift;
	my $refdir		= shift;
	my $diffdir		= shift;
	my $file		= shift;

	print "\tText file \"".$file."\"...\n";

	# compare
	# TODO: generate diff text with GNU diff?
	return 0;
}

# Check a special value in the file "NOT IMPLEMENTED"
#
# TODO one function per file type ?
#
sub IsNonImplemented {
	my $self	= shift;

	my $resdir		= shift;
	my $file		= shift;
	
	my $handle;
	my $content1;

	open $handle, "<:raw", $resdir."\\".$file or return 0;
	read($handle, $content1, -s $handle);
	close($handle);
	
	if ($content1 eq "NOT IMPLEMENTED")
		{
		return 1;
		}

	0;

}

# compare two binaries
sub compareBINARY {

	my $self	= shift;
	
	my $resdir		= shift;
	my $refdir		= shift;
	my $diffdir		= shift;
	my $file		= shift;
	
	print "\tRaw file  \"".$file."\"...\n";
	# compare
	my $handle;
	my $content1;
	my $content2;
	
	open $handle, "<:raw", $resdir."\\".$file or return 100;
	read($handle, $content1, -s $handle);
	close($handle);
	
	open $handle, "<:raw", $refdir."\\".$file or return 100;
	read($handle, $content2, -s $handle);
	close($handle);
	
	# TODO: how to get finer values?
	$content1 eq $content2 && return 0;
	100;
}

# Read the result file (.txt or .bin) :
#	*get the result value (the first line of the file)
#	*check if there is a log reference. If this is the case, a new log file (.log.txt) is created containing the test log.
sub readResultFile {
	my $self = shift;
	my $filename = shift;

	$self->{logFileNameInGot} = 0;
	$self->{logFileNameInRef} = 0;
	
	if ($filename !~ /\.(txt|bin)$/i) {
		print "Cannot read value because it is neither a .bin nor .txt file!\n";
		return 0;
	}

	my $path;
	my @content;
	my $line;
	my @log_ref;
	my @logsToCopy;
	my $logFileName;
	my @logFileNameArray;
	my $logline;
	
	@logFileNameArray = split (/\\/, $filename);
	$logFileName =  $logFileNameArray[0]."\\".$logFileNameArray[1]."\\".$logFileNameArray[1].".log.txt";
			
	#Open the LOG result file
	if ($self->{displayresult} || $self->{logFile}) {
		$path = $self->{resdir}."\\".$filename;
		if (open(FILEIN, $path)) {
			@content = <FILEIN>;
			close FILEIN;
			
			#Get log result
			if ($self->{displayresult}) {
				$line = $content[0];
				chomp($line);
				$self->{valuegot} = $line;
			}
			
			#Check log references
			if ($self->{logFile}) {
				foreach $line (@content) {
					chomp($line);
					if($line=~s/\[LOGREF\]://) {
						@log_ref = split(/-/, $line);
						splice (@logsToCopy, 0, 0, @{$self->{testlogs}}[$log_ref[0]..$log_ref[1]]);
						$path = ">".$self->{resdir}."\\".$logFileName;
						if(open(LOGOUT, $path)) {
							$self->{logFileNameInGot} = "log\\".$logFileName;
							foreach $logline(@logsToCopy) {
								print LOGOUT $logline;
							}
							close LOGOUT;
						}
						else {
							print "Cannot open for write :$path!\n";
						}
					}
				}
			}
		}
		else {
			print "Cannot open the ref BIN file for read :$path!\n";
		}
	}

	#Open the REF result file
	if ($self->{displayresult}) {
		$path = $self->{refdir}."\\".$filename;
		if (open(FILEIN, $path)) {
			@content = <FILEIN>;
			close FILEIN;
			#Get ref result
			$line = $content[0];
			chomp($line);
			$self->{valueref} = $line;
		}
		else {
			print "Cannot open the ref BIN file for read :$path!\n";
		}
		#Check log references
		$path = $self->{refdir}."\\".$logFileName;
		if(open(LOGOUT, $path)) {
			$self->{logFileNameInRef} = "ref\\".$logFileName;
			close LOGOUT;
		}
	}
	1;
}

# publish data in $self->{xml}
sub buildXML {
	my $self = shift;
	
	print "------------------------------------------------------------------------------\n";
	print ".oO Converting results to XML...\n";
	print "------------------------------------------------------------------------------\n";
	defined $self->{xml} && undef $self->{xml};
	
	#Check if there are test logs
	$self->checkAndStoreTestLogs();
	
	# build a hash with tests results and support files
	my %files;
	foreach my $file (sort keys %{$self->{results}}) {
		my @path = split(/\\/, $file);
		scalar @path and do {
			my $subfile = pop @path;
			@{$files{join("\\", @path)}{$subfile}} = ();
			if (defined $self->{supportfiles}{$file}) {
				foreach (sort @{$self->{supportfiles}{$file}}) {
					push @{$files{join("\\", @path)}{$subfile}}, $_;
				}
			}
		};
	}
	
	$self->{xmlentities} = scalar keys %files;

	# loop thru each test
	my %testsuites;

	foreach my $fullqualifiedtest (sort keys %files) {
		print "\tTest: $fullqualifiedtest\n";
		#
		#
		my %result;
		my @path = split(/\\/, $fullqualifiedtest);
		# test name
		$result{'name'} = pop @path;
		$result{'name'} or do { $result{'name'} = 'No name'; };
		# optional entities
		scalar @path and do { $result{'category'} = pop @path; };
		scalar @path and do { $result{'prologue'} = pop @path; };
		# files
		$result{'diffavg'} = 0;
		# ref and log count
		my $refcount=0;
		my $logcount=0;
		scalar keys %{$files{$fullqualifiedtest}} and do {
			foreach my $file (sort keys %{$files{$fullqualifiedtest}}) {
				my %fileh;
				
				$fileh{'diffvalue'}	= $self->{results}{$fullqualifiedtest."\\".$file};
				
				$fileh{'diffvalue'} > 100	and do { $fileh{'diffvalue'} = 100; };
				$fileh{'diffvalue'} < 0		and do { $fileh{'diffvalue'} = 100; };
				$result{'diffavg'} = $result{'diffavg'} + $fileh{'diffvalue'};
				
				$fileh{'name'} = $fullqualifiedtest."\\".$file;
				$file=~/\.png$/i			&& do { $fileh{'type'} = 'image'; };
				$file=~/\.(txt|log|html|cmt)$/i	&& do { $fileh{'type'} = 'text'; };
				
				-f  $self->{refdir} ."\\".$fullqualifiedtest."\\".$file && do {
					$fileh{'ref'}  = "ref\\" .$fullqualifiedtest."\\".$file;
					++$refcount;
				};
				-f  $self->{resdir} ."\\".$fullqualifiedtest."\\".$file && do {
					$fileh{'got'}  = "log\\" .$fullqualifiedtest."\\".$file;
					++$logcount;
				};				
				-f  $self->{diffdir}."\\".$fullqualifiedtest."\\".$file && do {
					$fileh{'diff'} = "diff\\".$fullqualifiedtest."\\".$file;
				};

				$self->readResultFile($fullqualifiedtest."\\".$file);
				if ($self->{displayresult}) {
						my %value;
						$value{'got'} = $self->{valuegot};
						$value{'ref'} = $self->{valueref};
						push @{$result{'/value'}}, \%value;
					}
				
				if ($self->{logFileNameInGot}) {
					$fileh{'loggot'} = $self->{logFileNameInGot};
					if ($self->{logFileNameInRef}) {
						$fileh{'logref'} = $self->{logFileNameInRef};
					}
				}
										
				defined $self->{supportfiles}{$fullqualifiedtest."\\".$file} && do {
					foreach my $supportfile (sort @{$self->{supportfiles}{$fullqualifiedtest."\\".$file}}) {
						push @{$fileh{'/support'}}, $supportfile;
						print '.';
					}
				};
				# comment per file
				-f  $self->{resdir} ."\\".$fullqualifiedtest."\\".$file.'.cmt' && do {
					my $commentFile = $self->{resdir} ."\\".$fullqualifiedtest."\\".$file.'.cmt';
					system("more $commentFile \> temp");
					open(DESCR,"temp");
					my $l=<DESCR>;
					close(DESCR);
					print "\n --- ".$l."\n";
					$l = substr($l,8,length($l));
					print "\n --- ".$l."\n";
					$fileh{'comments'}  = $l;
				};

				push @{$result{'/file'}}, \%fileh;

				# specification per test case
				my $commentFile = $file;
				$commentFile =~ s/\..+/\.spc\.cmt/;
				-f  $self->{resdir} ."\\".$fullqualifiedtest."\\".$commentFile && do {
					$commentFile = $self->{resdir} ."\\".$fullqualifiedtest."\\".$commentFile;
					open(DESCR,$commentFile);
					my @lines=<DESCR>;
					close(DESCR);
					my $comments;
					foreach my $line(@lines){
						$comments = $comments.$line;
					}
					$result{'comments'}  = $comments;
				};

				# is it a non-implemented test? 
				# if one of the test case is non implemented, 
				# the whole test is set as non implemented
				if (${%{$self->{nonimplemented}}}{$fullqualifiedtest."\\".$file}) {
					$result{'status'} = 'Not implemented';
				}
			}
			
			$result{'diffavg'} = $result{'diffavg'} / scalar keys %{$files{$fullqualifiedtest}};

			# if there is 0 not implemented test, set the status according to the  diffavg or
			# log/ref file presence
			if (! ($result{'status'} eq 'Not implemented') ) {	
				$result{'status'}  = 'Success';
				$result{'diffavg'} and do { $result{'status'} = 'Failure'; };
			
				$refcount ne $logcount and do {
					if ($refcount gt 0) {
						if ($logcount gt 0) {
							$result{'status'} = 'Mismatch';
						}
						else {
							$result{'status'} = 'Not run';
						}
					}
					else {
						$result{'status'} = 'New';
					}
				};
			}

			
			$result{'diffavg'}=~s/^(\d+\.\d+)$/sprintf("%.2f",$1)/e;
			
			#Statistics
			#Count run tests
			if (exists $testsuites{$result{'category'}}{'runtests'}) {
				$testsuites{$result{'category'}}{'runtests'}++;
			}
			else {
				$testsuites{$result{'category'}}{'runtests'}=1;
			}
			#Count passed tests
			if (exists $testsuites{$result{'category'}}{'passedtests'}) {
				if ($result{'status'} eq 'Success') {
					$testsuites{$result{'category'}}{'passedtests'}++;
				}
			}
			else {
				if ($result{'status'} eq 'Success') {
					$testsuites{$result{'category'}}{'passedtests'}=1;
				}
				else {
					$testsuites{$result{'category'}}{'passedtests'}=0;
				}
			}
			#Count failed tests = failed + not run
			if (exists $testsuites{$result{'category'}}{'failedtests'}) {
				if ( ($result{'status'} eq 'Failure') or ($result{'status'} eq 'Not run')) {
					$testsuites{$result{'category'}}{'failedtests'}++;
				}
			}
			else {
				if ( ($result{'status'} eq 'Failure') or ($result{'status'} eq 'Not run')) {
					$testsuites{$result{'category'}}{'failedtests'}=1;
				}
				else {
					$testsuites{$result{'category'}}{'failedtests'}=0;
				}
			}
		};
		
		push @{$self->{xml}{'/result'}}, \%result;
	}
	
	# add statistics
	my %statistics;
	my %total;
	
	$total{'runtests'} = 0;
	$total{'passedtests'} = 0;
	$total{'failedtests'} = 0;
	$total{'successrate'} = 0;
	
	foreach my $testsuite (keys (%testsuites)) {
		my %testsuitestat;
		$testsuitestat{'name'} = $testsuite;
		$testsuitestat{'runtests'} = $testsuites{$testsuite}{'runtests'};
		$testsuitestat{'passedtests'} = $testsuites{$testsuite}{'passedtests'};
		$testsuitestat{'failedtests'} = $testsuites{$testsuite}{'failedtests'};
		if ($testsuites{$testsuite}{'passedtests'} > 0 or $testsuites{$testsuite}{'failedtests'} > 0) {
			$testsuitestat{'successrate'} = int(100*$testsuites{$testsuite}{'passedtests'} / ($testsuites{$testsuite}{'passedtests'}+$testsuites{$testsuite}{'failedtests'}));
		}
		$total{'runtests'} += $testsuites{$testsuite}{'runtests'};
		$total{'passedtests'} += $testsuites{$testsuite}{'passedtests'};
		$total{'failedtests'} += $testsuites{$testsuite}{'failedtests'};

		$self->{runtests}{$testsuite} = $testsuites{$testsuite}{'runtests'};
		
		push @{$statistics{'/testsuite'}}, \%testsuitestat;
	}
	
	if ($total{'runtests'} > 0) {
		$total{'successrate'} = int(100*$total{'passedtests'} / $total{'runtests'});
	}

	push @{$statistics{'/total'}}, \%total;	
	push @{$self->{xml}{'/statistics'}}, \%statistics;
	


	# add title and urls and other info
	my %info;
	defined $self->{'title'} && do {
		$info{'title'} = $self->{'title'};
	};
		
	defined $self->{'urls'} && do {
		foreach (keys %{$self->{'urls'}}) {
			my %url;
			$url{'name'}	= $_;
			$url{'value'}	= $self->{'urls'}{$_};
			push @{$info{'/url'}}, \%url;
		}
	};
	
	if($self->{report_info_file}) {
		while (scalar(@{$self->{infoarray}}) > 0) {
			my %particular_info;
			$particular_info{'name'} = pop @{$self->{infoarray}};
			$particular_info{'value'} = pop @{$self->{infoarray}};
			push @{$info{'/unitinfo'}}, \%particular_info;
		}		
	}
	
	push @{$self->{xml}{'/info'}}, \%info;
	
	print "done!\n";
	1;
}

# save XML to a file
sub ouputRawXML {
	my $self = shift;
	
	print "------------------------------------------------------------------------------\n";
	print ".oO Writing XML and XSL documents...\n";
	print "------------------------------------------------------------------------------\n";

	print "\tCreating XML...\n";
	TestUtils::XMLWrite($self->{home}."\\results.xml",
						'tests',
						\%{$self->{xml}},
						$self->{xsl},
						$self->{extraxml}) or return undef;
	$self->{xsl} and do {
		print "\tCopying XSL...\n";
		TestUtils::copyforce($self->{xslbase}."\\".$self->{xsl}, $self->{home}."\\".$self->{xsl}) or return undef;
		TestUtils::copyforce($self->{xslbase}."\\".$self->{xslScreenShots}, $self->{home}."\\".$self->{xslScreenShots}) or return undef;
	};
	print "done!\n";
	1;
}

# invoke xalan
sub invokeXalan {
	my $self = shift;
	
	print "------------------------------------------------------------------------------\n";
	print ".oO Create HTML pages...\n";
	print "------------------------------------------------------------------------------\n";
	print "\nCreate Index ...\n";
	# Invoke Xalan.
	my $page_name = 'index.html';
	my $cmdline =	'xalan -u'
				.	' -p mode 2'
				.	' -o '.$self->{home}."\\".$page_name.' '.$self->{home}."\\results.xml ".$self->{home}."\\".$self->{xsl};
	print '$ '.$cmdline."\n";
	system($cmdline) == 0 or do {
		carp "Failed: $!\n";
		return undef;
	};

	
	foreach my $testsuite (sort keys %{$self->{runtests}}) {
		my $page_name = $testsuite.'.html';
		my $num_pages = $self->{runtests}{$testsuite} / $self->{resultsperpage};
		if ($num_pages=~/(\d+)\.\d+/) {
			$num_pages = $1;
			$num_pages++;
		}
		
		my $current_start = 0;
		my $current_page  = 1;
		while ($current_start <= $self->{runtests}{$testsuite}) {
			my $next_page  = $current_page + 1;
			my $next_start = $current_start + $self->{resultsperpage};
			
				if ($next_start > $self->{runtests}{$testsuite}) {
				$next_page = 'none';
			}
			
			# FIXME: add project_name and project_link
			my $cmdline =	'xalan -u'
						.	' -p mode 0'
						.	' -p start '.$current_start
						.	' -p end '.($next_start - 1)
						.	' -p current_page '.$current_page
						.	' -p num_pages '.$num_pages
						.	' -p sucess_threshold '.$self->{sucess_threshold}
							.	' -p testsuite '."'".$testsuite."'"
						.	' -o '.$self->{home}."\\".$page_name.' '.$self->{home}."\\results.xml ".$self->{home}."\\".$self->{xsl};
			print '$ '.$cmdline."\n";
			system($cmdline) == 0 or do {
				carp "Failed: $!\n";
				return undef;
			};
			
				$page_name     = $testsuite.$next_page.'.html';
			$current_page++;
			$current_start = $next_start;
		}	
	}
	
	# =============================================================================================================
	# TODO : make another sub to do that
	
	# build a hash with tests results and support files
	my %files;
	foreach my $file (sort keys %{$self->{results}}) {
		my @path = split(/\\/, $file);
		scalar @path and do {
			my $subfile = pop @path;
			@{$files{join("\\", @path)}{$subfile}} = ();
			if (defined $self->{supportfiles}{$file}) {
				foreach (sort @{$self->{supportfiles}{$file}}) {
					push @{$files{join("\\", @path)}{$subfile}}, $_;
				}
			}
		};
	}
	
	$self->{xmlentities} = scalar keys %files;
	
	# loop thru each test
	foreach my $testsuite (sort keys %{$self->{runtests}}) {
		for(my $test_num=1; $test_num<=$self->{runtests}{$testsuite}; $test_num++) {
			my $htmlfile = $testsuite.'_Test'.$test_num.'.html';
			print "\n\tCreating: $htmlfile\n";
			# FIXME: add project_name and project_link
			my $cmdline =	'xalan -u'
						.	' -p mode 1'
						.	' -p test_name '.$test_num
						.	' -p sucess_threshold '.$self->{sucess_threshold}
							.	' -p testsuite '."'".$testsuite."'"
						.	' -o '.$self->{home}."\\".$htmlfile.' '.$self->{home}."\\results.xml ".$self->{home}."\\".$self->{xslScreenShots};
			print '$ '.$cmdline."\n";
			system($cmdline) == 0 or do {
				carp "Failed: $!\n";
				return undef;
			};		
		}
	}
	# =============================================================================================================


	print "done!\n";
	1;
}

# Report info's file
sub setReportInfoFile {
	my $self	= shift;
	my $fileName	= shift;
	
	if (open(REPORTINFOIN, $fileName)) {
		$self->{report_info_file} = $fileName;
		close REPORTINFOIN;
	}
	else {
		print "Cannot open for read :$fileName!\n";
	}
	1;
}

# load TestsReport.inf
sub loadReportInfo{
	my $self = shift;

	print "------------------------------------------------------------------------------\n";
	print ".oO Load Report Info...\n";
	print "------------------------------------------------------------------------------\n";
	if (!$self->{report_info_file}) {
		print "There is no report info file\n";
		return;
	}
	
	if(!open(REPORTINFOIN, $self->{report_info_file})) {
		print "Cannot open for read :$self->{report_info_file}!\n";
		return;
	}
	
	my @report_info = <REPORTINFOIN>;
	close REPORTINFOIN;

	foreach my $info(@report_info) {
		chomp($info);
		if($info=~s/\[TITLE\]://i) { 
			$self->setTitle($info);
		}
		elsif($info=~s/\[(.+)\]://i) {
			unshift(@{$self->{infoarray}}, $1);
			unshift(@{$self->{infoarray}}, $info);
		}
	}

	print "done!\n";
	1;
}

# Report info's file
sub displayResultValues {
	my $self	= shift;
	$self->{displayresult} = 1;
	1;
}

# Search if there is a log. If there is, the content is stored to be used later.
sub checkAndStoreTestLogs {
	my $self	= shift;
	if (opendir(DIR, $self->{resdir})) {
		while (my $file = readdir DIR) {
			if($file =~ /\.log\.txt/i) {
				$self->{logFile} = $self->{resdir}."\\".$file;
				if(!open(LOGIN, $self->{logFile})) {
					print "Cannot open for read :$self->{logFile}!\n";
					0;
				}
				@{$self->{testlogs}} = <LOGIN>;
				close LOGIN;
				1;
			}
		}
	}	
	0;
}
