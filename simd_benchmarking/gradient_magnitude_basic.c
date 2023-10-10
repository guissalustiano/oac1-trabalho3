#include <stdint.h>
#include "gradient_magnitude.h"
#include "image.h"

#define FIXED_POINT (1 << 4)

Matrix conv2d(Matrix img, Matrix kernel) {
    Matrix out = {
        img.width,
        img.height,
        malloc(img.width * img.height * sizeof(int32_t))
    };

    #pragma omp parallel for
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            int16_t sum = 0;
            for (int k = 0; k < kernel.height; k++) {
                for (int l = 0; l < kernel.width; l++) {
                    int x = j + l - kernel.width / 2;
                    int y = i + k - kernel.height / 2;

                    x = clamp(x, 0, img.width - 1);
                    y = clamp(y, 0, img.height - 1);

                    int16_t weight = kernel.data[k*kernel.width + l];
                    int16_t pixel = img.data[y*img.width + x];

                    sum += weight * pixel;
                }
            }
            sum /= FIXED_POINT;
            out.data[i * img.width + j] = sum;
        }
    }

    return out;
}

double gaussian2d(double x, double y, double sigma) {
    return expf(-(x*x + y*y) / (2.0 * sigma * sigma));
}

double dx_gaussian2d(double x, double y, double sigma) {
    return -x * gaussian2d(x, y, sigma) / (sigma * sigma);
}

double dy_gaussian2d(double x, double y, double sigma) {
    return dx_gaussian2d(y, x, sigma);
}

Matrix generate_kernel(double sigma, double (*gen_func)(double, double, double)) {
    uint32_t size = 2 * ceil(3 * sigma) + 1;
    Matrix kernel = {
        size,
        size,
        malloc(size * size * sizeof(int32_t))
    };

    for (int i = 0; i < size; i++) {
        int x = i - size / 2;
        for (int j = 0; j < size; j++) {
            int y = j - size / 2;

            double weight = gen_func(x, y, sigma);
            kernel.data[i * size + j] = weight * FIXED_POINT;
        }
    }

    return kernel;
}

Matrix gaussian_kernel(double sigma) {
    return generate_kernel(sigma, gaussian2d);
}

Matrix dx_gaussian_kernel(double sigma) {
    return generate_kernel(sigma, dx_gaussian2d);
}

Matrix dy_gaussian_kernel(double sigma) {
    return generate_kernel(sigma, dy_gaussian2d);
}

Gradient gradient_kernels(Matrix gaussian) {
    int32_t d[] = {-1,0, 1};
    Matrix dx = {1, 3, d};
    Matrix dy = {3, 1, d};

    Matrix dx_gaussian = conv2d(gaussian, dx);
    Matrix dy_gaussian = conv2d(gaussian, dy);

    free_image(dx_gaussian);

    return (Gradient) {
        dx_gaussian,
        dy_gaussian
    };
}

Matrix gradient_magnitude(Matrix fx, Matrix fy) {
    Matrix out = {
        fx.width,
        fx.height,
        malloc(fx.width * fx.height * sizeof(int32_t))
    };

    #pragma omp parallel for
    for (int i = 0; i < fx.height; i++) {
        for (int j = 0; j < fx.width; j++) {
            int32_t fx_pixel = fx.data[i * fx.width + j];
            int32_t fy_pixel = fy.data[i * fy.width + j];

            float magnitude = sqrtf(fx_pixel * fx_pixel + fy_pixel * fy_pixel);
            out.data[i * fx.width + j] = magnitude;
        }
    }

    return out;
}
