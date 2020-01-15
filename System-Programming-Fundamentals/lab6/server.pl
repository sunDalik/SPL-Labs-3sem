#!/usr/bin/perl

use strict;
use warnings qw(FATAL all);
use threads;
use threads::shared;
use IO::Socket::INET;
use Class::Struct;
use Time::HiRes qw(usleep);

my $USAGE = "Usage $0 port [thread_count]\n";
scalar @ARGV or die $USAGE;
my ($port) = shift(@ARGV) =~ /(\d+)/ or die $USAGE;

my $thread_count = 2;
if (scalar @ARGV) {
    ($thread_count) = shift(@ARGV) =~ /(\d+)/ or die $USAGE;
}

struct(server_thread => {
    worker => '$',
    busy   => '$',
    client => '$',
    id     => '$',
});

my @thread_pool :shared;
for (my $i = 0; $i < $thread_count; $i++) {
    $thread_pool[$i] = share(server_thread->new());
    $thread_pool[$i]->busy(0);
    $thread_pool[$i]->client(-1);
    $thread_pool[$i]->id($i);
    $thread_pool[$i]->worker(threads->create(\&worker, $i)->detach());
}

my $server = IO::Socket::INET->new(
    LocalPort => $port, Proto => 'tcp', Listen => SOMAXCONN
) or die "Failed to start the server: $!\n";

print "Listening on $port\n";

while (1) {
    if ((my $free_thread_id = get_free_thread()) != -1) {
        my $clt = $server->accept();
        $thread_pool[$free_thread_id]->client($clt);

    }
    usleep(1000);
}

sub get_free_thread {
    for (my $i = 0; $i < $thread_count; $i++) {
        if ($thread_pool[$i]->busy == 0) {
            $thread_pool[$i]->busy(1);
            return $i;
        }
    }
    return -1;
}

sub worker {
    my $id = shift @_;
    my $meta = $thread_pool[$id];
    while (1) {
        while (1) {
            if ($meta->busy == 1 && $meta->client != -1) {
                last;
            }
            usleep(5000);
        }
        my $clt = $meta->client;
        my $clt_addr = $clt->peerhost;
        my $clt_port = $clt->peerport;
        print "Thread #$id: client connected: $clt_addr:$clt_port\n";

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
        $meta->client(-1);
        $meta->busy(0);
    }
}