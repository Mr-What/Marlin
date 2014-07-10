% $Header: /home/cvs/DETEC/BRDF/MatLab/getExtremaIndices.m,v 1.1 2006/05/25 21:57:19 aaron Exp $
%
% Return the indices of the low, hi, and next-to-higest values in a
% vector.
%
function [lo,hi,nhi] = getExtremaIndices(y)

n = length(y);
[x,hi] = max(y);
[x,lo] = min(y);
t = y;
t(hi) = x-abs(x);
[x,nhi] = max(t);

% $Log: getExtremaIndices.m,v $
% Revision 1.1  2006/05/25 21:57:19  aaron
% *** empty log message ***
%
