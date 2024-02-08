# mpicc -Wall -o test test.c -fopenmp -Minfo -lmpi
# mpirun -np 4 ./qf_hybrid --mca orte_base_help_aggregate 0

CC=gcc
MPI = mpicc
CFLAGS= -Wall
LDFLAGS= -fopenmp -Minfo -lmpi
HELP = --mca orte_base_help_aggregate 0
ALL=qf_seq qf_hybrid qf_omp

all: $(ALL)

qf_seq: qf_seq.c
	$(CC) $(CFLAGS) -o qf_seq qf_seq.c -O2 -fopt-info

qf_omp: qf_omp.c
	$(CC) $(CFLAGS) -o qf_omp qf_omp.c -Ofast -fopenmp -fopt-info


qf_hybrid: qf_hybrid.c
	$(MPI) $(CFLAGS) -o qf_hybrid qf_hybrid.c $(LDFLAGS) -Ofast


.PHONY: clean
clean:
	-rm -f $(ALL)

