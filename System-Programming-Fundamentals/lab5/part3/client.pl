#!/usr/bin/perl -T

use strict;
use warnings qw(FATAL all);
use IO::Socket::UNIX;

my $SYSTEM_INFO_FMT = "i<i<i<d<d<d<d<";
my $SYSTEM_INFO_SIZE = length pack($SYSTEM_INFO_FMT, 0, 0, 0, 0, 0, 0);
my $SOCKET_PATH = "LAB5_NDK_SOCKET";

print "Connecting to socket $SOCKET_PATH \n";
my $client = IO::Socket::UNIX->new(Type => SOCK_STREAM(), Peer => $SOCKET_PATH)
    or die "Failed to connect to socket: $!\n";

my $sys_info_buf = "";
read $client, $sys_info_buf, $SYSTEM_INFO_SIZE;
my ($spid, $suid, $sgid, $rt, $sl1, $sl5, $sl15) = unpack $SYSTEM_INFO_FMT, $sys_info_buf;

print "Data received: \n";
print "time = $rt\n";
print "pid = $spid\n";
print "uid = $suid\n";
print "gid = $sgid\n";
print "average system load in 1min = $sl1\n";
print "average system load in 5min = $sl5\n";
print "average system load in 15min = $sl15\n";