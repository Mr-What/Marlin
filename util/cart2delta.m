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
function delta = cart2delta(DeltaParams,cart)

if (isfield(DeltaParams,'RADIUS'))
   radius = DeltaParams.RADIUS + [0 0 0];  # assumes equal radius
else
   radius = DeltaParams.radius;
end
s = 0.8660254037844386; % sind(60)
c = 0.5;                % cosd(60)
T1x = -s * radius(1);
T1y = -c * radius(1);
T2x =  s * radius(2);
T2y = -c * radius(2);
T3x = 0;
T3y = radius(3);

dx1 = T1x - cart(1);
dy1 = T1y - cart(2);
dx2 = T2x - cart(1);
dy2 = T2y - cart(2);
dx3 = T3x - cart(1);
dy3 = T3y - cart(2);
d2 = DeltaParams.RodLen * DeltaParams.RodLen;
delta = sqrt(d2 - [dx1*dx1 + dy1*dy1,...
		   dx2*dx2 + dy2*dy2,...
		   dx3*dx3 + dy3*dy3]) + cart(3);
end
