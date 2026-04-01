#!/usr/bin/perl

#use strict;
#use warnings;

my ($startrun,$endrun,$forceflag) = @ARGV;

if ((not defined $startrun)||(not defined $endrun)) {
    die "Usage: batchana.pl startrun endrun (forceflag)\n";
}

$indir = "./data";

print "startrun = $startrun\n";
print "endrun = $endrun\n";

$nsubrun=10;

$command1 = "nice ./batchdat2root.pl $startrun $endrun $forceflag";
print "$command1\n";
system($command1);

$command2 = "nice ./batchcheckrun0e.pl $startrun $endrun";
print "$command2\n";
system($command2);

#$command3 = "nice ./batchanarun0e.pl $startrun $endrun";
#print "$command3\n";
#system($command3);


