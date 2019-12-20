#!/usr/bin/perl -T

use strict;
use warnings qw(FATAL all);
use IO::Socket::UNIX;
use English;

$ENV{PATH} = '/usr/bin';
$ENV{CDPATH} = "";
$ENV{ENV} = "";
$ENV{BASH_ENV} = '/usr/share/Modules/init/bash';

my $SYSTEM_INFO_FMT = 'i<i<i<d<d<d<d<';
my $SOCKET_PATH = "LAB5_NDK_SOCKET";
use sigtrap "handler" => \&signal_handler, qw(HUP INT TERM USR1 USR2);

my $server = IO::Socket::UNIX->new(
    Type => SOCK_STREAM(), Local => $SOCKET_PATH, Listen => 1
) or die "Failed to create a socket: $!\n";

my $gid = (split ' ', $GID)[0];
print "Init parameters:\n";
print "pid = $PID\n";
print "uid = $UID\n";
print "gid = $gid\n\n";
print "socket name: $SOCKET_PATH\n\n";

# init server parameters
my ($s_time, $time_working) = (time(), time());
my ($l1, $l5, $l15) = (0, 0, 0);

print "Server is running...\n";

while (my $clt = $server->accept()) {
    $time_working = time() - $s_time;
    my $loads = `uptime | awk '{print \$(NF-2)\$(NF-1)\$NF}'`;
    $loads =~ /(\d+\.\d+),(\d+\.\d+),(\d+\.\d+)/;
    my $state = pack $SYSTEM_INFO_FMT, $PID, $UID, $gid, $time_working, $l1, $l5, $l15;

    $clt->print($state);
    $clt->close();
}

sub signal_handler {
    $time_working = time() - $s_time;
    my $loads = `uptime | awk '{print \$(NF-2)\$(NF-1)\$NF}'`;
    $loads =~ /(\d+\.\d+),(\d+\.\d+),(\d+\.\d+)/;

    print "Signal received: $_[0]\n";
    print "time = $time_working\n";
    print "pid = $PID\n";
    print "uid = $UID\n";
    print "gid = $gid\n";
    print "average system load in 1min = $l1\n";
    print "average system load in 5min = $l5\n";
    print "average system load in 15min = $l15\n";
    unlink $SOCKET_PATH;
    exit(0);
}