#include "convolution.h"
#include "gradient_magnitude.h"
#include <immintrin.h>

Matrix conv2d(Matrix img, Matrix kernel) {
    Matrix out = {
        img.width,
        img.height,
        malloc(img.width * img.height * sizeof(int32_t))
    };
    #pragma omp parallel for
    for (int i = 0; i < out.height; i++) {
        for (int j = 0; j < out.width; j++) {
            //__m256i sum = _mm256_setzero_si256(); // sum = 0
            int16_t sum[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            for (int k = 0; k < kernel.height; k++) {
                for (int l = 0; l < kernel.width; l++) {
                    int x = j + l;
                    int y = i + k;

                    int16_t weight = kernel.data[k*kernel.width + l];
                    int16_t pixel = img.data[y*img.width + x];

                    int16_t pixeis[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                    for (int m = 0; m < 16; m++) {
                        pixeis[m] = img.data[y*img.width + x + m];
                    }

                    for(int m = 0; m < 16; m++) {
                        sum[m] += weight * pixeis[m];
                    }
                }
            }
            for(int m = 0; m < 16; m++) {
                sum[m] /= FIXED_POINT;
            }

            for(int m = 0; m < 16; m++) {
                out.data[i * out.width + j + m] = sum[m];
            }
        }
    }
    return out;
}
