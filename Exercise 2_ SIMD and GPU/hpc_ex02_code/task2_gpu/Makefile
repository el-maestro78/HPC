all: forces_cpu forces_gpu

forces_cpu: forces_cpu.c
	gcc forces_cpu.c -g -O2 -Wall -o $@ -lm

forces_gpu: forces_gpu.forces_cpu
	nvcc forces_gpu.cu -o $@

.PHONY: clean
clean:
	rm -f forces_cpu forces_gpu
