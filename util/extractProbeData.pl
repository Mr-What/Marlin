#!/usr/bin/perl -w
#
# Pipe output from a Auto-bed-leveling probe to this script
# to extract clean probe numbers into a format readable
# by Octave (MATLAB) or Excell
while (<STDIN>) {
    chomp;
    if (m/Bed x: /) {
	s/^.*Bed x: //;
	s/y: //;
	s/z: //;
	print "$_\n";
    }
}
