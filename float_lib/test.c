#include "float_lib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

# define true 1
# define false 0

#define ACCEPT_ERROR 0.00001

// from https://math.stackexchange.com/a/4508789
uint8_t within(float a, float b, float tol) {
    float d = fabs(a - b);
    if (d == 0) {
        return true;
    }
    // Since d =/= 0 we know a, b =/= 0 so division by zero will not happen
    return d/(fabs(a)+fabs(b)) < tol;
}

typedef union {
    mfloat i; // acess as int
    float  f; // acess as float
              // Lembrando que na implementacao de voces, voces nao podem usar float!
} floatint;

void assert_mint(int32_t a, int32_t b) {
    if (a != b) {
        printf("ERROR. Expected %d, got %d\n", b, a);
    } else {
        printf("OK. %d\n", a);
    }
}

void assert_float(floatint a, floatint b) {
    if (!within(a.f, b.f, ACCEPT_ERROR)) {
        printf("ERROR. Expected %.3f (%x), got %.3f (%x)\n", b.f, b.i, a.f, a.i);
    } else {
        printf("OK. %.3f (%x)\n", a.f, a.i);
    }
}

void assert_floatsisf(int32_t a) {
    floatint actual, expected;

    actual.i = floatsisf(a);
    expected.f = (float) a;

    assert_float(actual, expected);
}

void test_cases_floatsist() {
    for (mint i = INT32_MIN; i < -0; i/=10) {
        assert_floatsisf(i);
    }
    assert_floatsisf(0);
    for (mint i = INT32_MAX; i > 0; i/=10) {
        assert_floatsisf(i);
    }
}

void assert_fixsfsi(float a) {
    floatint arg;
    mint actual, expected;

    arg.f = a;
    actual = fixsfsi(arg.i);
    expected = (mint) a;

    assert_mint(actual, expected);
}

void test_cases_fixsfsi() {
    for (float i = INT32_MIN; i < -0; i/=10) {
        assert_fixsfsi(i);
    }
    assert_fixsfsi(0.0);
    for (float i = INT32_MAX; i > 0; i/=10) {
        assert_fixsfsi(i);
    }
}

int main() {
    test_cases_floatsist();
    test_cases_fixsfsi();
    return 0;
}

