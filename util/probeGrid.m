% given 37 bed-level probe readings, construct a regular grid, which is
% convenient for surface plots.
function [x,y,z] = probeGrid(a)
dx = abs(a(1,1)-a(2,1));
dy = abs(a(4,2)-a(3,2));
i=[-3:3];
z = zeros(7);
x = ones(7,1) * i;
y = x' * dy;
x = x  * dx;
x(1,3:5) = a(3:-1:1,1)';
y(1,3:5) = a(3:-1:1,2)';
z(1,3:5) = a(3:-1:1,3)';

x(2,2:6) = a(4:8,1)';
y(2,2:6) = a(4:8,2)';
z(2,2:6) = a(4:8,3)';

x(3,:) = a(15:-1:9,1)';
y(3,:) = a(15:-1:9,2)';
z(3,:) = a(15:-1:9,3)';

x(4,:) = a(16:22,1)';
y(4,:) = a(16:22,2)';
z(4,:) = a(16:22,3)';

x(5,:) = a(29:-1:23,1)';
y(5,:) = a(29:-1:23,2)';
z(5,:) = a(29:-1:23,3)';

x(6,2:6) = a(30:34,1)';
y(6,2:6) = a(30:34,2)';
z(6,2:6) = a(30:34,3)';

x(7,3:5) = a(37:-1:35,1)';
y(7,3:5) = a(37:-1:35,2)';
z(7,3:5) = a(37:-1:35,3)';
end

