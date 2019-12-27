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
    scalar @ARGV or die "pow: x not specified\n";
    my $x = shift @ARGV;
    scalar @ARGV or die "pow: p not specified\n";
    my $p = shift @ARGV;
    print_pow($x, $p)
}
if ($type == 2) {
    scalar @ARGV or die "bubble_sort: the array is empty\n";
    print_bubble_sort(@ARGV);
}
if ($type == -1) {
    scalar @ARGV or die "rnd: message amount not specified\n";
    my $len = shift @ARGV;
    for (my $i = 0; $i < $len; $i++) {
        $type = int(rand 3);
        if ($type == 0) {
            my $n = int(rand 20) + 1;
            print_fibonacci($n)
        }
        if ($type == 1) {
            my $x = int(rand 10) + 1;
            my $p = int(rand 4) + 1;
            print_pow($x, $p)
        }
        if ($type == 2) {
            my @arr;
            my $arr_len = int(rand 17) + 4;
            for (my $j = 0; $j < $arr_len; $j++) {
                @arr[$j] = int(rand 100);
            }
            print_bubble_sort(@arr);
        }
    }
}

sub print_fibonacci {
    my ($n) = @_;
    my $msg = pack 'CL<L<', $type, 4, $n;
    print $msg;
}

sub print_pow {
    my ($x, $p) = @_;
    my $msg = pack 'CL<L<L<', $type, 8, $x, $p;
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