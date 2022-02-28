#include <stdio.h> 
#include <mpi.h> 
#include<math.h>

int main(int argc, char** argv) { 
    int i, j; 
    


    MPI_Init(&argc, &argv); 

    MPI_FINALIZE(); 

    return 0; 
}