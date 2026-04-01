#!/usr/bin/perl

#use strict;
#use warnings;

my ($startrun,$endrun) = @ARGV;

if ((not defined $startrun)||(not defined $endrun)) {
    die "Usage: batchchecktag.pl startrun endrun\n";
}

$indir = "/disk1/mrpcdaq/data/dat_DRS4_HRTDC_run0e_e16";

print "startrun = $startrun\n";
print "endrun = $endrun\n";
$trigdir = "/disk1/mrpcdaq/trig_data";
for (my $run=$startrun;$run<=$endrun;$run++) {
    $infile = sprintf("%s/run%06d_0.dat",$indir,$run);
    $outfile = sprintf("%s/run%06d_0.root",$indir,$run);

    $mergefile = sprintf("./ana/check_drs4_run0e_e16/check_drs4_run0e_m_run%05d.root", $run);
    $trigfile = sprintf("%s/run%06d_trg.root",$trigdir,$run);

    if (!(-e $trigfile)) {
	print "Trig file $trigfile does not exist. Exiting...\n";
	exit(-1);
    }
    if (!(-e $mergefile)) {
	print "Merge file $mergefile does not exist. Exiting...\n";
	exit(-1);
    }
    
    $command1 = sprintf("nice /home/mrpcdaq/DRS4_HRTDC_run0e_e16/Analyzer/ana_DRS4_run0e_e16/check_tag_trig %d",$run);
    print "$command1\n";
    system($command1);
    
    $command2 = sprintf("nice /home/mrpcdaq/DRS4_HRTDC_run0e_e16/Analyzer/ana_DRS4_run0e_e16/check_tag_HRTDC %d",$run);
    print "$command2\n";
    system($command2);

    $command3 = sprintf("nice /home/mrpcdaq/DRS4_HRTDC_run0e_e16/Analyzer/ana_DRS4_run0e_e16/check_tag_diff.pl %d",$run);
    print "$command3\n";
    system($command3);
    
}


