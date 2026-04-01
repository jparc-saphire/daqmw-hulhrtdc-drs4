#!/usr/bin/perl

#use strict;
#use warnings;

my ($startrun,$endrun) = @ARGV;

if ((not defined $startrun)||(not defined $endrun)) {
    die "Usage: batchcheckhrtdc5.pl startrun endrun\n";
}

$indir = "/disk1/mrpcdaq/data/dat_DRS4_HRTDC_run0e_e16";
$outdir = "/home/mrpcdaq/DRS4_HRTDC_run0e_e16/Analyzer/ana_DRS4_run0e_e16/ana/check_drs4_run0e_e16";
$comdir = "/home/mrpcdaq/DRS4_HRTDC_run0e_e16/Analyzer/ana_DRS4_run0e_e16";
$trigdir = "/disk1/mrpcdaq/trig_data";

print "startrun = $startrun\n";
print "endrun = $endrun\n";

for (my $run=$startrun;$run<=$endrun;$run++) {
    $trigfile = sprintf("%s/run%06d_trg.root",$trigdir,$run);
    $infile = sprintf("%s/check_drs4_run0e_run%d.root",$outdir,$run);
    print "infile=$infile\n";
    $outfile = sprintf("%s/check_drs4_run0e_m_run%d.root",$outdir,$run);
    print "outfile=$outfile\n";
    if (-e $infile) {
	if (!(-e $outfile)) {
	    if (!(-e $trigfile)) {
		print "******** trigdata $trigfile does not exist. exiting\n";
		exit(-1);
	    }

	    $command = sprintf("nice %s/merge_mrpc_trig %d",$comdir,$run);
	    
	    print "$command\n";
	    system($command);
	}
	
    } else {
	print "$infile does not exist\n";
    }
    
    
}


