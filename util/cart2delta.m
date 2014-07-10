% convert cartesian coords to delta-bot displacements.
% Assumes delta bed coordinates are:
%
%      +Y                       3(RAMPS-Z)
%       ^                          X
%       |  Card coords            / \          Tower name/number
%       |                        /   \
%       +-->+X       (RAMPS-X)1 +-----+ 2 (RAMPS-Y)
%
%
% DeltaParams struct must contain:
%       RADIUS   -- Marlin DELTA_RADIUS, which is radius from tip to center
%                   of tower pivot for diagonal arm, minus effector offset
%                   (kind of a radius - effector_offset)
%       RodLen   -- length between center of pivots on diagonal rods
function delta = cart2delta(DeltaParams,x,y,z)

s = 0.8660254037844386; % sind(60)
c = 0.5;                % cosd(60)
T1x = -s * (DeltaParams.RADIUS+.6);  % current build has 1 thin carriage
T1y = -c * (DeltaParams.RADIUS+.6);
T2x =  s * DeltaParams.RADIUS;
T2y = -c * DeltaParams.RADIUS;
T3x =    0;
T3y =      DeltaParams.RADIUS;

dx1 = T1x - x;
dy1 = T1y - y;
dx2 = T2x - x;
dy2 = T2y - y;
dx3 = T3x - x;
dy3 = T3y - y;
d2 = DeltaParams.RodLen * DeltaParams.RodLen;
delta = sqrt(d2 - [dx1*dx1 + dy1*dy1,...
		   dx2*dx2 + dy2*dy2,...
		   dx3*dx3 + dy3*dy3]) + z;
end

