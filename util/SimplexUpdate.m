% $Project: BRDFToolSource$     $Folder: BRDFfit$
% $Header: /home/cvs/DETEC/BRDF/MatLab/SimplexUpdate.m,v 1.2 2006/08/18 11:39:41 aaron Exp $
%
% Update a simplex for SimplesMinimize

function [simplex,ytry,psum] =  SimplexUpdate(simplex,fcn,ihi,fac)

nDim = length(simplex)-1;

psum = simplex(nDim+1).p;
for i=1:nDim;
    psum = psum + simplex(i).p;
end

fac1 = (1.0-fac)/nDim;
fac2 = fac1 - fac;

ptry = psum*fac1 - (simplex(ihi).p * fac2);
ytry = feval(fcn,ptry);

if (ytry < simplex(ihi).y)
    % this is better than current highest, so replace it.
    psum = psum + ptry - simplex(ihi).p;
    simplex(ihi).y = ytry;
    simplex(ihi).p = ptry;
    %ptry % debug output
    %ytry
end

% $Log: SimplexUpdate.m,v $
% Revision 1.2  2006/08/18 11:39:41  aaron
% cleaning up debug for production runs
%
% Revision 1.1  2006/05/25 21:57:19  aaron
% *** empty log message ***
%
% Revision 1.1  2003/07/21 20:20:06  birenaa
% moving from RCS to CVS
%
% Revision 1.1  2003/07/09 23:30:50  birenaa
% Initial revision
%
