#!/usr/bin/perl -T
use strict;
use warnings qw(FATAL all);
use Getopt::Long;

my $ignore_interrupts_mode = 0;
my $append_mode = 0;
my $errors = 0;

$SIG{'INT'} = \&handle_interruptions;

sub handle_interruptions {
	if(!$ignore_interrupts_mode){
		print "\n";
		exit $errors;
	}
}

GetOptions("a" => \$append_mode,
	"i" => \$ignore_interrupts_mode)


