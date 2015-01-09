// delta-specific definitions

typedef struct DeltaParams_s {
  float delta[3];  // delta power positions
  float endstop_adj[3];
  float diagRodLen;
  float radius[3];
  #ifdef DELTA_TRAM_COMPENSATION
  struct { // bed tramming compensation polynomial
    float A,B,C,D,E,F;
  } tramPoly;
  // dependent variables:
  bool tramDisabled;
  #endif
  float diagRodLen2;
  float t1x, t1y, t2x, t2y, t3y; // tower axis positions  (t3x==0)
  int verbose;
} DeltaParams;
extern DeltaParams Delta;
//extern float delta[3];

void calculate_delta(float cartesian[3]);
void adjust_delta(float cartesian[3]);


#ifdef ENABLE_AUTO_BED_LEVELING
#ifdef ACCURATE_BED_LEVELING
void set_bed_level_equation_lsq(double *plane_equation_coefficients);
#endif
#endif
void run_z_probe();

#ifdef NONLINEAR_BED_LEVELING
void extrapolate_one_point(int x, int y, int xdir, int ydir);
void extrapolate_unprobed_bed_level();
void print_bed_level();
void reset_bed_level();
#endif //NONLINEAR_BED_LEVELING

void prepare_move_raw();
void prepare_move_delta();
void retrace_from_endstop(const int axis);

