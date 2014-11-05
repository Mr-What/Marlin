/*
Delta specific routines.
License : GPL
*/
#include "Marlin.h"
#ifdef DELTA

//#include "ultralcd.h"
//#include "planner.h"
#include "stepper.h"  // enable_endstops()
//#include "temperature.h"
//#include "motion_control.h"
//#include "cardreader.h"
//#include "watchdog.h"
//#include "ConfigurationStore.h"
//#include "language.h"
//#include "pins_arduino.h"
//#include "math.h"

//#ifdef BLINKM
//#include "BlinkM.h"
//#include "Wire.h"
//#endif

//#if NUM_SERVOS > 0
//#include "Servo.h"
//#endif

//#if defined(DIGIPOTSS_PIN) && DIGIPOTSS_PIN > -1
//#include <SPI.h>
//#endif


//=============================imported variables============================
//===========================================================================


//===========================================================================
//=============================public variables=============================
//===========================================================================
//#ifdef SDSUPPORT
//CardReader card;
//#endif
//float homing_feedrate[] = HOMING_FEEDRATE;
//bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
//int feedmultiply=100; //100->1 200->2
//int saved_feedmultiply;
//int extrudemultiply=100; //100->1 200->2
//float volumetric_multiplier[EXTRUDERS] = {1.0
//  #if EXTRUDERS > 1
//    , 1.0
//    #if EXTRUDERS > 2
//      , 1.0
//    #endif
//  #endif
//};

// should get these by extern from Marlin.h :
//float current_position[NUM_AXIS] = { 0.0, 0.0, 0.0, 0.0 };
//float add_homeing[3]={0,0,0};
//float min_pos[3] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };
//float max_pos[3] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };
//bool axis_known_position[3] = {false, false, false};
//float zprobe_zoffset;

//// Extruder offset
//#if EXTRUDERS > 1
//#ifndef DUAL_X_CARRIAGE
//  #define NUM_EXTRUDER_OFFSETS 2 // only in XY plane
//#else
//  #define NUM_EXTRUDER_OFFSETS 3 // supports offsets in XYZ plane
//#endif
//float extruder_offset[NUM_EXTRUDER_OFFSETS][EXTRUDERS] = {
//#if defined(EXTRUDER_OFFSET_X) && defined(EXTRUDER_OFFSET_Y)
//  EXTRUDER_OFFSET_X, EXTRUDER_OFFSET_Y
//#endif
//};
//#endif

//uint8_t active_extruder = 0;
//int fanSpeed=0;
//#ifdef SERVO_ENDSTOPS
//  int servo_endstops[] = SERVO_ENDSTOPS;
//  int servo_endstop_angles[] = SERVO_ENDSTOP_ANGLES;
//#endif
//#ifdef BARICUDA
//int ValvePressure=0;
//int EtoPPressure=0;
//#endif

//#ifdef FWRETRACT
//  bool autoretract_enabled=true;
//  bool retracted=false;
//  float retract_length=3, retract_feedrate=17*60, retract_zlift=0.8;
//  float retract_recover_length=0, retract_recover_feedrate=8*60;
//#endif

//#ifdef ULTIPANEL
//  #ifdef PS_DEFAULT_OFF
//    bool powersupply = false;
//  #else
//    bool powersupply = true;
//  #endif
//#endif

// declare actual instance of Delta here, others access by extern in Marlin.h
DeltaParams Delta;

//#ifdef NONLINEAR_BED_LEVELING // puked when attempting linear fit (ab)
#ifdef ACCURATE_BED_LEVELING
float bed_level[ACCURATE_BED_LEVELING_POINTS][ACCURATE_BED_LEVELING_POINTS];
#endif

//===========================================================================
//=============================private variables=============================
//===========================================================================
//const char axis_codes[NUM_AXIS] = {'X', 'Y', 'Z', 'E'};
extern float destination[NUM_AXIS];  // from Marlin_main.cpp
//static float offset[3] = {0.0, 0.0, 0.0};
//static bool home_all_axis = true;
extern float feedrate; // from Marlin_main.cpp
//static float feedrate = 1500.0, next_feedrate, saved_feedrate;
//static float feedrate = 1500.0, next_feedrate, saved_feedrate;
//static long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

//static bool relative_mode = false;  //Determines Absolute or Relative Coordinates

//static char cmdbuffer[BUFSIZE][MAX_CMD_SIZE];
//static bool fromsd[BUFSIZE];
//static int bufindr = 0;
//static int bufindw = 0;
//static int buflen = 0;
////static int i = 0;
//static char serial_char;
//static int serial_count = 0;
//static boolean comment_mode = false;
//static char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc

//const int sensitive_pins[] = SENSITIVE_PINS; // Sensitive pin list for M42

