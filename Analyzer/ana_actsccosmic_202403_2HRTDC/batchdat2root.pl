#!/usr/bin/perl

#use strict;
#use warnings;

my ($startrun,$endrun,$forceflag) = @ARGV;

if ((not defined $startrun)||(not defined $endrun)) {
    die "Usage: batchdat2root.pl startrun endrun (forceflag)\n";
}

$indir = "./data";

print "startrun = $startrun\n";
print "endrun = $endrun\n";

$nsubrun=10;

for (my $run=$startrun;$run<=$endrun;$run++) {

    for (my $subrun=0;$subrun<$nsubrun;$subrun++) {
	$infile = sprintf("%s/run%06d_%d.dat",$indir,$run,$subrun);
	$outfile = sprintf("%s/run%06d_%d.root",$indir,$run,$subrun);

	if (-e $infile) {
	    if ((defined $forceflag)||((not defined $forceflag) && (!(-e $outfile)))) {
		$command = sprintf("nice /home/sako/DRS4_HRTDC_202402/Analyzer/dat2root.pl %d %d",$run,$subrun);
		print "$command\n";
		system($command);
	    }
	} else {
	    break;
	}
    }
    
}


