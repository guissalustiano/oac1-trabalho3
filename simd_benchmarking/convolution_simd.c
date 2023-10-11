#include "convolution.h"
#include "gradient_magnitude.h"
#include <immintrin.h>

Matrix conv2d(Matrix img, Matrix kernel) {
    Matrix out = {
        img.width - kernel.width + 1,
        img.height - kernel.height + 1,
        malloc(img.width * img.height * sizeof(int32_t))
    };

    #pragma omp parallel for
    for (int i = 0; i < out.height; i++) {
        for (int j = 0; j < out.width; j+=16) {
            // sum = 0;
            __m512i sum = _mm512_setzero_si512();

            for (int k = 0; k < kernel.height; k++) {
                for (int l = 0; l < kernel.width; l++) {
                    int x = j + l;
                    int y = i + k;

                    int32_t weight = kernel.data[k*kernel.width + l];

                    // pixeis = img.data[y*img.width + x];
                    __m512i pixeis = _mm512_loadu_epi32(img.data + y*img.width + x);

                    // sum += weight * pixeis[m];
                    __m512i weights = _mm512_set1_epi32(weight);
                    sum = _mm512_add_epi32(sum, _mm512_mullo_epi32(weights, pixeis));
                }
            }
            // sum /= FIXED_POINT;
            //__m512i fixed_point = _mm512_set1_epi32(FIXED_POINT);
            //sum = _mm512_div_epi16(sum, fixed_point);

            // out.data[i * out.width + j] = sum;
            _mm512_storeu_epi32(out.data + i * out.width + j, sum);

            // just intel compiler supoort simd integer div
            for(int m = 0; m < 16; m++) {
                out.data[i * out.width + j + m] /= FIXED_POINT;
            }
        }
    }
    return out;
}
