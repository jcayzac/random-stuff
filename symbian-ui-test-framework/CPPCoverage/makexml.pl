#!/bin/perl
require 5.008;
use strict;
package mkx;
use Data::Dumper;
use Encode;

sub err_usage {
	my $errstr = shift;
	print STDERR "Usage: makexml.pl <input.cov> <output.xml>\n";
	if ($errstr) {
		die "ERROR: $errstr\n";
	}
}

my $cov_input=shift;
$cov_input or err_usage("No .cov file supplied.");
-f $cov_input or err_usage("Can't find file \"$cov_input\".");

my $xml_output=shift;
$xml_output or err_usage("No .xml file specified.");

# parse input
my %data;

open COV, "<$cov_input" or err_usage("Can't open \"$cov_input\".");
while(<COV>) {
	/^Module Statistics for (.+)$/ and do {
		$data{current}{name} = $1;
		next;
	};
	/^Source file: (.+)$/ and do {
		$data{current}{file}{name} = $1;
		# ignore system headers
		$data{current}{file}{name}=~/epoc32.include./ and do {
			$data{current}{file}{name} = undef;
		};
		defined $data{current}{file}{name} and do {
			# add reference to current module
			my $fileref = $data{current}{file}{name}.'=fileref';
			$data{'modules'}{$data{current}{name}.'=module'}{reflist}{$fileref} = '';
		};
		next;
	};
	/^(-+)$/ and do {
		next;
	};
	/^\s+Lines in module: (\d+)$/ and do {
		$data{'modules'}{$data{current}{name}.'=module'}{linecount} = $1;
		next;
	};
	/^\s+Module line coverage: (\d+)[^\d].+%$/ and do {
		next;
	};
	/^\s*(\d+):/ and do {
		# willbe updated till the end of the current file
		defined $data{current}{file}{name} and do {
			my $filename = $data{current}{file}{name}.'=file';
#			$data{files}{$filename}{linecount}++;
			$data{files}{$filename}{linecount} = $1;
		};
		# no next statement!
	};
	/^\s*(\d+):\s\s\s\s\*\s\s\s\s(.+)$/ and do {
		defined $data{current}{file}{name} and do {
			my $filename = $data{current}{file}{name}.'=file';
			$data{files}{$filename}{linematch}{$1}{ord}=$1;
			$data{files}{$filename}{linematch}{$1}{cov}='y';
			$data{files}{$filename}{linematch}{$1}{src}=$2;
		};
		next;
	};
	/^\s*(\d+):\s\s\s\s\.\s\s\s\s(.+)$/ and do {
		defined $data{current}{file}{name} and do {
			my $filename = $data{current}{file}{name}.'=file';
			defined $data{files}{$filename}{linematch}{$1}{cov} or do {
				$data{files}{$filename}{linematch}{$1}{ord}=$1;
				$data{files}{$filename}{linematch}{$1}{cov}='n';
				$data{files}{$filename}{linematch}{$1}{src}=$2;
			};
		};
		next;
	};

}
close COV;
delete $data{current};

foreach my $module (keys %{$data{'modules'}}) {
	print $module."\n";
	my $total_covered		= 0;
	my $total_notcovered	= 0;
	
	foreach my $file (keys %{$data{files}}) {
		my $covered		= 0;
		my $notcovered	= 0;
		foreach my $lin (keys %{$data{files}{$file}{linematch}}) {
			if ($data{files}{$file}{linematch}{$lin}{cov} eq 'y') {
				$covered++;
				$total_covered++;
			}
			else {
				$notcovered++;
				$total_notcovered++;
			}
			push @{$data{files}{$file}{'lines/line'}}, \%{$data{files}{$file}{linematch}{$lin}};
		}
		delete $data{files}{$file}{linematch};
		$data{files}{$file}{covered}	= $covered;
		$data{files}{$file}{notcovered}	= $notcovered;
		$data{files}{$file}{linecount}++; # last line is skipped, in the above code
	}

#	foreach my $file (keys %{$data{'modules'}{$module}{reflist}}) {
#		$file=~s/=.+$//;
#		$file.='=file';
#		
#		foreach my $hashref (@{$data{files}{$file}{'lines/line'}}) {
#			my %info = %{$hashref};
#			$info{cov} eq 'y' and do { $total_covered++; };
#			$info{cov} eq 'n' and do { $total_notcovered++; };
#		}
#	}
	my $coverage = $total_notcovered + $total_covered;
	if ($coverage > 0) {
		$coverage = ($total_covered * 100) / $coverage;
	}
	else {
		$coverage = 0;
	}

	$data{'modules'}{$module}{coverage}		= $coverage;
}
XMLWrite($xml_output, "codecoverage", \%data, "CPPCoverage.xsl");

