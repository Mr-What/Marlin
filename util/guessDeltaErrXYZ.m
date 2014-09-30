% Given a set of measurements of the print bed level and
% a standard test print, guess the individual tower radii,
% delta rod length, endstop offsets, and printer spread that was most
% likely to have caused this distortion.
%
% Assumes delta bed coordinates are:
%
%      +Y                       3(RAMPS-Z)
%       ^                          X
%       |  Card coords            / \          Tower name/number
%       |                        /   \
%       +-->+X       (RAMPS-X)1 +-----+ 2 (RAMPS-Y)
%
%
% DeltaParams(DP) struct must contain:
%      radius(3) -- Marlin DELTA_RADIUS, which is radius from tip to center
%                   of tower pivot for diagonal arm, minus effector offset
%                   (kind of a radius - effector_offset)
%      RodLen    -- length between center of pivots on diagonal rods
%      XYcal     -- test print definitions, from loadXYcalDef()
%      meas      -- measurements of test print, from loadXYcalMeas(XYcal,...)
%      bed       -- bed measurements, adjusted for probe offset
%
% RETURN:  values to SUBTRACT from :
%             towerZErr -- endstop offsets
%             radiusErr -- DELTA_RADIUS1,2,3 settings
%             diagErr   -- diagonal rod length (RodLen)
%    also...
%             spread    -- estimate of printer spread (mm)
function [towerZErr, radiusErr, diagErr, spread] = guessDeltaErrXYZ(DP)
DP.verbose=1;  % set desired diagnostic verbosity

% initial data plot
figure(2);
hold off;
[c,ax,pFit] = plotParabolicFit(DP.bed.xyz);
grid on;hold on;
plot3(DP.bed.xyz(:,1),DP.bed.xyz(:,2),DP.bed.xyz(:,3),'+');
title('Parabolic fit to measurements, + is measurements, . are fit points');
pause(0.1); % forces plot to display, so we can review it while computing

[dErr,nEval,status,err] = SimplexMinimize(...
              @(p) deltaGuessErrXYZ(p,DP),...
              [0 0 0 0 0 0 0 0.1], ... % seed
	      [1 1 1 1 1 1 1 0.3]*0.1, ... % initial step
	      [1 1 1 1 1 1 1 0.5]*0.005, 999) % close enough, max iterations
towerZErr = dErr(1:3);
radiusErr = dErr(4:6);
diagErr   = dErr(7);
spread    = dErr(8);

% plot delta parameter fit
errZ = deltaErrZ(dErr,DP);
fm = DP.bed.xyz; fm(:,3) = fm(:,3)+errZ;
plot3(fm(:,1),fm(:,2),fm(:,3),'r.');
xlabel('X(mm)');ylabel('Y(mm)');zlabel('mm');
hold off

figure(3);
hold off;
c = plotParabolicFit(fm);
grid on;hold on;
plot3(fm(:,1),fm(:,2),fm(:,3),'+');
hold off;
title('Parabolic Fit to simulated points');
xlabel('X');ylabel('Y');

figure(1);
hold off
plot3(fm(:,1),fm(:,2),fm(:,3)*1000,'rx');
#plotParabolicFit(fm);
grid on;hold on;
plot3(fm(:,1),fm(:,2),DP.bed.xyz(:,3)*1000,'+');
legend('Fit','Measured');
xlabel('X(mm)');ylabel('Y(mm)');zlabel('Z(um)');
hold off

% need to add some sort of XY distortion plot
errXY0   = deltaErrXY([0 0 0 0 0 0 0 dErr(8)],DP);
disp(sprintf('Initial XY RMSE : %.3f',sqrt(mean(errXY0 .^ 2))));
errXYfit = deltaErrXY(dErr,DP);
disp(sprintf('    fit XY RMSE : %.3f',sqrt(mean(errXYfit .^ 2))));
end

% Error metric for minimization
function err = deltaGuessErrXYZ(p,DP)
err = deltaErrXY(p,DP);
errXY = mean(err .* err);
err = deltaErrZ(p,DP);
errZ = mean(err .* err);
disp(sqrt([errXY,errZ]));
% may want to weight one type of error over another
err = (errXY^2) * errZ;
end

