#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void do_work(int i) {
    printf("processing %d\n", i);
    sleep(5);
}

int main(int argc, char** argv) {
    double start_time, end_time, elapsed_time;
    int rank, size, M = 2;  // two tasks per process
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    start_time = MPI_Wtime();

    if (rank == 0)
        printf("Running with %d MPI processes\n", size);

    int N = M * size;
    int* all_inputs = NULL;
    int* local_inputs = (int*)malloc(M * sizeof(int));

    if (rank == 0) {
        all_inputs = (int*)malloc(N * sizeof(int));
        srand48(time(0));
        for (int i = 0; i < N; i++) {
            all_inputs[i] = lrand48() % 1000;  // some random value
        }
    }

    MPI_Scatter(all_inputs, M, MPI_INT, local_inputs, M, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < M; i++) {
        do_work(local_inputs[i]);
    }

    end_time = MPI_Wtime();
    elapsed_time = end_time - start_time;

    free(local_inputs);
    if (rank == 0) {
        printf("Elapsed time: %f seconds\n", elapsed_time);
        free(all_inputs);
    }

    MPI_Finalize();
    return 0;
}