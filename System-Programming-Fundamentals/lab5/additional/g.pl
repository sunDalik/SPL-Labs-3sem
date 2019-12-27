#!/usr/bin/perl

use strict;
use warnings qw(FATAL all);

use constant MSG_FMT => 'CQ<';
use constant MSG_SIZE => length pack(MSG_FMT, 0, 0);

scalar @ARGV or die "Usage: $0 type [args]\n";
my $type = shift @ARGV;

if ($type == 0) {
    scalar @ARGV or die "fibonacci: n not specified\n";
    my $n = shift @ARGV;
    my $msg = pack 'CQ<Q<', $type, 8, $n;
    #print STDERR "fibonacci n=$n\n";
    print $msg;
}
if ($type == 1) {
    scalar @ARGV or die "pow: n not specified\n";
    my $n = shift @ARGV;
    my $msg = pack 'CQ<Q<', $type, 8, $n;
    #print STDERR "pow n=$n\n";
    print $msg;
}
if ($type == 2) {
    scalar @ARGV or die "bubble-sort: len not specified\n";
    my $len = scalar @ARGV;
    #print STDERR "bubble-sort len=$len";

    my $msg = pack 'CQ<', $type, $len * 8;
    print $msg;

    while (@ARGV) {
        my $num = shift @ARGV;
        print STDERR " $num";
        print pack 'Q<', $num;
    }
    #print STDERR "\n";
}

if ($type == -1) {
    #)))
}