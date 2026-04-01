#!/usr/bin/perl

#use strict;
#use warnings;

my ($startrun,$endrun,$forceflag) = @ARGV;

if ((not defined $startrun)||(not defined $endrun)) {
    die "Usage: batchanarun0d1.pl startrun endrun (forceflag)\n";
}

$indir = "./ana/check_ac";
$outdir = "./ana/ana_run0d_drs4_1";
$comdir = "./";

print "startrun = $startrun\n";
print "endrun = $endrun\n";

for (my $run=$startrun;$run<=$endrun;$run++) {
    $infile = sprintf("%s/check_ac_run%d.root",$indir,$run);
    print "infile=$infile\n";
    $outfile = sprintf("%s/ana_run0d_drs4_1_run%d.root",$outdir,$run);
    print "outfile=$outfile\n";
    if (-e $infile) {
#	if ((defined $forceflag)||((not defined $forceflag) && (!(-e $outfile)))) {

	    $command = sprintf("nice %s/ana_run0d_drs4_1 %d %s %s",$comdir,$run,$infile,$outfile);
	    
	    print "$command\n";
	    system($command);
#	}
	
    }
    
}


