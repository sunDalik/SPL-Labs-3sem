#!/usr/bin/perl -T
use strict;
use warnings qw(FATAL all);

$ENV{PATH} = '/usr/bin';
$ENV{ENV}="";

my $filename = shift @ARGV or die "Usage: $0 file\n";

open my $fd, '<', $filename or die "Can't open $filename!\n";
pipe my $pipe_fd_rd, my $pipe_fd_wr;

my $pid = fork();
if ($pid == 0) {
    close $pipe_fd_wr;
    open STDIN, '<&', $pipe_fd_rd;
    exec "wc";
}
else {
    close $pipe_fd_rd;
    my $file_char;
    while (read $fd, $file_char, 1) {
        print $pipe_fd_wr $file_char;
        read $fd, $file_char, 1;
    }
    close $pipe_fd_wr;
}
close($fd);