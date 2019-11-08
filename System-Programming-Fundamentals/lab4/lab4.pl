#!/usr/bin/perl -T
use strict;
use warnings qw(FATAL all);
use Getopt::Long;

my $ignore_interrupts_mode = 0;
my $append_mode = 0;
my $errors = 0;
my $i = 0;
my @files;
my $file;
my $line;

$SIG{'INT'} = \&handle_interruptions;

sub handle_interruptions {
	if(!$ignore_interrupts_mode){
		print "\n";
		exit $errors;
	}
}

GetOptions("a" => \$append_mode,
	"i" => \$ignore_interrupts_mode);

foreach $file(@ARGV) {
	if($append_mode == 1){
		open($files[$i], ">>", $file) or die "Can't open file $file: $!";
	} else {
		open($files[$i], ">", $file) or die "Can't open file $file: $!"; 
	}
	$i++;
}

foreach $line(<STDIN>) {
	foreach $file(@files) {
		print $file $line;
	}
	print $line;
}

foreach $file(@files) {
	close($file);
}