////static float tt = 0;
////static float bt = 0;

//Inactivity shutdown variables
extern unsigned long previous_millis_cmd;  // from Marlin_main.cpp

//static unsigned long max_inactive_time = 0;
//static unsigned long stepper_inactive_time = DEFAULT_STEPPER_DEACTIVE_TIME*1000l;

//unsigned long starttime=0;
//unsigned long stoptime=0;

//static uint8_t tmp_extruder;


//bool Stopped=false;

//#if NUM_SERVOS > 0
//  Servo servos[NUM_SERVOS];
//#endif

//bool CooldownNoWait = true;
//bool target_direction;

//===========================================================================
//=============================ROUTINES=============================
//===========================================================================

/*

void get_arc_coordinates();
bool setTargetedHotend(int code);

void serial_echopair_P(const char *s_P, float v)
    { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char *s_P, double v)
    { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char *s_P, unsigned long v)
    { serialprintPGM(s_P); SERIAL_ECHO(v); }
*/

#ifdef ENABLE_AUTO_BED_LEVELING
#ifdef ACCURATE_BED_LEVELING
void set_bed_level_equation_lsq(double *plane_equation_coefficients)
{
    vector_3 planeNormal = vector_3(-plane_equation_coefficients[0], -plane_equation_coefficients[1], 1);
    planeNormal.debug("planeNormal");
    plan_bed_level_matrix = matrix_3x3::create_look_at(planeNormal);
    //bedLevel.debug("bedLevel");

    //plan_bed_level_matrix.debug("bed level before");
    //vector_3 uncorrected_position = plan_get_position_mm();
    //uncorrected_position.debug("position before");

    vector_3 corrected_position = plan_get_position();
//    corrected_position.debug("position after");
    current_position[X_AXIS] = corrected_position.x;
    current_position[Y_AXIS] = corrected_position.y;
    current_position[Z_AXIS] = corrected_position.z;

    // but the bed at 0 so we don't go below it.
    current_position[Z_AXIS] = zprobe_zoffset; // in the lsq we reach here after raising the extruder due to the loop structure

    plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}
#endif
#endif

void run_z_probe() {
#ifndef DELTA_TRAM_COMPENSATION
    plan_bed_level_matrix.set_to_identity();
#endif

    enable_endstops(true);
    //SERIAL_ECHOLNPGM("endstops enabled for z-probe");
    float start_z = current_position[Z_AXIS];
    long start_steps = st_get_position(Z_AXIS);

    feedrate = homing_feedrate[Z_AXIS]/20;
    destination[Z_AXIS] = -3;
    prepare_move_raw();
    st_synchronize();
    endstops_hit_on_purpose();

    enable_endstops(false);
    long stop_steps = st_get_position(Z_AXIS);
    //SERIAL_ECHOPGM("start position (steps)? = ");SERIAL_ECHOLN(stop_steps);

    float mm = start_z - float(start_steps - stop_steps) / axis_steps_per_unit[Z_AXIS];
    current_position[Z_AXIS] = mm;
    calculate_delta(current_position);
    //SERIAL_ECHOPGM("Delta start ");
    plan_set_position(Delta.delta[X_AXIS], Delta.delta[Y_AXIS], Delta.delta[Z_AXIS],
                      current_position[E_AXIS]);
}

#ifdef NONLINEAR_BED_LEVELING
static void extrapolate_one_point(int x, int y, int xdir, int ydir) {
  if (bed_level[x][y] != 0.0) {
    return;  // Don't overwrite good values.
  }
  float a = 2*bed_level[x+xdir][y] - bed_level[x+xdir*2][y];  // Left to right.
  float b = 2*bed_level[x][y+ydir] - bed_level[x][y+ydir*2];  // Front to back.
  float c = 2*bed_level[x+xdir][y+ydir] - bed_level[x+xdir*2][y+ydir*2];  // Diagonal.
  float median = c;  // Median is robust (ignores outliers).
  if (a < b) {
    if (b < c) median = b;
    if (c < a) median = a;
  } else {  // b <= a
    if (c < b) median = b;
    if (a < c) median = a;
  }
  bed_level[x][y] = median;
}

// Fill in the unprobed points (corners of circular print surface)
// using linear extrapolation, away from the center.
static void extrapolate_unprobed_bed_level() {
  int half = (ACCURATE_BED_LEVELING_POINTS-1)/2;
  for (int y = 0; y <= half; y++) {
    for (int x = 0; x <= half; x++) {
      if (x + y < 3) continue;
      extrapolate_one_point(half-x, half-y, x>1?+1:0, y>1?+1:0);
      extrapolate_one_point(half+x, half-y, x>1?-1:0, y>1?+1:0);
      extrapolate_one_point(half-x, half+y, x>1?+1:0, y>1?-1:0);
      extrapolate_one_point(half+x, half+y, x>1?-1:0, y>1?-1:0);
    }
  }
}

