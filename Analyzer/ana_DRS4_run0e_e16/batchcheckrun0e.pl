#!/usr/bin/perl

#use strict;
#use warnings;

my ($startrun,$endrun) = @ARGV;

if ((not defined $startrun)||(not defined $endrun)) {
    die "Usage: batchcheckactsc.pl startrun endrun\n";
}

$indir = "./data";
$outdir = "./ana/check_drs4_run0e_e16";
$comdir = "/home/mrpcdaq/DRS4_HRTDC_run0e_e16/Analyzer/ana_DRS4_run0e_e16";

print "startrun = $startrun\n";
print "endrun = $endrun\n";

for (my $run=$startrun;$run<=$endrun;$run++) {
    $infile = sprintf("%s/run%06d_0.root",$indir,$run);
    print "infile=$infile\n";
    $outfile = sprintf("%s/check_drs4_run0e_run%d.root",$outdir,$run);
    print "outfile=$outfile\n";
    if (-e $infile) {

	$command = sprintf("nice %s/check_drs4_run0e_e16 %d",$comdir,$run);
	
	print "$command\n";
	system($command);
    }
    
}


