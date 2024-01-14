all: forces_cpu forces_gpu

forces_cpu: forces_cpu.c
	gcc forces_cpu.c -g -O2 -Wall -o forces_cpu -lm -fopt-info

forces_gpu: forces_gpu
	nvcc -g -O3 -Xptxas -v -o forces_gpu forces_gpu.cu -lm

.PHONY: clean
clean:
	rm -f forces_cpu forces_gpu
