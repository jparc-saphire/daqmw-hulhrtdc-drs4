#!/usr/bin/perl

#use strict;
#use warnings;

my ($startrun,$endrun) = @ARGV;

if ((not defined $startrun)||(not defined $endrun)) {
    die "Usage: batchcheckactsc.pl startrun endrun\n";
}

$indir = "./data";
$outdir = "./ana/check_ac";
$comdir = "/home/mrpcdaq/2021_tsukui/DRS4_HRTDC_201909/Analyzer/ana_run0d_drs4_20230616";

print "startrun = $startrun\n";
print "endrun = $endrun\n";

for (my $run=$startrun;$run<=$endrun;$run++) {
    $infile = sprintf("%s/run%06d_0.root",$indir,$run);
    print "infile=$infile\n";
    $outfile = sprintf("%s/check_ac_run%d.root",$outdir,$run);
    print "outfile=$outfile\n";
    if (-e $infile) {
#	if (!(-e $outfile)) {

	    $command = sprintf("nice %s/check_ac %d",$comdir,$run);
	    
	    print "$command\n";
	    system($command);
#	}
	
    }
    
}


