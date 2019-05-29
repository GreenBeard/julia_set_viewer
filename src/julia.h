#ifdef __cplusplus
extern "C" {
#endif

#include "override/complex.h"

void render_set_rgb(unsigned char* data_out,
    unsigned int width, unsigned int height,
    double func_width, double x_mid, double y_mid,
    unsigned int max_iters,
    bool mandelbrot, double complex julia_c);

#ifdef __cplusplus
}
#endif
