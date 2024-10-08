// sequential code
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    clock_t start_time, end_time;
    double elapsed_time;
	int n = 16384;
    long total_flop = 6 * n * n + 6 * n;
    double flops;
	if (argc == 2)
		n = atoi(argv[1]);

	// allocate memory
	double *A = (double *)malloc(n*n*sizeof(double));
	double *v = (double *)malloc(n*sizeof(double));
	double *w = (double *)malloc(n*sizeof(double));
    start_time = clock(); //start
	/// init A_ij = (i + 2*j) / n^2
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
			A[i*n+j] = (i + 2.0*j) / (n*n);

	/// init v_i = 1 + 2 / (i+0.5)
	for (int i=0; i<n; ++i)
		v[i] = 1.0 + 2.0 / (i + 0.5);

	/// init w_i = 1 - i / (3.*n)
	for (int i=0; i<n; ++i)
		w[i] = 1.0 - i / (3.0*n);

	/// compute
	double result = 0.;
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
			result += v[i] * A[i*n + j] * w[j];
    end_time = clock();

    elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    flops = total_flop / elapsed_time;
    printf("Result = %lf\n", result);
    printf("Elapsed time: %f seconds\n", elapsed_time);
    printf("FLOPs per second: %e\n", flops);
	/// free memory
	free(A);
	free(v);
	free(w);

	return 0;
}
