#!/usr/bin/perl -T
use strict;
use warnings qw(FATAL all);
use IO::Socket::UNIX;

my $SOCKET_PATH = "LAB5_NDK_SOCKET";
my $client = IO::Socket::UNIX->new(
    Type => SOCK_STREAM(),
    Peer => $SOCKET_PATH,
) or die "Cannot connect to server $SOCKET_PATH\n";

while (<$client>) {
    print("$_");
}

close($client);