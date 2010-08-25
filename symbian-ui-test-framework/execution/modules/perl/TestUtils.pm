#####################:.
# Utils
####:.

# THIS FILE IS DEPRECATED!
#
# The TestReport.pm Perl module is meant to get rewritten in Python
# using the available Python XML handling facilities, and won't be
# maintained in the future...
#
###################################################################

package TestUtils;
require 5.8.0;
use Encode;
use Data::Dumper;
use Win32;
use Win32::Process;
use File::Copy;
use Carp;
use strict;
use Env qw($EPOCDRIVE $EPOCROOT $TEMP $TMP $repositorypath $platformscope); 

sub new {
	my $class = shift;
	my $self = { @_ };
	bless($self, $class);
	#
	return($self);
}

# copy a file, overriding the destination if it already exists.
sub copyforce {
	my $src = shift;
	my $dst = shift;
	if (-f $src) {
		chmod(0666, $src);
		if (-f $dst) {
			chmod(0666, $dst);
			unlink $dst;
		}
		copy($src, $dst) or do {
			carp "!Could not copy '".$src."' to '".$dst."': ".$!."\n";
			return undef;
		};
		print "\t\"".$dst."\" created.\n";
		return 1;
	}
	carp "!Could not find '".$src."'!\n";
	return undef;
}

# delete a file
sub delfile {
	my $file	= shift;
	chmod(0666, $file);
	unlink $file or return undef;
	1;
}

sub deltree {
	my $dir		= shift;
	opendir(my $handle, $dir);
	my @entries = grep(!/^\./, readdir($handle));
	closedir($handle);
	foreach my $entry (@entries) {
		if (-d $dir."\\".$entry) {
			chmod(0777, $dir."\\".$entry);
			deltree($dir."\\".$entry);
		}
		else {
			chmod(0666, $dir."\\".$entry);
			unlink $dir."\\".$entry;
		}
	}
	chmod(0777, $dir);
	unlink $dir;
}

sub createdir {
	my $dir		= shift;
	my $base	= shift;

	# stop recursion if $dir is empty
	$dir or return 1;

	my $isnetwork;
	$base=~/^\\\\/ and do { $isnetwork=1; };
	if ($dir=~/^(\\\\[^\\]+)(.+)$/) {
		$base=$1;
		$dir=$2;
	}
	$dir or return 1;
	

	# remove drive unit or network host
	(my $unit, my $path) = split(/:/, $dir);
	
	$path && do {
		print "unit: $unit\npath: $path\n";
		$dir = $path;
		if ($isnetwork) {
			$base = $unit;
		}
		else {
			$base = $unit.":";
		}
	};
	
	(my $leftmost, my @pathsplit) = split(/\\/, $dir);
	
	if ($base) {
		$leftmost and do { $base .= "\\".$leftmost; };
	}
	else {
		$base = $leftmost;
	}

	$leftmost and do {
		mkdir $base or do {
			-d $base or do {
				carp "Cannot create \"".$base."\"!\n";
				return undef;
			};
		};
		print "Created \"".$base."\".\n";
	};
	
	if (scalar @pathsplit > 0) {
		return createdir(join("\\", @pathsplit), $base);
	}
	1;
}

sub MBMtoPNG {
	my $file	= shift;
	
	if ($file=~/\.mbm$/) {
		my $old_file = $file;
		$file=~s/\.mbm$//;
		my $new_file = $file.".bmp";
		
		# decompiles the mbm in several bmp
		my $cmdline = 'bmconv /u "'.$old_file.'" "'.$new_file.'"';
		system($cmdline.' >NUL: 2>&1') || print $!."\n";
		chmod(0666, $old_file);
		# remove the old mbm file
		unlink $old_file || print $!."\n";
		
		$old_file = $new_file;
		$new_file = $file.".png";
		
		# concert bmp to png.
		$cmdline = 'bmp2png "'.$old_file.'" "'.$new_file.'"';
		system($cmdline.' >NUL: 2>&1') || print $!."\n";
		chmod(0666, $old_file);
		# remove the old bmp files
		unlink $old_file || print $!."\n";
		
		print "=Converted '".$file.".mbm' to '".$file.".bmp\n";
		
		return -f $new_file;
	}
	return undef;
}

sub UCS2toUTF8 {
	my $file	= shift;

	my $buffer='';
	open my $in,  "<:encoding(UCS-2LE)", $file or return undef;
	while(<$in>) {
		$buffer.=$_; 
	}
	close $in;
	unlink $file;

	open my $out, ">:raw", $file or return undef;
	print $out encode("utf8", $buffer) or return undef;
	close $out or return undef;

	return -f $file;
}

sub traversetree {
	my $dir		= shift;

	my $hashref	= shift;
	my %handlers = %{$hashref};

	# optional args	
	my @args	= @_;

	# read dir
	opendir(my $handle, $dir);
	my @entries = grep(!/^\./, readdir($handle));
	closedir($handle);
	
	# for each directory, traverse it
	foreach my $entry (sort @entries) {
		-d $dir."\\".$entry && do {
			traversetree($dir."\\".$entry, $hashref, @args) or return undef;
		};
	}
	
	# for each file, check handle
	foreach my $expression (sort keys %handlers) {
		my $coderef = \&{$handlers{$expression}};
		foreach my $entry (sort @entries) {
			-f $dir."\\".$entry && do {
				if ($entry=~/$expression/i) {
					&$coderef($dir."\\".$entry, @args);
				}
			};
		}
	}
	1;
}

#####################:.
# Poor man's XML
####:.

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
			
			foreach my $k (sort keys %d) {
				(my $name, my $entry) = split(/\//, $k);
				if ("$name" ne '') {
					$buffer .= "\n".Indent().'<'.$name.'>';
					$indent++;
				}
				push @entryname, $entry;
				$bufferref = dump_xml_recurse(\$buffer, $d{$k});
				$buffer = ${$bufferref};
				pop @entryname;
				if ("$name" ne '') {
					$indent--;
					if ($entry ne undef) {
						$buffer .= "\n".Indent().'</'.$name.">";
					}
					else {
						$buffer .= '</'.$name.">";
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

### Sample usage:
# my %data;
# foreach my $r (1 .. 3) {
# 	my %result;
# 	$result{'id'}		= $r;
# 	$result{'spec'}		= 'toto'.$r;
# 	$result{'value'}	= 'val of '.$r;
# 
# 	foreach my $i (1 .. 3) {
# 		my %screen;
# 		$screen{'ref'}	= 'ref'.$i.'.bmp';
# 		$screen{'got'}	= 'got'.$i.'.bmp';
# 		$screen{'diff'}	= 'diff'.$i.'.bmp';
# 		push @{$result{'screens/screen'}}, \%screen;
# 	}
# 	push @{$data{'tests/test'}}, \%result;
# }
# 
# my $buf='';
# $buf = ${XMLDumper(\$buf, \%data)};
# print $buf;
# XMLWrite("toto.xml", "mydoc");
######## /XML OUTPUT ########

sub cardinal_string {
	my $self	= shift;
	my $num		= shift;
	my $res		= 'th';
	$_ = $num;
	/1$/ && do { $res = 'st'; };
	/2$/ && do { $res = 'nd'; };
	/3$/ && do { $res = 'rd'; };
	return $num.$res;
}
