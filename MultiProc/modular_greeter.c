/* From Pacheco "Parallel Programming with MPI", chapter 3 

The order in which processes send or receive matters; having every process wait until it receives a message effectively 
puts the program into deadlock since no process actually gets to the point in the code where it sends a message. 
*/

#include <stdio.h> 
#include <mpi.h> 
#include <string.h> 

int main(int argc, char *argv[]) { 
    int             my_rank; 
    int             p; 
    int             source; 
    int             dest; 
    int             tag = 0; 
    char            message[100];
    char            temp[100];
    MPI_Status      status;

    MPI_Init(&argc, &argv); 

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &p); 

    sprintf(message, "Greetings from process %d!", my_rank); 

    dest = (my_rank + 1) % p; 
    source = (my_rank - 1) % p;

    MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    MPI_Recv(temp, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status); 
    printf("%s\n", temp);
    
    MPI_Finalize(); 

    return 0; 
}