#include <stdio.h> 
#include <mpi.h> 

int main(int argc, char** argv) { 
    int curr_PE_num, toreceive, tosend=4, idx, result=0; 
    MPI_Status status; 

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &curr_PE_num); 

    if (curr_PE_num == 0) { 
        for (idx = 1; idx < 4; idx++) { 
            MPI_Send(&tosend, 1, MPI_INT, idx, 10, MPI_COMM_WORLD); 
        }
    } else { 
        MPI_Recv(&toreceive, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, &status); 
        result = toreceive * curr_PE_num; 
    }

    for (idx = 1; idx < 4; idx++) { 
        //MPI_Barrier(MPI_COMM_WORLD); 
        if (idx == curr_PE_num) printf("PE %d's result is %d. \n", curr_PE_num, result);
        MPI_Barrier(MPI_COMM_WORLD);  
    }

    if (curr_PE_num == 0) { 
        for (idx = 1; idx < 4; idx++) { 
            MPI_Recv(&toreceive, 1, MPI_INT, idx, 10, MPI_COMM_WORLD, &status); 
            result += toreceive; 
        }
        printf("Total is %d.\n", result); 
    } else { 
        MPI_Send(&result, 1, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }

    MPI_Finalize(); 
}