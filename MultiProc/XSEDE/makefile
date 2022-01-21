EXECS = laplace 
MPICC = mpicc 

all: ${EXECS}
laplace: laplace.c 
	${MPICC} -o laplace laplace.c -lm

clean: 
	rm -f ${EXECS} 