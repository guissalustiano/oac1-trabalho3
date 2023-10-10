#include "convolution.h"

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
