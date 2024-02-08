//#include "async/mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char** argv)
{
  int rank, size, provided ;
  int n = 16384;

  MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (provided < MPI_THREAD_FUNNELED) {
    printf("Error: MPI threading level is not enough.\n");
    MPI_Finalize();
    return 1;
  }

    double *A = (double *)malloc(n*n*sizeof(double));
    double *v = (double *)malloc(n*sizeof(double));
    double *w = (double *)malloc(n*sizeof(double));

    int local_n = n / size;
    double *local_A = (double *)malloc(local_n * sizeof(double));

    // Scatter the global array 'A' to local arrays
    MPI_Scatter(A, local_n, MPI_DOUBLE, local_A, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    #pragma omp parallel for simd
    for (int i = 0; i < local_n; ++i) {
        v[i] = 1.0 + 2.0 / (i + 0.5);
        w[i] = 1.0 - i / (3.0 * n);
    }

    // Compute the result locally using OpenMP
    double local_result = 0.0;
#pragma omp parallel for reduction(+:local_result) simd
    for (int i = 0; i < local_n; ++i) {
      for (int j = 0; j < n; ++j) {
        local_result += v[i] * local_A[i] * w[j];
      }
    }

    // Gather the results from all processes
    double *results = NULL;
    if (rank == 0) {
      results = (double *)malloc(size * sizeof(double));
    }
    //MPI_Gather(&local_result, 1, MPI_DOUBLE, results, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Allreduce(&local_result, &results,1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    // Print the result on the root process
    if (rank == 0) {
      double total_result = 0.0;
      for (int p = 0; p < size; ++p) {
        total_result += results[p];
      }
      printf("Total Result = %lf\n", total_result);
      free(results);
    }

    // Free memory
    free(local_A);
    free(A);
    free(v);
    free(w);

    MPI_Finalize();

    return 0;
}
