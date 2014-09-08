% Given a grid of tower positions, compute associated cartesian positions
function xyz = cartGrid(DP,tz)
  xyz = tz;
  s = size(tz);
  for i=1:s(1)
    for j=1:s(2)
      if (prod(tz(i,j,:)) > 1)
        c = delta2cart(DP,tz(i,j,1),tz(i,j,2),tz(i,j,3));
        xyz(i,j,:) = c;
      end
    end
  end
end
