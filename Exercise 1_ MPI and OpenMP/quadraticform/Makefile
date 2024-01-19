CC=gcc
MPI = mpicc
CFLAGS= -Wall
LDFLAGS= -fopenmp -Minfo

ALL=qf_seq qf_hybrid

all: $(ALL)

qf_seq: qf_seq.c
	$(CC) $(CFLAGS) -o qf_seq qf_seq.c -O2 -fopt-info

qf_hybrid: qf_hybrid.c
	$(MPI) $(CFLAGS) -o qf_hybrid qf_hybrid.c $(LDFLAGS) -Ofast


.PHONY: clean
clean:
	-rm -f $(ALL)

