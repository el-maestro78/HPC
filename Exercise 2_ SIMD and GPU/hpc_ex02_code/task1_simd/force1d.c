#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include <immintrin.h>
#include <x86intrin.h>
#include <omp.h>

//1 gcc -o t1_no_opt force1d.c
//2 gcc -o t1_3_opt force1d.c -O3
//3 gcc -o t1_auto_vect force1d.c -O3 -ftree-vectorizer-verbose=6
//4 gcc -o t1_man_vect force1d.c -msse4 -O3

double get_wtime(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec*1.0e-6;
}

/// parameters
const size_t N  = 1<<16; // system size
const float eps = 5.0;    // Lenard-Jones, eps
const float rm  = 0.1;   // Lenard-Jones, r_m


/// compute the Lennard-Jones force particle at position x0
float compute_force(float *positions, float x0)
{
	float rm2 = rm * rm;
	float force = 0.0f;
        /* old loop
    for (size_t i=0; i<N; ++i) {
      float r = x0 - positions[i];
      float r2 = r * r; // r^2
      float s2 = rm2 / r2; // (rm/r)^2
      float s6 = s2*s2*s2; // (rm/r)^6
      force += 12 * eps * (s6*s6 - s6) / r;
    } //*/
    __m256 x0_avx = _mm256_set1_ps(x0);
    __m256 rm2_avx = _mm256_set1_ps(rm2);
    __m256 eps_avx = _mm256_set1_ps(eps);
    __m256 twelve_avx = _mm256_set1_ps(12.0f);
    for (size_t i = 0; i < N; i += 8) {
        __m256 pos_avx = _mm256_load_ps(&positions[i]);
        __m256 r_avx = _mm256_sub_ps(x0_avx, pos_avx);
        __m256 r2_avx = _mm256_mul_ps(r_avx, r_avx);
            // (rm/r)^2 and (rm/r)^6
        __m256 s2_avx = _mm256_div_ps(rm2_avx, r2_avx);
        __m256 s6_avx = _mm256_mul_ps(_mm256_mul_ps(s2_avx, s2_avx), s2_avx);
            //force Equation
        __m256 tweleve_eps_avx = _mm256_mul_ps(twelve_avx, eps_avx);
        __m256 s6_sq_avx = _mm256_mul_ps(s6_avx,s6_avx);
        __m256 s6_sq_sub_s6_avx =_mm256_sub_ps(s6_sq_avx, s6_avx);
        __m256 force_fractor_avx = _mm256_div_ps(s6_sq_sub_s6_avx, r_avx);
        __m256 force_contribution_avx = _mm256_mul_ps(tweleve_eps_avx,force_fractor_avx);
        force_avx = _mm256_add_ps(force_avx, force_contribution_avx);
    }
    // Reduce & store
    float force_result[8];
    _mm256_store_ps(force_result, force_avx);

    for (int i = 0; i < 8; ++i) {
      force += force_result[i];
    }

    return force;
}

int main(int argc, const char** argv)
{
    /// init random number generator
	srand48(1);

    float *positions;
		positions = malloc(N*sizeof(float));

	for (size_t i=0; i<N; i++)
		positions[i] = drand48()+0.1;

    /// timings
	double start, end;

    float x0[] = { 0., -0.1, -0.2 };
    float f0[] = { 0, 0, 0 };

    const size_t repetitions = 1000;
    start = get_wtime();
    for (size_t i = 0; i < repetitions; ++i )
    {
        for( size_t j = 0; j < 3; ++j )
            f0[j] += compute_force(positions, x0[j]);
    }
    end = get_wtime();

    for(size_t j = 0; j < 3; ++j )
        printf("Force acting at x_0=%lf : %lf\n", x0[j], f0[j]/repetitions);

    printf("elapsed time: %lf mus\n", 1e6*(end-start));
		return 0;
}