#!/usr/bin/perl

use strict;
use warnings qw(FATAL all);
use threads;
use IO::Socket::INET;

my $USAGE = "Usage $0 port [thread_count]\n";
scalar @ARGV or die $USAGE;
my ($port) = shift(@ARGV) =~ /(\d+)/ or die $USAGE;

my $thread_count = 2;
if (scalar @ARGV) {
    ($thread_count) = shift(@ARGV) =~ /(\d+)/ or die $USAGE;
}

my $server = IO::Socket::INET->new(
    LocalPort => $port, Proto => 'tcp', Listen => SOMAXCONN
) or die "Failed to start the server: $!\n";

print "Listening on $port\n";

while (my $clt = $server->accept()) {
    threads->create(\&process_request, $clt)->detach();
}

sub process_request {
    my $clt = shift @_;

    my $clt_addr = $clt->peerhost;
    my $clt_port = $clt->peerport;
    print "Client connected: $clt_addr:$clt_port\n";

    my $request = "";
    while (<$clt>) {
        $request .= $_;
        last if $request =~ /\r\n\r\n$/;
    }

    my @dirs = split /\r\n/, $request;

    for my $dir (@dirs) {
        if (opendir my $dirent, $dir) {
            my @files = readdir $dirent;
            closedir $dirent;

            print $clt "Contents of $dir:\r\n" . join("\r\n", @files) . "\r\n\r\n";
        }
        else {
            print $clt "Failed to open $dir: $!\r\n\r\n";
        }
    }

    close $clt;
}