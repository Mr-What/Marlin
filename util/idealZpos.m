% compute ideal position of tower carriages for a given grid of points on the bed
function z = idealZpos(DP,x)
  x = x(:)';
  n = length(x);
  r2max = max(abs(x));
  r2max = r2max * r2max;
  z = zeros(n,n,3);
  for i=1:n
    for j=1:n
      if (x(i)*x(i) + x(j)*x(j) < r2max)
        d = cart2delta(DP,x(i),x(j),0);
        z(i,j,:) = d;
      end
    end
  end
end
