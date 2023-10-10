FLAGS = -O2 -Wall -Wextra -std=c99 -fopenmp -lm

.PHONY : all
all : gradient_magnitude_basic gradient_magnitude_simd

image.o: image.c image.h
	gcc -c image.c $(FLAGS)

gradient_magnitude_basic.o: gradient_magnitude_basic.c gradient_magnitude.h
	gcc -c gradient_magnitude_basic.c $(FLAGS)

gradient_magnitude_simd.o: gradient_magnitude_simd.c gradient_magnitude.h
	gcc -c gradient_magnitude_simd.c $(FLAGS)

main.o: main.c image.h gradient_magnitude.h
	gcc -c main.c $(FLAGS)

gradient_magnitude_basic: main.o image.o gradient_magnitude_basic.o
	gcc -o gradient_magnitude_basic main.o image.o gradient_magnitude_basic.o $(FLAGS)

gradient_magnitude_simd: main.o image.o gradient_magnitude_simd.o
	gcc -o gradient_magnitude_simd main.o image.o gradient_magnitude_simd.o $(FLAGS)

clean:
	rm -f *.o gradient_magnitude_basic gradient_magnitude_simd images/magnitude.pgm
