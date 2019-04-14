#ifdef __cplusplus
extern "C" {
#endif

void render_julia_rgb(unsigned char* data_out,
    unsigned int width, unsigned int height,
    double func_width, double x_mid, double y_mid,
    unsigned int max_iters);

#ifdef __cplusplus
}
#endif
