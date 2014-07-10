#!/usr/bin/perl -w
# Given a stream of Marlin responses, extract extruder positions
# as clean data for Excell or Octave(MATLAB)
while(<STDIN>) {
    if (m/^X:/) {
	s/ E:.*$//;
	s/[XYZ:]+//g;
	print;
    }
}