// Print calibration results for plotting or manual frame adjustment.
static void print_bed_level() {
  for (int y = 0; y < ACCURATE_BED_LEVELING_POINTS; y++) {
    for (int x = 0; x < ACCURATE_BED_LEVELING_POINTS; x++) {
      SERIAL_PROTOCOL_F(bed_level[x][y], 2);
      SERIAL_PROTOCOLPGM(" ");
    }
    SERIAL_ECHOLN("");
  }
}

// Reset calibration results to zero.
static void reset_bed_level() {
  for (int y = 0; y < ACCURATE_BED_LEVELING_POINTS; y++) {
    for (int x = 0; x < ACCURATE_BED_LEVELING_POINTS; x++) {
      bed_level[x][y] = 0.0;
    }
  }
}
#endif //NONLINEAR_BED_LEVELING

void calculate_delta(float cartesian[3])
{
  float dx,dy;
  // it appears that if you set DELTA_RADIUS appropriately, the below is equivalent to above
  dx = Delta.t1x - cartesian[X_AXIS];
  dy = Delta.t1y - cartesian[Y_AXIS];
  Delta.delta[X_AXIS] = sqrt(Delta.diagRodLen2 - (dx*dx + dy*dy)) + cartesian[Z_AXIS];
  dx = Delta.t2x - cartesian[X_AXIS];
  dy = Delta.t2y - cartesian[Y_AXIS];
  Delta.delta[Y_AXIS] = sqrt(Delta.diagRodLen2 - (dx*dx + dy*dy)) + cartesian[Z_AXIS];
  dx = cartesian[X_AXIS];
  dy = Delta.t3y - cartesian[Y_AXIS];
  Delta.delta[Z_AXIS] = sqrt(Delta.diagRodLen2 - (dx*dx + dy*dy)) + cartesian[Z_AXIS];

  if (Delta.verbose & 1)
    {
      SERIAL_ECHOPGM("cartesian("); SERIAL_ECHO(cartesian[X_AXIS]);
      SERIAL_ECHOPGM(","); SERIAL_ECHO(cartesian[Y_AXIS]);
      SERIAL_ECHOPGM(","); SERIAL_ECHO(cartesian[Z_AXIS]);

      SERIAL_ECHOPGM(")   delta("); SERIAL_ECHO(Delta.delta[X_AXIS]);
      SERIAL_ECHOPGM(","); SERIAL_ECHO(Delta.delta[Y_AXIS]);
      SERIAL_ECHOPGM(","); SERIAL_ECHO(Delta.delta[Z_AXIS]);
      SERIAL_ECHOLNPGM(")");
    }
}

// Adjust print surface height by linear interpolation over the bed_level array.
void adjust_delta(float cartesian[3])
{
#ifdef DELTA_TRAM_COMPENSATION
  float x = cartesian[X_AXIS];
  float y = cartesian[Y_AXIS];
  float offset = Delta.tramPoly.A +
    Delta.tramPoly.B * x   +
    Delta.tramPoly.C * y   +
    Delta.tramPoly.D * x*y +
    Delta.tramPoly.E * x*x +
    Delta.tramPoly.F * y*y ;
  if (Delta.verbose & 2)
    {
      SERIAL_ECHOPGM(" offset("); SERIAL_ECHO(round(cartesian[X_AXIS]));
      SERIAL_ECHOPGM(",");        SERIAL_ECHO(round(cartesian[Y_AXIS]));
      SERIAL_ECHOPGM(")=");SERIAL_ECHOLN(offset);
    }
#else
#ifdef ENABLE_AUTO_BED_LEVELING
  int half = (ACCURATE_BED_LEVELING_POINTS - 1) / 2;
  float grid_x = max(0.001-half, min(half-0.001, cartesian[X_AXIS] / ACCURATE_BED_LEVELING_GRID_X));
  float grid_y = max(0.001-half, min(half-0.001, cartesian[Y_AXIS] / ACCURATE_BED_LEVELING_GRID_Y));
  int floor_x = floor(grid_x);
  int floor_y = floor(grid_y);
  float ratio_x = grid_x - floor_x;
  float ratio_y = grid_y - floor_y;
  float z1 = bed_level[floor_x+half][floor_y+half];
  float z2 = bed_level[floor_x+half][floor_y+half+1];
  float z3 = bed_level[floor_x+half+1][floor_y+half];
  float z4 = bed_level[floor_x+half+1][floor_y+half+1];
  float left = (1-ratio_y)*z1 + ratio_y*z2;
  float right = (1-ratio_y)*z3 + ratio_y*z4;
  float offset = (1-ratio_x)*left + ratio_x*right;

  /*
  SERIAL_ECHOPGM("grid_x="); SERIAL_ECHO(grid_x);
  SERIAL_ECHOPGM(" grid_y="); SERIAL_ECHO(grid_y);
  SERIAL_ECHOPGM(" floor_x="); SERIAL_ECHO(floor_x);
  SERIAL_ECHOPGM(" floor_y="); SERIAL_ECHO(floor_y);
  SERIAL_ECHOPGM(" ratio_x="); SERIAL_ECHO(ratio_x);
  SERIAL_ECHOPGM(" ratio_y="); SERIAL_ECHO(ratio_y);
  SERIAL_ECHOPGM(" z1="); SERIAL_ECHO(z1);
  SERIAL_ECHOPGM(" z2="); SERIAL_ECHO(z2);
  SERIAL_ECHOPGM(" z3="); SERIAL_ECHO(z3);
  SERIAL_ECHOPGM(" z4="); SERIAL_ECHO(z4);
  SERIAL_ECHOPGM(" left="); SERIAL_ECHO(left);
  SERIAL_ECHOPGM(" right="); SERIAL_ECHO(right);
  SERIAL_ECHOPGM(" offset="); SERIAL_ECHOLN(offset);
  */
#endif
#endif
  Delta.delta[X_AXIS] += offset;
  Delta.delta[Y_AXIS] += offset;
  Delta.delta[Z_AXIS] += offset;
}

