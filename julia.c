#include <assert.h>
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int iterate(double complex seed, double complex c, unsigned int max_iters) {
  unsigned int i;
  double complex z = seed;
  for (i = 0; i < max_iters; ++i) {
    if (cabs(z) > 2.0) {
      break;
    } else {
      z = z * z + c;
    }
  }
  if (i == max_iters) {
    return max_iters;
  } else {
    return i + 1 - log(log2(cabs(z)));
  }
}

double complex fuzzy_seed(double x, double y) {
  /*return csqrt(x) + csqrt(y)*I;*/
  return 0;
}

double complex fuzzy_c(double x, double y) {
  return x + y * I;
}

struct hsv_pixel {
  double h; /* hue */
  double s; /* saturation */
  double v; /* value/shade */
};

struct rgb_pixel {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

/* This algorithm was implemented from the method provided by rapidtables.com */
struct rgb_pixel hsv_to_rgb(struct hsv_pixel pixel) {
  assert(pixel.h <= 360 && pixel.h >= 0);
  assert(pixel.s <= 1 && pixel.s >= 0);
  assert(pixel.v <= 1 && pixel.v >= 0);

  struct rgb_pixel rgb_pixel;

  double c = pixel.v * pixel.s;
  double x = c * (1 - fabs(fmod(pixel.h / 60.0, 2.0) - 1));
  double m = pixel.v - c;

  double r;
  double g;
  double b;
  if (pixel.h < 60 * 1) {
    r = c;
    g = x;
    b = 0;
  } else if (pixel.h < 60 * 2) {
    r = x;
    g = c;
    b = 0;
  } else if (pixel.h < 60 * 3) {
    r = 0;
    g = c;
    b = x;
  } else if (pixel.h < 60 * 4) {
    r = 0;
    g = x;
    b = c;
  } else if (pixel.h < 60 * 5) {
    r = x;
    g = 0;
    b = c;
  } else {
    r = c;
    g = 0;
    b = x;
  }

  rgb_pixel.r = (r + m) * 255;
  rgb_pixel.g = (g + m) * 255;
  rgb_pixel.b = (b + m) * 255;

  return rgb_pixel;
}

void render_julia_rgb(unsigned char* data_out,
    unsigned int width, unsigned int height,
    double func_width, double x_mid, double y_mid,
    unsigned int max_iters) {
  double half_width = width / 2.0;
  double half_height = height / 2.0;
  double scale = func_width / width;

  #pragma omp parallel for collapse(2)
  for (unsigned int j = 0; j < height; ++j) {
    for (unsigned int i = 0; i < width; ++i) {
      double x = x_mid + scale * (i - half_width);
      double y = y_mid + scale * (half_height - j);
      double complex seed = fuzzy_seed(x, y);
      double complex c = fuzzy_c(x, y);
      unsigned int iters = iterate(seed, c, max_iters);
      struct hsv_pixel pixel;
      if (iters == max_iters) {
        pixel.h = 0;
        pixel.s = 0;
        pixel.v = 0;
      } else {
        pixel.h = fmod((((double) iters / (double) max_iters) * 360 + 120), 360);
        pixel.s = 1;
        pixel.v = 1;
      }
      struct rgb_pixel rgb_pixel = hsv_to_rgb(pixel);
      data_out[3*(j*width + i) + 0] = rgb_pixel.r;
      data_out[3*(j*width + i) + 1] = rgb_pixel.g;
      data_out[3*(j*width + i) + 2] = rgb_pixel.b;
    }
  }
}
