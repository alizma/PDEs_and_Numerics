#include <stdio.h> 
#include <string.h> 
#include <math.h> 
#include <mpi.h> 
#include <sys/time.h> 
#include <stdlib.h> 

#define COLUMNS     1000
#define ROWS_TOTAL  1000
#define NPES        4
#define ROWS_SINGLE (ROWS_TOTAL/NPES)

#define MAX_TEMP_ERROR 0.01

#define UP      100
#define DOWN    101

void init(); 
void track_prog(int iter); 

double Temp[ROWS_SINGLE+2][COLUMNS+2]; 
double Temp_last[ROWS_SINGLE+2][COLUMNS+2]; 

void init(int npe_count, int curr_npe) {
    int i, j; 
    int temp_min, temp_max; 

    for (i = 0; i <= ROWS_SINGLE+1; i++) { 
        for (j = 0; j <= COLUMNS+1; j++) { 
            Temp_last[i][j] = 0.0;
        }
    }

    temp_min = 100.0 * (curr_npe / NPES);
    temp_max = 100.0 * ((curr_npe + 1) / NPES); 

    for (i = 0; i <= ROWS_SINGLE + 1; i++) { 
        Temp_last[i][0] = 0.0; 
        Temp_last[i][COLUMNS+1] = temp_min + ((temp_max - temp_min) / 100) * i; 
    }

    if (curr_npe == 0) {
        for (j = 0; j <= COLUMNS+1; j++) { 
            Temp_last[0][j] = 0.0; 
        } 
    }
    if (curr_npe == npe_count - 1) {
        for (j = 0; j <= COLUMNS+1; j++) {
            Temp_last[ROWS_SINGLE+1][j] = (100.0/COLUMNS) * j; 
        } 
    }
    
}

// prints out bottom-right corner of plate 
void track_prog(int curr_iter) { 
    int i; 
    printf("-------------- Iteration number: %d --------------\n", curr_iter); 

    for (i = 5; i >= 0; i--) { 
        printf("[%d, %d]: %5.2f  ", ROWS_TOTAL - i, COLUMNS - i, Temp[ROWS_SINGLE - i][COLUMNS - i]); 
    }

    printf("\n");
}


int main(int argc, char** argv) { 
    int i, j; 
    int max_iter; 
    int curr_iter = 1; 
    double dt; 
    struct timeval start_time, stop_time, elapsed_time; // timers

    int npe_count; 
    int curr_npe; 
    double dt_global = 100; // maximum time step across all PEs
    MPI_Status status; 

    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &npe_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &curr_npe); 

    if (npe_count != NPES) { 
        if (curr_npe == 0) { 
            printf("Code must be run with %d PEs \n. Aborting.", NPES); 
        }
        MPI_Finalize(); 
        exit(1); 
    }

    if (curr_npe == 0) { 
        printf("Max number of iterations?\n");
        fflush(stdout); 
        scanf("%d", &max_iter); 
    }

    MPI_Bcast(&max_iter, 1, MPI_INT, 0, MPI_COMM_WORLD); 

    if (curr_npe == 0) { 
        gettimeofday(&start_time, NULL); 
    }

    init(npe_count, curr_npe); 

    while (dt_global > MAX_TEMP_ERROR && curr_iter <= max_iter) { 
        for (i = 1; i <= ROWS_SINGLE; i++) { 
            for (j = 1; j <= COLUMNS; j++) { 
                Temp[i][j] = 0.25 * (Temp_last[i+1][j] + Temp_last[i-1][j] + Temp_last[i][j+1] + Temp_last[i][j-1]); 
            }
        }

        // send bottom real row down 
        if (curr_npe != npe_count - 1) { 
            MPI_Send(&Temp[ROWS_SINGLE][1], COLUMNS, MPI_DOUBLE, curr_npe + 1, DOWN, MPI_COMM_WORLD); 
        }

        // receive bottom row from above into top ghost row 
        if (curr_npe != 0) { 
            MPI_Recv(&Temp_last[0][1], COLUMNS, MPI_DOUBLE, curr_npe - 1, DOWN, MPI_COMM_WORLD, &status);
        }

        // send top real row up a processor
        if (curr_npe != 0) { 
            MPI_Send(&Temp[1][1], COLUMNS, MPI_DOUBLE, curr_npe - 1, UP, MPI_COMM_WORLD); 
        }

        // receive top row from lower processor into bottom ghost row on current processor 
        if (curr_npe != npe_count - 1) { 
            MPI_Recv(&Temp_last[ROWS_SINGLE + 1][1], COLUMNS, MPI_DOUBLE, curr_npe + 1, UP, MPI_COMM_WORLD, &status);
        }

        dt = 0.0; 

        for (i = 1; i <= ROWS_SINGLE; i++) { 
            for (j = 1; j <= COLUMNS; j++) { 
                dt = fmax(fabs(Temp[i][j] - Temp_last[i][j]), dt);
                Temp_last[i][j] = Temp[i][j]; 
            }
        }

        // compute global dt 
        MPI_Reduce(&dt, &dt_global, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
        MPI_Bcast(&dt_global, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 

        if((curr_iter % 100) == 0) { 
            if (curr_npe == npe_count-1) { 
                track_prog(curr_iter);
            }
        }

        curr_iter++; 
    } 

    MPI_Barrier(MPI_COMM_WORLD); 
    
    if (curr_npe == 0) { 
        gettimeofday(&stop_time, NULL); 
        timersub(&stop_time, &start_time, &elapsed_time); 
        printf("\nMax error at iteration %d was %f\n", curr_iter -1, dt_global); 
        printf("Total time was %f seconds.\n", elapsed_time.tv_sec + elapsed_time.tv_usec/1000000.0); 
    }
 
    MPI_Finalize(); 

    return 0; 
}