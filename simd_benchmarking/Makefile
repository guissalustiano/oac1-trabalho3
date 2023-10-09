.PHONY : all
all : gradient_magnitude_basic gradient_magnitude_simd

gradient_magnitude_basic: gradient_magnitude_basic.c
	gcc -O2 -o gradient_magnitude_basic gradient_magnitude_basic.c

gradient_magnitude_simd: gradient_magnitude_simd.c
	gcc -O2 -o gradient_magnitude_simd gradient_magnitude_simd.c

clean:
	rm -f gradient_magnitude_basic gradient_magnitude_simd
