#!/usr/bin/perl

use strict;
use warnings qw(FATAL all);
use threads;
use threads::shared;
use IO::Socket::INET;
use Time::HiRes qw(usleep);

my $USAGE = "Usage $0 port [thread_count]\n";
scalar @ARGV or die $USAGE;
my ($port) = shift(@ARGV) =~ /(\d+)/ or die $USAGE;

my $thread_count = 2;
if (scalar @ARGV) {
    ($thread_count) = shift(@ARGV) =~ /(\d+)/ or die $USAGE;
}

my $busy_i :shared = 0;
my $client_i :shared = 1;
my $id_i :shared = 2;

my @thread_pool :shared;
for (my $i = 0; $i < $thread_count; $i++) {
    my @server_thread :shared;
    $server_thread[$busy_i] = 0;
    $server_thread[$client_i] = -1;
    $server_thread[$id_i] = $i;
    $thread_pool[$i] = \@server_thread;

    threads->create(\&worker, $i)->detach();
}

my $server = IO::Socket::INET->new(
    LocalPort => $port, Proto => 'tcp', Listen => SOMAXCONN
) or die "Failed to start the server: $!\n";

print "Listening on $port\n";

my @clients;
while (1) {
    if ((my $free_thread_id = get_free_thread()) != -1) {
        my $clt = $server->accept();
        push @clients, $clt;
        $thread_pool[$free_thread_id]->[$client_i] = fileno($clt);
    }
    usleep(1000);
}

sub get_free_thread {
    for (my $i = 0; $i < $thread_count; $i++) {
        if ($thread_pool[$i]->[$busy_i] == 0) {
            $thread_pool[$i]->[$busy_i] = 1;
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
            if ($meta->[$busy_i] == 1 && $meta->[$client_i] != -1) {
                last;
            }
            usleep(5000);
        }
        my $fn = $meta->[$client_i];

        open my $clt, '+<&=' . $fn or die $!;
        print "Thread #$id: client $fn connected!\n";
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
        $meta->[$client_i] = -1;
        $meta->[$busy_i] = 0;
    }
}