#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void do_work(int i) {
    printf("Processing %d on rank %d\n", i, MPI_Comm_rank(MPI_COMM_WORLD, NULL));
    sleep(5);
}

int main(int argc, char** argv) {
    int rank, size, M = 2;  // two tasks per process
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

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

    MPI_Request requests[M];
    for (int i = 0; i < M; i++) {
        CHECK_MPI(MPI_Isend(&local_inputs[i], 1, MPI_INT, rank, i, MPI_COMM_WORLD, &requests[i]));
    }

    for (int i = 0; i < M; i++) {
        int input;
        MPI_Status status;
        CHECK_MPI(MPI_Recv(&input, 1, MPI_INT, rank, i, MPI_COMM_WORLD, &status));
        do_work(input);
    }

    MPI_Waitall(M, requests, MPI_STATUSES_IGNORE);

    free(local_inputs);
    if (rank == 0) {
        free(all_inputs);
    }

    MPI_Finalize();
    return 0;
}