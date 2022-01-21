#include <mpi.h> 
#include <math.h> 
#include <stdio.h> 

int main(int argc, char **argv) { 
    int n, curr_PE_num, numprocs, idx; 
    float approx_pi, pi, h, x, start, end; 

    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs); 
    MPI_Comm_rank(MPI_COMM_WORLD, &curr_PE_num); 

    if (curr_PE_num == 0) { 
        fprintf(stderr, "How many intervals?\n"); 
        scanf("%d", &n); 
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); 

    approx_pi = 0; 
    h = (float) 2/n; // slice size 
    start   = (curr_PE_num * 2/numprocs) - 1; 
    end     = ((curr_PE_num+1)*2/numprocs) - 1; 

    for (x = start; x < end; x = x+h) { 
        approx_pi = approx_pi + h * 2 * sqrt(1 - x*x); 
    }

    MPI_Reduce(&approx_pi, &pi, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD); 

    if (curr_PE_num == 0) { 
        printf("Pi is approximately %f\n", pi); 
        printf("Error is %f\n", pi-3.14159265358979323856); 
    }

    MPI_Finalize(); 

    return 0; 
}