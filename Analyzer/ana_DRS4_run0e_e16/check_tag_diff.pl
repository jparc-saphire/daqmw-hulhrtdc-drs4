#!/usr/bin/perl

my ($run) = @ARGV;

if (not defined $run) {
    die "Usage: check_tag_diff.pl run\n";
}

$command = sprintf("diff ./ana/trig_data/check_tag_trig_run%05d.dat  ./ana/trig_data/check_tag_HRTDC_run%05d.dat  > ./ana/trig_data/diff_tag_run%05d.dat", $run,$run,$run);

print "$command\n";
system($command);

$command2 = sprintf("grep '<' ./ana/trig_data/diff_tag_run%05d.dat | wc -l", $run);

print "$command2\n";
$diffcount = `$command2`;

print "diffcount = $diffcount\n";