#####################:.
# Poor man's XML (v1.1)
####:.
my $cols=0;
# return a xml litteral
sub XMLDumper {
	our $indent=0;
	our @entryname;
	push @entryname, 'entry';
	return dump_xml_recurse(@_);
	
	sub dump_xml_recurse {
		my $bufferref	= shift;
		my $ref			= shift;
		my $buffer		= ${$bufferref};
		
		$cols > 72 and do {
			$cols=0;
			print STDERR "\n";
		};
		
		sub xml_escape {
			my $text = shift;
			# ugly: replace \ with /
			$text=~s/\\/\//g;
			#
			$text=~s/([^A-Za-z0-9_.-\/:;\(\)\{\}\"\'=\+\s])/sprintf("&#x00%02X;", ord($1))/ge;
			return $text;
		}
		
		sub Indent {
			my $b = '';
			for (my $i=0; $i<$indent; $i++) {
				$b .= "\t";
			}
			return $b;
		}
		
		if ($ref=~/^ARRAY/) {
			my @d = @{$ref};
			my $entry = $entryname[$#entryname];
			
			print STDERR "@"; ++$cols;
			foreach my $k (@d) {
				$buffer .= "\n".Indent().'<'.$entry.'>';
				$indent++;
				$bufferref = dump_xml_recurse(\$buffer, $k);
				$buffer = ${$bufferref};
				$indent--;
				@d < 2 or do {
					$buffer .= "\n".Indent();
				};
				$buffer .= "</".$entry.">";
			}
		}
		elsif ($ref=~/^HASH/) {
			my %d = %{$ref};
			
		print STDERR "%"; ++$cols;
			foreach my $k (sort keys %d) {
				(my $name, my $entry) = split(/\//, $k);
				($name, my $tagged) = split(/=/, $name);

				if ("$name" ne '') {
					$buffer .= "\n".Indent();
					if (defined $tagged) {
						$buffer .= '<'.$tagged.' id="'.xml_escape($name).'">';
					}
					else {
						$buffer .= '<'.$name.'>';
					}
					$indent++;
				}
				push @entryname, $entry;
				$bufferref = dump_xml_recurse(\$buffer, $d{$k});
				$buffer = ${$bufferref};
				pop @entryname;
				if ("$name" ne '') {
					$indent--;
					if ($entry ne undef) {
						$buffer .= "\n".Indent();
					}
					if (defined $tagged) {
						$buffer .= '</'.$tagged.'>';
					}
					else {
						$buffer .= '</'.$name.'>';
					}
				}
			}
		}
		else {
			$buffer .= xml_escape($ref);
		}
		return \$buffer;
	}
}

# write an XML document
sub XMLWrite {
	my $file		= shift;
	my $document	= shift;
	my $dataref		= shift;
	my $xsl			= shift; # opt
	my $extradata	= shift; # opt
	my $xmlbuf = '';
	$xmlbuf = ${XMLDumper(\$xmlbuf, $dataref)};
	print STDERR "\n";

	$xsl or do { $xsl = "results.xsl"; };
	
	my $buffer =	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
				.	"<?xml-stylesheet type=\"text/xsl\" href=\"".$xsl."\"?>\n"
				.	"<".$document.">\n"
				.	$xmlbuf;
	$extradata and do { $buffer .= $extradata; };
	$buffer		.=	"</".$document.">\n";

	my $out;
	open $out, ">:raw", $file;
	print $out encode("utf-8", $buffer);
	close $out;
	1;
}
