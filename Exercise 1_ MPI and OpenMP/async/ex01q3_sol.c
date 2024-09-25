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
    int rank;
    int size;
    double start_time, end_time, elapsed_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    start_time = MPI_Wtime();
    if (rank == 0)
        printf("Running with %d MPI processes\n", size);

    int M = 2;  // Two tasks per process
    int N = M * size;  // Total number of tasks

    int inputs[M];
    MPI_Request send_requests[N];
    MPI_Request recv_requests[M];

    if (rank == 0) {
        srand48(time(0));

        // Send tasks to all processes, including self
        for (int i = 0; i < N; i++) {
            int task = lrand48() % 1000;  // Generate random task value
            MPI_Isend(&task, 1, MPI_INT, i % size, 100, MPI_COMM_WORLD, &send_requests[i]);
        }

        // Wait for all sends to complete
        MPI_Waitall(N, send_requests, MPI_STATUSES_IGNORE);
    }

    for (int i = 0; i < M; i++) {
        MPI_Irecv(&inputs[i], 1, MPI_INT, 0, 100, MPI_COMM_WORLD, &recv_requests[i]);
    }

    MPI_Waitall(M, recv_requests, MPI_STATUSES_IGNORE);

    for (int i = 0; i < M; i++) {
        do_work(inputs[i]);
    }
    end_time = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    //end_time = MPI_Wtime();
    elapsed_time = end_time - start_time;
    if (rank == 0) {
        printf("Elapsed time: %f seconds\n", elapsed_time);
    }
    MPI_Finalize();
    return 0;
}
