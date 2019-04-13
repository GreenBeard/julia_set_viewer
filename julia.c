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
  return csqrt(x) + csqrt(y)*I;
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

//int main(int argc, char** argv) {
void test(void) {
  printf("Attempting a 2D julia set...\n");
  printf("Scale is the size of each pixel on the graph\n");
  printf("Output width, height, function width, and max iterations? ");

  unsigned int width;
  unsigned int height;
  double func_width;
  unsigned int max_iters;

  scanf("%u%u%lf%u", &width, &height, &func_width, &max_iters);

  double half_width = width / 2.0;
  double half_height = height / 2.0;
  double scale = func_width / width;

  FILE* file = fopen("./output.ppm", "w");
  if (file == NULL) {
    printf("Error opening file\n");
    return;
  }

  struct rgb_pixel* image = malloc(sizeof(struct rgb_pixel) * width * height);
  // TODO fix
  assert(image != NULL);

  #pragma omp parallel for collapse(2)
  for (unsigned int i = 0; i < width; ++i) {
    for (unsigned int j = 0; j < height; ++j) {
      double x = scale * (i - half_width);
      double y = scale * (half_height - j);
      double complex seed = fuzzy_seed(x, y);
      double complex c = fuzzy_c(x, y);
      unsigned int iters = iterate(seed, c, max_iters);
      struct hsv_pixel pixel;
      if (iters == max_iters) {
        pixel.h = 0;
        pixel.s = 0;
        pixel.v = 0;
      } else {
        pixel.h = fmod((((double) iters / (double) max_iters) * 360 + 180), 360);
        pixel.s = 1;
        pixel.v = 1;
      }
      image[j*width + i] = hsv_to_rgb(pixel);
    }
  }

  fprintf(file, "P6 %u %u 255\n", width, height);
  /* Output image as PPM */
  for (unsigned int j = 0; j < height; ++j) {
    for (unsigned int i = 0; i < width; ++i) {
      struct rgb_pixel p = image[j*width + i];
      fprintf(file, "%c%c%c", p.r, p.g, p.b);
    }
  }

  free(image);
  fclose(file);
}
