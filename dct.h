#include <math.h>

#define PI 3.141592653589

//input: f, N; output: F
short dct_direct( short N, double *f, double *F );
//f[i][j] * coef
//input: N, F; output f
short idct_direct( short N, double *F, double *f );
// change values from short to double and vice versa.
short dct ( short N, short *f, short *F );
// change values from short to doulbe, and vice versa.
short idct ( short N, short *F, short *f );
void print_elements ( short N,  short *f );
