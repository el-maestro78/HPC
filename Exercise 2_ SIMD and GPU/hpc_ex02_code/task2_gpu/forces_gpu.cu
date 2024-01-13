#include <stdio.h>
#include <math.h>

#define CUDA_CHECK(call) \
do { \
    cudaError_t result = call; \
    if (result != cudaSuccess) { \
        fprintf(stderr, "CUDA error at %s:%d code=%d(%s) \n", \
                __FILE__, __LINE__, static_cast<unsigned int>(result), cudaGetErrorString(result)); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

typedef struct Particle_s
{
  double x, y, z, m;
  double fx, fy, fz;
} Particle_t;

const double G = 6.67408e-11;

__global__ void computeGravitationalForcesKernel(Particle_t *particles, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
    {
        particles[i].fx = 0;
        particles[i].fy = 0;
        particles[i].fz = 0;

        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                double tmp = pow(particles[i].x - particles[j].x, 2.0) +
                              pow(particles[i].y - particles[j].y, 2.0) +
                              pow(particles[i].z - particles[j].z, 2.0);

                double magnitude = G * particles[i].m * particles[j].m / pow(tmp, 1.5);

                particles[i].fx += (particles[i].x - particles[j].x) * magnitude;
                particles[i].fy += (particles[i].y - particles[j].y) * magnitude;
                particles[i].fz += (particles[i].z - particles[j].z) * magnitude;
            }
        }
    }
}

void computeGravitationalForces(Particle_t *particles, int n)
{
    Particle_t *d_particles;
    size_t size = n * sizeof(Particle_t);

    CUDA_CHECK(cudaMalloc((void **)&d_particles, size));
    CUDA_CHECK(cudaMemcpy(d_particles, particles, size, cudaMemcpyHostToDevice));

    int blockSize = 256;
    int gridSize = (n + blockSize - 1) / blockSize;

    computeGravitationalForcesKernel<<<gridSize, blockSize>>>(d_particles, n);

    CUDA_CHECK(cudaMemcpy(particles, d_particles, size, cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaFree(d_particles));
}

// Rest of the code remains unchanged