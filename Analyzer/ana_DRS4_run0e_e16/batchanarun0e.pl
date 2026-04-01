#!/usr/bin/perl

#use strict;
#use warnings;

my ($startrun,$endrun,$forceflag) = @ARGV;

if ((not defined $startrun)||(not defined $endrun)) {
    die "Usage: batchanarun0e.pl startrun endrun (forceflag)\n";
}

$indir = "/home/mrpcdaq/DRS4_HRTDC_run0e_e16/Analyzer/ana_DRS4_run0e_e16/ana/check_drs4_run0e_e16";
$outdir = "/home/mrpcdaq/DRS4_HRTDC_run0e_e16/Analyzer/ana_DRS4_run0e_e16/ana/ana_run0e_e16";
$comdir = "/home/mrpcdaq/DRS4_HRTDC_run0e_e16/Analyzer/ana_DRS4_run0e_e16";

print "startrun = $startrun\n";
print "endrun = $endrun\n";

for (my $run=$startrun;$run<=$endrun;$run++) {
    $infile = sprintf("%s/check_run0e_e16_run%d.root",$indir,$run);
    print "infile=$infile\n";
    $outfile = sprintf("%s/ana_run0e_e16_run%d.root",$outdir,$run);
    print "outfile=$outfile\n";
    if (-e $infile) {
#	if ((defined $forceflag)||((not defined $forceflag) && (!(-e $outfile)))) {

	    $command = sprintf("nice %s/ana_run0e_e16 %d %s %s",$comdir,$run,$infile,$outfile);
	    
	    print "$command\n";
	    system($command);
#	}
	
    }
    
}


