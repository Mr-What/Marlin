% convert delta tower positions to effector cartesian coords
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
% DeltaParams can contain these for distortions:
%       radiusErr -- Tower radius error, each tower
function cart = delta2cart(DP,T1z,T2z,T3z)
DeltaParams = DP;
DeltaParams.radiusErr = getFieldDef(DP,'radiusErr',[0,0,0]);
s = 0.8660254037844386; % sind(60)
c = 0.5;                % cosd(60)
T1x = -s * (DeltaParams.RADIUS + DeltaParams.radiusErr(1));
T1y = -c * (DeltaParams.RADIUS + DeltaParams.radiusErr(1));
T2x =  s * (DeltaParams.RADIUS + DeltaParams.radiusErr(2));
T2y = -c * (DeltaParams.RADIUS + DeltaParams.radiusErr(2));
T3x =    0;
T3y =       DeltaParams.RADIUS + DeltaParams.radiusErr(3);

DeltaParams.tower = [T1x T1y T1z; T2x T2y T2z; T3x T3y T3z];
zMin = sqrt(DeltaParams.RodLen ^ 2 - 4*(DeltaParams.RADIUS ^2));
if ((T1z < zMin) || (T2z < zMin) || (T3z < zMin))
   %disp('Non-Physical');disp([T1z T2z T3z zMin]);
   cart = [0,0,0];
   return
end

% ? add center err here to test?
R2 = DP.RodLen * DP.RodLen;

% try iterative soln method
x = 0;
y = 0;
z = 0;
sd = 99;
bestSD = 999;
cart = [x y z];
i=0;
updateGain = 1.0;
while (sd > 0.002)
   z1 = deltaQuadSln(R2,T1z,T1x,T1y,x,y);
   z2 = deltaQuadSln(R2,T2z,T2x,T2y,x,y);
   z3 = deltaQuadSln(R2,T3z,T3x,T3y,x,y);
   [z,sdz] = updateDeltaGuess([z1 z2 z3],updateGain,z);
   [a,y1] = deltaQuadSln(R2,T1y,T1x,T1z,x,z);
   [a,y2] = deltaQuadSln(R2,T2y,T2x,T2z,x,z);
   [y3,a] = deltaQuadSln(R2,T3y,T3x,T3z,x,z);
   [y,sdy] = updateDeltaGuess([y1 y2 y3],updateGain,y);
   [ a , x1] = deltaQuadSln(R2,T1x,T1y,T1z,y,z);
   [x2 , a ] = deltaQuadSln(R2,T2x,T2y,T2z,y,z);
   [x3a,x3b] = deltaQuadSln(R2,T3x,T3y,T3z,y,z);
   % the x ordinate on tower 3 could be on either side of the tower.
   % this is the only tower where this happens, so we need to try each side
   [xa,sda] = updateDeltaGuess([x1 x2 x3a],updateGain,x);
   [xb,sdb] = updateDeltaGuess([x1 x2 x3b],updateGain,x);
   if (sda < sdb)
      sdx = sda;
      x=xa;
   else
      sdx = sdb;
      x=xb;
   end
   i=i+1;
   sd = sdx + sdy + sdz;
   if (sd < bestSD)
      bestSD = sd;
      cart = [x y z];
      %disp(sprintf('%d %.3f [%.3f,%.3f,%.3f]',i,sd,cart));
   end
   if (i == 6)
      %disp('Initial guess complete, try simplex search');
      % use anonymous function to pass structure of 'fixed' parameters
      % to deltaErrFcb
      %disp(cart);
      [cart,nEval,status,err] = SimplexMinimize(...
              @(p) deltaErrFcn(p,DeltaParams),...
   	      cart, 0.1+[0 0 0], 0.0001+[0 0 0], 300);
      if (err < 0.0001), return; end
      x=cart(1);y=cart(2);z=cart(3);
      updateGain=1.0;
   end
   if (i == 30)
      %disp('Slow Convergence, try simplex search');
      % use anonymous function to pass structure of 'fixed' parameters
      % to deltaErrFcb
      %disp(cart);
      [cart,nEval,status,err] = SimplexMinimize(...
              @(p) deltaErrFcn(p,DeltaParams),...
	      cart, 0.1+[0 0 0], 0.0001+[0 0 0], 300)
      if (err < 0.0001), return; end
      x=cart(1);y=cart(2);z=cart(3);
      updateGain=1.0;
   end
   if (i==48), updateGain=0.7; end
   if (i > 60)
      disp('Non Convergence');
      disp([T1z T2z T3z])
      disp([x1 x2 x3a x3b])
      disp([y1 y2 y3])
      disp([z1 z2 z3])
      sd=0;x=0;y=0;z=0;
   end
   if (i==8 ), updateGain=0.8; end  % slow down after a few steps
   if (i==20), updateGain=0.6; end  % slow down after a few steps
end
%disp([i,cart]);
end

function [za,zb] = deltaQuadSln(R2,T1z,T1x,T1y,x,y)
dx = T1x - x;
dy = T1y - y;
disc = R2 - dx*dx - dy*dy;
zb = T1z + sqrt(disc);
za = T1z - sqrt(disc);
end

function [y,sd] = updateDeltaGuess(x,gain,x0);
nGood = 0;
y = [];
for i=1:length(x)
   if (imag(x(i)) == 0)
      nGood = nGood + 1;
      y(nGood) = real(x(i));
   end
end
sd = 69;
if (nGood == 0)
   y = mean(real(x));
   sd = 99;
elseif (nGood > 1)
   sd = std(y);
   y = mean(y);
end
if (gain < 1.0)
   t = gain * y + (1.0-gain) * x0;
end
end

% in case we fall-back to a simplex minimization
function err = deltaErrFcn(p,DP)
%global DeltaParams
v1 = DP.tower(1,:) - p;
v2 = DP.tower(2,:) - p;
v3 = DP.tower(3,:) - p;
e1 = norm(v1) - DP.RodLen;
e2 = norm(v2) - DP.RodLen;
e3 = norm(v3) - DP.RodLen;
err = e1*e1 + e2*e2 + e3*e3;
end
