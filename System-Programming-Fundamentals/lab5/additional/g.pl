#!/usr/bin/perl

use strict;
use warnings qw(FATAL all);

use constant MSG_FMT => 'CL<';
use constant MSG_SIZE => length pack(MSG_FMT, 0, 0);

scalar @ARGV or die "Usage: $0 type [args]\n";
my $type = shift @ARGV;

if ($type == 0) {
    scalar @ARGV or die "fibonacci: n not specified\n";
    print_fibonacci(shift @ARGV)
}
if ($type == 1) {
    scalar @ARGV or die "pow: n not specified\n";
    print_pow(shift @ARGV, shift @ARGV)
}
if ($type == 2) {
    print STDERR scalar @ARGV;
    scalar @ARGV or die "bubble_sort: the array is empty\n";
    print_bubble_sort(@ARGV);
}

if ($type == -1) {
    #)))
}

sub print_fibonacci {
    my ($n) = @_;
    my $msg = pack 'CL<L<', $type, 4, $n;
    print $msg;
}

sub print_pow {
    my ($x, $p) = @_;
    my $msg = pack 'CL<L<L<', $type, 4, $x, $p;
    print $msg;
}

sub print_bubble_sort {
    my (@nums) = @_;
    my $len = scalar @nums;
    my $msg = pack 'CL<', $type, $len * 4;
    print $msg;

    while (@nums) {
        my $num = shift @nums;
        print pack 'L<', $num;
    }
}