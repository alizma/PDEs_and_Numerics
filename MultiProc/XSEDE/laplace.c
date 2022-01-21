#include <stdio.h> 
#include <mpi.h> 
#include <string.h> 
#include <math.h> 
#include <sys/time.h> 

#define COLUMNS     1000
#define ROWS        1000

// largest permitted change in temperature in a single time step
#define MAX_TEMP_ERROR 0.01

void init(); 
void track_prog(int iter); 

double Temp[ROWS+2][COLUMNS+2]; 
double Temp_last[ROWS+2][COLUMNS+2]; 

// initialize plate and boundary conditions
void init() { 
    int i, j; 

    for (i = 0; i <= ROWS+1; i++) { 
        for (j = 0; j <= COLUMNS+1; j++) { 
            Temp_last[i][j] = 0.0; 
        }
    }

    for (i = 0; i <= ROWS+1; i++) { 
        Temp_last[i][0] = 0.0; // set left to 0 
        Temp_last[i][COLUMNS+1] = (100.0/ROWS) * i; // set right column to linear increase 
    }

    for (j = 0; j <= COLUMNS+1; j++) { 
        Temp_last[0][j] = 0.0; // set top to 0
        Temp_last[ROWS+1][i] = (100.0/COLUMNS) * j; // set bottom to linear increase
    }
}

// prints out bottom-right corner of plate 
void track_prog(int curr_iter) { 
    int i; 
    printf("-------------- Iteration number: %d --------------\n", curr_iter); 

    for (i = ROWS-5; i<= ROWS; i++) { 
        printf("[%d, %d]: %5.2f  ", i, i, Temp[i][i]); 
    }

    printf("\n");
}

int main(int argc, char** argv) { 

    int i, j; 
    int max_iter; 
    int curr_iter = 1; 
    double dt = 100; 
    struct timeval start_time, stop_time, elapsed_time; // timers 

    printf("Max number of iterations?\n");
    scanf("%d", &max_iter); 

    gettimeofday(&start_time, NULL); 

    init(); 

    while (dt > MAX_TEMP_ERROR && curr_iter <= max_iter) {
        //kernel computation 
        for (i = 1; i <= ROWS; i++) { 
            for (j = 1; j <= ROWS; j++) { 
                Temp[i][j] = 0.25 * (Temp_last[i+1][j] + Temp_last[i-1][j] + Temp_last[i][j+1] + Temp_last[i][j-1]); 
            }
        }

        dt = 0.0; 

        // compute maximum change in temperature and update Temp_last matrix 
        for (i = 1; i <= ROWS; i++) { 
            for (j = 1; j <= COLUMNS; j++) { 
                dt = fmax(fabs(Temp[i][j] - Temp_last[i][j]), dt); 
                Temp_last[i][j] = Temp[i][j]; 
            }
        }

        if ((curr_iter % 100) == 0) { 
            track_prog(curr_iter); 
        }

        curr_iter++; 
    }

    return 0; 
}