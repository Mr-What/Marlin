% Script to perform full delta printer calibration estimate,
% using both bed probe and test print measurements
%
% Given the set of measurements, and test object definition,
% estimates of :
%   - tower endstop error
%   - tower radii error
%   - diagonal rod length error
%   - print spread
% most likely to have produced these measurements are returned
%
%
%      deltaCalXYZ(rodLen,radii,xyPointsFile,xyPairsFile,xyMeasFile,bedMeas)
%
% RETURN:  values to SUBTRACT from :
%             towerZErr -- endstop offsets
%             radiusErr -- DELTA_RADIUS1,2,3 settings
%             diagErr   -- diagonal rod length (RodLen)
%    also...
%             spread    -- estimate of printer spread (mm)
%             [DP]      -- full structure of test data
function [towerZErr, radiusErr, diagErr, spread,DP] = ...
      deltaCalXYZ(rodLen,radii,xyPointsFile,xyPairsFile,xyMeasFile,bedMeas)
DP.verbose=1;  % set desired diagnostic verbosity
DP.RodLen = rodLen;
if (length(radii)==1), radii=radii*[1 1 1]; end
DP.radius = radii;
DP.XYcal = loadXYcalDef(DP,xyPointsFile,xyPairsFile);
DP.meas  = loadXYcalMeas(DP.XYcal,xyMeasFile);
DP.bed.xyz = bedMeas;
n=size(DP.bed.xyz,1);
DP.bed.twr = zeros(n,3);
for i=1:n, DP.bed.twr(i,:) = cart2delta(DP,[DP.bed.xyz(i,1:2),0]); end

[towerZErr, radiusErr, diagErr, spread] = guessDeltaErrXYZ(DP)
end