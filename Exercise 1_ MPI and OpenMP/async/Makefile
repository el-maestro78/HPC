CC=mpicc
CFLAGS=-O2 -Wall #-fopt-info
LDFLAGS=

ALL=ex01q3 ex01q3_sol ex01q3_scatter

all: $(ALL)

ex01q3: ex01q3.c
	$(CC) $(CFLAGS) -o ex01q3 ex01q3.c $(LDFLAGS)

ex01q3_sol: ex01q3_sol.c
	$(CC) $(CFLAGS) -o ex01q3_sol ex01q3_sol.c $(LDFLAGS)

ex01q3_scatter: ex01q3_scatter.c
	$(CC) $(CFLAGS) -o ex01q3_sol ex01q3_sol.c $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f $(ALL)

# mpirun -n 4 --mca coll_hcoll_enable 0 --mca pml ucx --mca pml_ucx_priority 0 ./ex01q3_sol