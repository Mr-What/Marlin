% $Header: /home/cvs/DETEC/BRDF/MatLab/SimplexMinimize.m,v 1.5 2006/09/27 21:13:37 aaron Exp $
%
% Implementation of the Simplex minimization technique.
% Based off of the code in Numerical Recipes,
% but generalized

function [fit,nEval,status,err] = SimplexMinimize(fcn,...
       initialGuess,initialStep,smallBox,maxIter)

status = 0;  % Normal completion
nDim = prod(size(initialGuess));

% initialize simplex
simplex.p = initialGuess;
simplex.y = 0;
x = simplex;
for i=2:nDim+1;
    %simplex = [simplex x]; % expand simplex vector
    simplex(i).p = initialGuess;
end
for i=1:nDim;
    simplex(i).p(i) = simplex(i).p(i) + initialStep(i);
end

% get value of errFunc at each simplex point
for i=1:nDim+1;
    simplex(i).y = feval(fcn,simplex(i).p);
end
nEval = nDim+1;  % initialize function evaluation counter

% initialize variable to store sum of simplex point values
psum = simplex(nDim+1).p;
for i=1:nDim;
    psum=psum+simplex(i).p;
end

% evaluation counter when last new low found
lastNewLow = nEval;
while(1)
    % find two worst points, and best point
    [ilo,ihi,inhi] = getExtremaIndices([simplex.y]);

    % check exit condition(s)
    if (SimplexExitCriteriaMet(simplex,smallBox,nEval-lastNewLow) || (nEval > maxIter))
	    fit = simplex(ilo).p;
        err = simplex(ilo).y;
        if (nEval > maxIter)
           status = -1;  % did not converge
	    end
	    return;
    end
    
    [simplex,ytry,psum] = SimplexUpdate(simplex,fcn,ihi,-1.0);
    nEval = nEval + 1;
    if (ytry <= simplex(ilo).y)
	    % found a new best point, try to go in that direction even farther
	    [simplex,ytry,psum] = SimplexUpdate(simplex,fcn,ihi,2.0);
	    nEval = nEval + 1;
        lastNewLow = nEval;
    elseif (ytry >= simplex(inhi).y)
	    % reflected point was worse than second-highest, so do
	    % a 1-dimensional contraction
	    ysave = simplex(ihi).y;
	    [simplex,ytry,psum] = SimplexUpdate(simplex,fcn,ihi,0.5);
	    if (ytry >= ysave)
	        % did not get rid of highest point,
	        % try contracting around lowest(best) point
	        for i=1,nDim+1;
		        if (i ~= ilo)
		            simplex(i).p = 0.5 * (simplex(i).p + simplex(ilo).p);
		            simplex(i).y = feval(fcn,simplex(i).p);
		            nEval = nEval + 1;
		        end
	        end

	        % update simplex sum
	        psum = simplex(nDim+1).p;
	        for i=1:nDim;psum=psum+simplex(i).p;end
	    end
    end
    %nEval % debug output
    %pause;
end

% $Log: SimplexMinimize.m,v $
% Revision 1.5  2006/09/27 21:13:37  aaron
% trying to get rid of ans = output on some runs
%
% Revision 1.4  2006/08/18 11:39:41  aaron
% cleaning up debug for production runs
%
% Revision 1.3  2006/06/28 22:36:45  aaron
% return err too
%
% Revision 1.2  2006/06/27 22:14:55  aaron
% running, but needs speed and robustness help
%
% Revision 1.1  2006/05/25 21:57:19  aaron
% *** empty log message ***
