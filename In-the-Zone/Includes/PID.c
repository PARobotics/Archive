#ifndef PID_C
#define PID_C

//Pid
int vcmd_ctrl_xv(
  int x, int v,
  int x_target, int v_target, float kx, float kv,
  int v_def, int v_min, int v_max) {
  // parameters:
  // x:  position from sensor (say angle)
  // v:  velocity from sensor (say rpm)
  // x_target:  target position
  // v_target:  target velocity
  // kx: control parameter
  // kv: control parameter
  // v_def: default vcmd if y is within the band
  // v_min: min value of vcmd
  // v_max: max value of vcmd

  // How it works
  // this can be used for holding a target at a position -- essentially set v_target=0
  // vcmd = v_def-kx*(x-x_target)-kv*(v-v_target)
  // if you are ahead of x_target; will lower vcmd to pull back
  // if the speed is in the direction of getting ahead, also lower vcmd to pull back

  int vcmd;
  vcmd = v_def - kx * (x - x_target) - kv * (v - v_target);
  vcmd = BOUND (vcmd, v_min, v_max);

  return vcmd;
}


int vcmd_ctrl_linear_band(
  int x, int y,
  int x1, int x2, int y1, int y2, int dy, float kv,
  int v_def, int v_min, int v_max) {
  // parameters:
  // x:      something from sensor (say angle)
  // y(x):   something else from sensor (say rpm)
  // vcmd:   -127 yo 127
  // desired response y(x) is a line defined by (x1,y1), (x2,y2)
  // dy:     the band is defined by dy
  // v_def: default vcmd if y is within the band
  // v_min: min value of vcmd
  // v_max: max value of vcmd
  // kv: control parameter

  // How it works
  // at given x, the desired y_desired = y1+(x-x1)*(y2-y1)/(x2-x1)
  // if y > y_desired+dy, reduce vcmd from v_def as vmd = v_def-kv*(y - y_desired -dy)
  // if y < y_desired-dy, increase vcmd from v_def as vmd = v_def+kv*(y_desired-dy-y)
  int y_desired,  vcmd;
  y_desired = y1 + (x - x1) * (y2 - y1) / (x2 - x1);

  if (y > y_desired + dy) {
    vcmd = v_def - kv * (y - y_desired - dy);
  }
  else if (y < y_desired - dy) {
    vcmd = v_def + kv * (y_desired - dy - y);
  }
  else {
    vcmd = v_def;
  }
  vcmd = BOUND (vcmd, v_min, v_max);
  // writeDebugStreamLine("X %4d %4d %4d %4d",x,y,y_desired,vcmd);
  return vcmd;
}
#endif //PID_C
