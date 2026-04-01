#!/usr/bin/perl
my ($run) = @ARGV;

if (not defined $run) {
        die "Usage: dat2root.py run\n";
}

$datadir = "./data";
$anadir = "/home/mrpcdaq/2021_tsukui/DRS4_HRTDC_201909/Analyzer";
chdir($anadir);
$command = sprintf("./bin/analysMonitor2 conf/analyzer.conf.default $datadir/run%06d_0.dat $datadir/run%06d_0.root",$run,$run);

print "$command\n";

system($command);

