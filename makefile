main.out: main.c
	mpicc main.c -lgmp -lm -o main.out