union() {

  for (a=[0,120,240]) rotate([0,0,a]) {
    // tip 39+62/2 = 70 mm from exact center
    translate([0, 39,0]) cube([8,62,4],center=true);
    // outer edge aligned with center of tip of bar towards tower,
    // exactly 35mm from center
    translate([0,-32,0]) {
      cube([60,6,4],center=true);
      for (b=[-1,1]) 
        translate([28*b,8,0]) cube([4,15,4],center=true);
    }

    // more stuff to test Z scale
    // corners exactly 60 from center, on outside, 5mm above rest.
    translate([0,60-4/cos(30),0]) rotate([0,0,30]) cylinder(r=4/cos(30),h=2+5,$fn=6);
  }
  // lowest (-Y) extent : tip center at -70/2=35, but 8mm wide bar extends another 4*cos(30)
  // Low Y point: 38.464
  // hi Y point, 70.
  // If slic3r tries to center this, it will shift things by 15.768,
  // so when slicing, put plot center at (0,15.768) to force object center
  // to be printbed center.
  echo("centroid 15.768mm below center of Y-axis extent");

  // more XY-square parts to test straight lines/squareness
  for(a=[-1,1]) {
    translate([52*a, 10  ,0]) cube([ 6,60,4],center=true);
    translate([43*a,-18  ,0]) cube([16, 4,4],center=true);
    translate([34*a, 37.5,0]) cube([35, 5,4],center=true);
  }

  difference() {
    //cube([20/cos(30),20,4],center=true);
    cube([20,20,4],center=true);
    translate([0,0,-3]) cylinder(h=6,r=5/cos(30),$fn=6);  // 1cm wide hex, marks center
  }

  // more z-scale tests
  translate([0,5+4,12/2]) cube([8,8,12],center=true);
}

%translate([0,0,-1]) rotate([0,0,30]) cylinder(h=2,r=70,$fn=6);
