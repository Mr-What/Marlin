union() {

  // tower aligned rods to hexagon edge is 110
  for (a=[0,120,240]) rotate([0,0,a])
    translate([0,39,0]) cube([8,62,4],center=true);
    //translate([0,15,0]) cube([8,110,4],center=true); // rods even with hex outside

  difference() {
    cube([80/cos(30)   ,80   ,4],center=true);
    cube([80/cos(30)-12,80-12,6],center=true);
  }

  translate([0,0,-2]) difference() {
    cylinder(h=4,r=40/cos(30),$fn=6);
    translate([0,0,-1])cylinder(h=6,r=34/cos(30),$fn=6);
  }

  difference() {
    cube([20/cos(30),20,4],center=true);
    translate([0,0,-3]) #cylinder(h=6,r=5/cos(30),$fn=6);  // 1cm high hex
  }
}

%translate([0,0,-1]) rotate([0,0,30]) cylinder(h=6,r=70,$fn=6);
