#!/usr/bin/perl -T

use strict;
use warnings qw(FATAL all);

$ENV{PATH} = '/usr/bin';
$ENV{CDPATH}="";
$ENV{ENV}="";
$ENV{BASH_ENV} = '/usr/share/Modules/init/bash';

my $filename = shift @ARGV;
die "Usage: $0 file\n" if !$filename;

open my $fd, '<', $filename or die "Can't open $filename!\n";
pipe my $pipefd_rd, my $pipefd_wr or die "Can't create a pipe!\n";

my $pid = fork();
if (!$pid) {
    close $pipefd_wr or die "Can't close parent's end of the pipe!\n";
    open STDIN, '<&', $pipefd_rd or die "Can't replace STDIN with the pipe!\n";

    exec "wc -c" or die "Can't execute wc!\n";
}
else {
    close $pipefd_rd or die "Can't close child's end of the pipe!\n";

    my $buf = "";
    while (read $fd, $buf, 2) {
        print $pipefd_wr substr($buf, 1, 1);
    }
}