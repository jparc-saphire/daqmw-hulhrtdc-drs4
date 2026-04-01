#!/usr/bin/perl
my ($run,$subrun,$event) = @ARGV;


if (not defined $run) {
        die "Usage: dat2root.pl run\n";
}

$nevent = 10000000;
if (defined $event) {
    $nevent = $event;
}
$datadir = "./data";
$anadir = "/home/mrpcdaq/DRS4_HRTDC_run0e_e16/Analyzer";
chdir($anadir);
if (not defined $subrun) {
    $command = sprintf("./bin/analysMonitor conf/analyzer.conf.default $datadir/run%06d_0.dat $datadir/run%06d_0.root %d",$run,$run,$nevent);
} else {
    $command = sprintf("./bin/analysMonitor conf/analyzer.conf.default $datadir/run%06d_%d.dat $datadir/run%06d_%d.root %d",$run,$subrun,$run,$subrun,$nevent);
}

print "command=$command\n";

system($command);

