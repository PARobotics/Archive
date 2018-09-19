// Definition of Global functions!
void flip_mobile_goal();
void mg_out();
void mg_in();
void down_with_pid();
int min(int a, int b);
int max(int a, int b);
int vcmd_stripped(
  int x, int x_target, float kx,
  int v_def, int v_min, int v_max
);
