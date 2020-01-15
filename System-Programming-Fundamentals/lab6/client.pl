#!/usr/bin/perl -T

use strict;
use warnings qw(FATAL all);
use IO::Socket::INET;

my $USAGE = "Usage $0 host port paths...\n";

scalar @ARGV >= 3 or die $USAGE;
my ($host) = shift(@ARGV) =~ /([A-Za-z0-9.]+)/ or die $USAGE;
my ($port) = shift(@ARGV) =~ /(\d+)/ or die $USAGE;
my $request = join("\r\n", @ARGV) . "\r\n\r\n";

my $clt = new IO::Socket::INET(
    PeerHost => $host, PeerPort => $port, Proto => 'tcp'
) or die "Unable to connect to $host:$port: $!\n";

$clt->send($request);

while (<$clt>) {
    print $_;
}