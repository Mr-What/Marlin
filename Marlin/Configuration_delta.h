
// --------- Commonly altered delta calibration settings (ab)
// set this to slightly less than actual max dist from bed, so that
// we can use endstop-adjustments to fine-tune.
#define MANUAL_Z_HOME_POS 235

#define X_PROBE_OFFSET_FROM_EXTRUDER   0//-4
#define Y_PROBE_OFFSET_FROM_EXTRUDER   0//7
#define Z_PROBE_OFFSET_FROM_EXTRUDER -11.582


//===========================================================================
//============================= DELTA Printer ===============================
//===========================================================================
// For a Delta printer replace the configuration files with the files in the
// example_configurations/delta directory.
//

// Make delta curves from many straight lines (linear interpolation).
// This is a trade-off between visible corners (not enough segments)
// and processor overload (too many expensive sqrt calls).
#define DELTA_SEGMENTS_PER_SECOND 160

// Center-to-center distance of the holes in the diagonal push rods.
// ** move to top.  changes frequently
#define DELTA_DIAGONAL_ROD 218.233 // 217.95 // mm

// Horizontal distance bridged by diagonal push rods when effector is centered.
// Move this to top since it is a common calibration setting
// It appears that DELTA_RADIUS is the only number used, so just set it directly,
// instead of frobbing the 3 ill-defined and hard-to-measure numbers (ab)
//#define DELTA_RADIUS (DELTA_SMOOTH_ROD_OFFSET-DELTA_EFFECTOR_OFFSET-DELTA_CARRIAGE_OFFSET)
//
// Radius from center of bed to ?vert tower?
// Decreasing this number makes Z=const surface more  bowl-shaped (center  lower than edges)
// Increasing this number makes Z=const surface more igloo-shaped (center higher than edges)
#define DELTA_RADIUS 109.5 //109.4 //108.07 original build, before initial Frog Carriage
//#define DELTA_RADIUS1 111.083//109.5//109.15
//#define DELTA_RADIUS2 108.321//110.18
//#define DELTA_RADIUS3 109.337//109.10

#define DELTA_TRAM_COMPENSATION  // define this for bed-leveling polynomial

// Print surface diameter/2 minus unreachable space (avoid collisions with vertical towers).
#define DELTA_PRINTABLE_RADIUS 75.0

//// Effective X/Y positions of the three vertical towers.
//#define SIN_60 0.8660254037844386
//#define COS_60 0.5
//// on my current build Tower1 carriage is a bit thinner than others
//#define DELTA_TOWER1_X -SIN_60*DELTA_RADIUS1 // front left tower
//#define DELTA_TOWER1_Y -COS_60*DELTA_RADIUS1
//#define DELTA_TOWER2_X SIN_60*DELTA_RADIUS2 // front right tower
//#define DELTA_TOWER2_Y -COS_60*DELTA_RADIUS2
//#define DELTA_TOWER3_X 0.0 // back middle tower
//#define DELTA_TOWER3_Y DELTA_RADIUS3

// If the DELTA_RADIUS is the linkage radius MINUS EFFECTOR_OFFSET, we don't need this!
//#define EFFECTOR_OFFSET1_X -SIN_60*DELTA_EFFECTOR_OFFSET // toward front left tower
//#define EFFECTOR_OFFSET1_Y -COS_60*DELTA_EFFECTOR_OFFSET
//#define EFFECTOR_OFFSET2_X  SIN_60*DELTA_EFFECTOR_OFFSET // toward front right tower
//#define EFFECTOR_OFFSET2_Y -COS_60*DELTA_EFFECTOR_OFFSET
//#define EFFECTOR_OFFSET3_X  0.0                          // toward back middle tower
//#define EFFECTOR_OFFSET3_Y         DELTA_EFFECTOR_OFFSET

// Diagonal rod squared
//#define DELTA_DIAGONAL_ROD_2 (DELTA_DIAGONAL_ROD*DELTA_DIAGONAL_ROD)

// Travel limits after homing
#define X_MAX_POS DELTA_PRINTABLE_RADIUS
#define X_MIN_POS -DELTA_PRINTABLE_RADIUS
#define Y_MAX_POS DELTA_PRINTABLE_RADIUS
#define Y_MIN_POS -DELTA_PRINTABLE_RADIUS
#define Z_MAX_POS MANUAL_Z_HOME_POS
#define Z_MIN_POS 0

#define X_MAX_LENGTH (X_MAX_POS - X_MIN_POS)
#define Y_MAX_LENGTH (Y_MAX_POS - Y_MIN_POS)
#define Z_MAX_LENGTH (Z_MAX_POS - Z_MIN_POS)
//============================= Bed Auto Leveling ===========================

#ifdef ENABLE_AUTO_BED_LEVELING

  // these are the positions on the bed to do the probing
  #define DELTA_PROBABLE_RADIUS (DELTA_PRINTABLE_RADIUS-15)
  #define LEFT_PROBE_BED_POSITION -DELTA_PROBABLE_RADIUS
  #define RIGHT_PROBE_BED_POSITION DELTA_PROBABLE_RADIUS
  #define BACK_PROBE_BED_POSITION DELTA_PROBABLE_RADIUS
  #define FRONT_PROBE_BED_POSITION -DELTA_PROBABLE_RADIUS

  // with accurate bed leveling, the bed is sampled in a ACCURATE_BED_LEVELING_POINTSxACCURATE_BED_LEVELING_POINTS grid and least squares solution is calculated
  // Note: this feature occupies 10'206 byte
  //#define ACCURATE_BED_LEVELING

  #ifdef ACCURATE_BED_LEVELING
    #define ACCURATE_BED_LEVELING_POINTS 7
    #define ACCURATE_BED_LEVELING_GRID_X ((RIGHT_PROBE_BED_POSITION - LEFT_PROBE_BED_POSITION) / (ACCURATE_BED_LEVELING_POINTS - 1))
    #define ACCURATE_BED_LEVELING_GRID_Y ((BACK_PROBE_BED_POSITION - FRONT_PROBE_BED_POSITION) / (ACCURATE_BED_LEVELING_POINTS - 1))

    // NONLINEAR_BED_LEVELING means: don't try to calculate linear coefficients but instead
    // compensate by interpolating between the nearest four Z probe values for each point.
    // Useful for deltabots where the print surface may appear like a bowl or dome shape.
    // Works best with ACCURATE_BED_LEVELING_POINTS 5 or higher.
    #define NONLINEAR_BED_LEVELING
  #endif
#endif

