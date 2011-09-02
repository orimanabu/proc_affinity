#!/usr/bin/env perl

use strict;

open FILE, "ps axh |" or die;
print "\"pid\",";
system("./proc_affinity --header-only --quote");
print "\"command\"\n";

while (<FILE>) {
	my ($pid, $tty, $status, $time, $command) = /^\s*(\d+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(.*)$/;
#	print "$pid\t$command\n";
	print "\"$pid\",";
	system("./proc_affinity --csv --quote $pid");
	print "\"$command\"\n";
}
