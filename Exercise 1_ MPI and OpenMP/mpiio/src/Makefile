MPICC=mpicc
GCC=gcc
perf?=0
ifeq "$(perf)" "1"
	CPPFLAGS += -D_PERF_
endif

dump?=0
ifeq "$(dump)" "1"
  CFLAGS += -D_DUMP_DENSITY_
endif


CFLAGS+=-Wall -O2
CFLAGS_THREADS=$(CFLAGS) -fopenmp
ZFPHOME = ~/ex1/mpiio/zfp-0.5.5 #/include/zfp.h
ZFPFLAGS = -I$(ZFPHOME)/include #-L$(ZFPHOME)/build/lib -lzfp
all: diffusion2d_mpi diffusion2d_mpi_nb diffusion2d_mpi_nb_io decompress_bin compare

diffusion2d_mpi: diffusion2d_mpi.c
	$(MPICC) $(CFLAGS) -o $@ $<

diffusion2d_mpi_nb: diffusion2d_mpi_nb.c
	$(MPICC) $(CFLAGS) -o $@ $<

diffusion2d_mpi_nb_io: diffusion2d_mpi_nb_io.c
	#$(MPICC) $(CFLAGS) $(ZFPFLAGS) -o $@ $<
	$(MPICC) $(CFLAGS) -o $@ $< -lz
decompress_bin: decompress_bin.c
	gcc -o decompress_bin decompress_bin.c -lz
compare: compare.c
	gcc -o compare compare.c -lz

clean:
	rm -f diffusion2d_mpi diffusion2d_mpi_nb diffusion2d_mpi_nb_io decompress_bin compare
	rm -rf *.dSYM
	rm -rf *.bin
	rm -rf *.dat


# Execution examples

# For benchmarking
# mpirun -n 1 ./diffusion2d_mpi_nb 1 1 1024 1000 0.00000001
# mpirun -n 4 ./diffusion2d_mpi_nb_io 1 1 1024 1000 0.00000001
# mpirun -n 4 --mca coll_hcoll_enable 0 --mca pml ucx --mca pml_ucx_priority 0 ./diffusion2d_mpi_nb 1 1 1024 1000 0.00000001
# mpirun -n 4 --mca coll_hcoll_enable 0 --mca pml ucx --mca pml_ucx_priority 0 ./diffusion2d_mpi_nb 1 1 2048 1000 0.00000001
# mpirun -n 4 --mca coll_hcoll_enable 0 --mca pml ucx --mca pml_ucx_priority 0 ./diffusion2d_mpi_nb 1 1 4096 1000 0.00000001

# For testing and visualization
# mpirun -n 1 ./diffusion2d_mpi_nb 1 1 128 50000 0.00001
# mpirun -n 4 ./diffusion2d_mpi_nb_io 1 1 128 50000 0.00001
# mpirun -n 4 --mca coll_hcoll_enable 0 --mca pml ucx --mca pml_ucx_priority 0 ./diffusion2d_mpi_nb_io 1 1 128 50000 0.00001
