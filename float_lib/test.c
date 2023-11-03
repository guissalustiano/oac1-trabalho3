#include "float_lib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <fenv.h>

# define true 1
# define false 0

// from https://math.stackexchange.com/a/4508789
uint8_t within(float a, float b, float tol) {
    float d = fabs(a - b);
    if (d == 0) {
        return true;
    }
    // Since d =/= 0 we know a, b =/= 0 so division by zero will not happen
    float err = d/(fabs(a)+fabs(b));
    return err < tol;
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

void assert_float(floatint a, floatint b, float tol) {
    if (!within(a.f, b.f, tol)) {
        printf("ERROR. Expected %f (%x), got %f (%x)\n", b.f, b.i, a.f, a.i);
    } else {
        printf("OK. %f (%x)\n", a.f, a.i);
    }
}

void assert_floatsisf(int32_t a) {
    floatint actual, expected;

    actual.i = floatsisf(a);
    expected.f = (float) a;

    assert_float(actual, expected, 0.00001);
}

void test_cases_floatsist() {
    puts("Testing floatsist");
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
    puts("Testing fixsfsi");
    for (float i = INT32_MIN; i < -0; i/=10) {
        assert_fixsfsi(i);
    }
    assert_fixsfsi(0.0);
    for (float i = INT32_MAX; i > 0; i/=10) {
        assert_fixsfsi(i);
    }
}

void assert_negsf2(float a) {
    floatint arg;
    floatint actual, expected;

    arg.f = a;
    actual.i = negsf2(arg.i);
    expected.f = -a;

    assert_float(actual, expected, 0);
}

void test_cases_negsf2() {
    puts("Testing negsf2");
    for (float i = FLT_MAX; i > 0; i/=1000) {
        assert_negsf2(i);
        assert_negsf2(-i);
    }
    assert_negsf2(0.0);
}

// float flags from: https://stackoverflow.com/a/15655732
void assert_addsf3(float a, float b) {
    floatint arg1, arg2;
    floatint actual, expected;

    arg1.f = a;
    arg2.f = b;
    actual.i = addsf3(arg1.i, arg2.i);

    feclearexcept(FE_ALL_EXCEPT);
    expected.f = a + b;
    if (fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)) {
        return; // ignore invalid test cases
    }

    assert_float(actual, expected, 0.025);
}

void test_cases_addsf3() {
    puts("Testing addsf3");
    for (float i = FLT_MAX; i > 1e-20; i/=1e2) {
        for (float j = FLT_MAX; j > 1e-20; j/=1e2) {
            assert_addsf3(i, j);
            assert_addsf3(-i, j);
            assert_addsf3(i, -j);
            assert_addsf3(-i, -j);
        }
    }
}

void assert_subsf3(float a, float b) {
    floatint arg1, arg2;
    floatint actual, expected;

    arg1.f = a;
    arg2.f = b;
    actual.i = subsf3(arg1.i, arg2.i);

    feclearexcept(FE_ALL_EXCEPT);
    expected.f = a - b;
    if (fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)) {
        return; // ignore invalid test cases
    }

    assert_float(actual, expected, 0.025);
}

void test_cases_subsf3() {
    puts("Testing subsf3");
    for (float i = FLT_MAX; i > 1e-20; i/=1e2) {
        for (float j = FLT_MAX; j > 1e-20; j/=1e2) {
            assert_subsf3(i, j);
            assert_subsf3(-i, j);
            assert_subsf3(i, -j);
            assert_subsf3(-i, -j);
        }
    }
}

int main() {
    test_cases_floatsist();
    test_cases_fixsfsi();
    test_cases_negsf2();
    test_cases_addsf3();
    test_cases_subsf3();
    return 0;
}

