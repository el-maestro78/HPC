#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char** argv)
{
  int unsigned rank, size, provided ;
  const unsigned int n = 16384;
  // Measurements
  double start_time, end_time, elapsed_time;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);

  if (provided < MPI_THREAD_FUNNELED) {
    printf("Error: MPI threading level is not enough.\n");
    MPI_Finalize();
    return 1;
  }

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const unsigned int start = n*rank / size;
    const unsigned int end = n*(rank+1) / size;
    const unsigned int chunk_size = end - start;

    double *A = (double *)malloc(chunk_size * chunk_size * sizeof(double));
    double *v = (double *)malloc(chunk_size * sizeof(double));
    double *w = (double *)malloc(chunk_size * sizeof(double));

    start_time = MPI_Wtime();

#pragma omp parallel for simd// collapse(2)
    for (unsigned int i=0; i<chunk_size; ++i)
        for (unsigned int j=0; j<chunk_size; ++j)
            A[i * chunk_size + j] = (i + 2.0 * j) / (chunk_size * chunk_size);


#pragma omp parallel for simd
    for (unsigned int i = 0; i < chunk_size; ++i) {
        v[i] = 1.0 + 2.0 / (i + 0.5);
        w[i] = 1.0 - i / (3.0 * chunk_size);
    }

    // Compute the result locally using OpenMP
    double local_result = 0.0;
#pragma omp parallel for reduction(+:local_result) simd// collapse(2)
    for (unsigned int i = 0; i < chunk_size; ++i) {
      for (unsigned int j = 0; j < chunk_size; ++j) {
        local_result += v[i] * A[i*chunk_size + j] * w[j];
      }
    }

    double results;
    MPI_Reduce(&local_result, &results, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    long long total_flop = 6 * chunk_size * chunk_size + 6 * chunk_size;
    double flops;
    double speedup;
    double sequential_time = 2.192374;

    end_time = MPI_Wtime();
    elapsed_time = end_time - start_time;
    flops = total_flop / elapsed_time;
    speedup = sequential_time / elapsed_time;
    if (rank == 0){
        printf("Total Result = %lf\n", results);
        printf("Elapsed time: %f seconds\n", elapsed_time);
        printf("FLOPs per second: %e\n", flops);
        printf("Speedup: %f\n", speedup);
    }

      //free(results);
    free(A);
    free(v);
    free(w);
    MPI_Finalize();


    return 0;
}