void prepare_move_raw()
{
  previous_millis_cmd = millis();
  calculate_delta(destination);
  plan_buffer_line(Delta.delta[X_AXIS], Delta.delta[Y_AXIS], Delta.delta[Z_AXIS],
                   destination[E_AXIS], feedrate*feedmultiply/60/100.0,
                   active_extruder);
  for(int8_t i=0; i < NUM_AXIS; i++) {
    current_position[i] = destination[i];
  }
}

void prepare_move_delta()
{
  float difference[NUM_AXIS];
  for (int8_t i=0; i < NUM_AXIS; i++) {
    difference[i] = destination[i] - current_position[i];
  }
  float cartesian_mm = sqrt(sq(difference[X_AXIS]) +
                            sq(difference[Y_AXIS]) +
                            sq(difference[Z_AXIS]));
  if (cartesian_mm < 0.000001) { cartesian_mm = abs(difference[E_AXIS]); }
  if (cartesian_mm < 0.000001) { return; }
  float seconds = 6000 * cartesian_mm / feedrate / feedmultiply;
  int steps = max(1, int(DELTA_SEGMENTS_PER_SECOND * seconds));
  // SERIAL_ECHOPGM("mm="); SERIAL_ECHO(cartesian_mm);
  // SERIAL_ECHOPGM(" seconds="); SERIAL_ECHO(seconds);
  // SERIAL_ECHOPGM(" steps="); SERIAL_ECHOLN(steps);
  for (int s = 1; s <= steps; s++) {
    float fraction = float(s) / float(steps);
    for(int8_t i=0; i < NUM_AXIS; i++) {
      destination[i] = current_position[i] + difference[i] * fraction;
    }
    calculate_delta(destination);
    #ifdef NONLINEAR_BED_LEVELING
      #error DELTA_TRAM should not call this
      adjust_delta(destination);
    #endif
    #ifdef DELTA_TRAM_COMPENSATION
      if (!Delta.tramDisabled) adjust_delta(destination);
    #endif

    plan_buffer_line(Delta.delta[X_AXIS], Delta.delta[Y_AXIS], Delta.delta[Z_AXIS],
                     destination[E_AXIS], feedrate*feedmultiply/60/100.0,
                     active_extruder);
  }
}

// retrace by the amount specified in endstop_adj
void retrace_from_endstop(const int axis)
{
  plan_set_position(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
  destination[axis] = Delta.endstop_adj[axis];
MYSERIAL.print("Tower");MYSERIAL.print(axis);MYSERIAL.print("  adjust=");MYSERIAL.println(Delta.endstop_adj[axis]);
//MYSERIAL.print(destination[X_AXIS]);MYSERIAL.print(' ');
//MYSERIAL.print(destination[Y_AXIS]);MYSERIAL.print(' ');
//MYSERIAL.println(destination[Z_AXIS]);
  plan_buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feedrate/60, active_extruder);
delay(1);
  st_synchronize();
delay(1);
      // trouble with Z tower not adjusting?!?? stays at endstop.  issue with Zmin being triggered at same time(ab)
}

#endif // DELTA -- no need for this code, non-delta

