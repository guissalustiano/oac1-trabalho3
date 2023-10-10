#ifndef GRADIENT_MAGNITUDE_H
#define GRADIENT_MAGNITUDE_H

#include "image.h"
#include <math.h>
#include <stdint.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define clamp(x, a, b) max(a, min(b, x))

typedef struct {
    Matrix dx;
    Matrix dy;
} Gradient;

Matrix conv2d(Matrix img, Matrix kernel);
Matrix gradient_magnitude(Matrix fx, Matrix fy);
Matrix gaussian_kernel(double sigma);
Matrix dx_gaussian_kernel(double sigma);
Matrix dy_gaussian_kernel(double sigma);

#endif
