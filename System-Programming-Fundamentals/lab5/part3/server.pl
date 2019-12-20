#!/usr/bin/perl -T
use strict;
use warnings qw(FATAL all);
use IO::Socket::UNIX;
use English;
$ENV{'PATH'} = '/usr/bin';

my @gr_list = split(/ /, $GID);
my $gid = $gr_list[0];
my $start_time = time();
my $run_time;
my @sys_load;
my $SOCKET_PATH = "LAB5_NDK_SOCKET";

sub update_info {
    $run_time = time() - $start_time;
    @sys_load = split(/load average: /, qx(uptime));
    @sys_load = split(", ", $sys_load[1]);
}

print "pid = $PID\n";
print "uid = $UID\n";
print "gid = $gid\n";
print "socket name: $SOCKET_PATH\n\n";
print("Server is running...\n");
unlink $SOCKET_PATH;
my $server = IO::Socket::UNIX->new(
    Type   => SOCK_STREAM(),
    Local  => $SOCKET_PATH,
    Listen => 1,
) or die "Cannot create a socket $SOCKET_PATH\n";

update_info;
while (my $conn = $server->accept()) {
    update_info;
    $conn->print("pid = $PID\n");
    $conn->print("uid = $UID\n");
    $conn->print("gid = $gid\n");
    $conn->print("time = $run_time\n");
    $conn->print("average system load in 1min = $sys_load[0]\n");
    $conn->print("average system load in 5min = $sys_load[1]\n");
    $conn->print("average system load in 15min = $sys_load[2]\n");
    $conn->close();
}

use sigtrap handler => \&signal_handler, qw(HUP INT TERM USR1 USR2);

sub signal_handler {
    $run_time = time() - $start_time;
    print "Signal received: $_[0]\n";
    print "time = $run_time\n";
    print "pid = $PID\n";
    print "uid = $UID\n";
    print "gid = $gid\n";
    print "average system load in 1min = $sys_load[0]\n";
    print "average system load in 5min = $sys_load[1]\n";
    print "average system load in 15min = $sys_load[2]\n";
    unlink $SOCKET_PATH;
    exit(0);
}