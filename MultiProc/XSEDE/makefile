EXECS = simpsonpi 
MPICC = mpicc 

all: ${EXECS}
simpsonpi: simpsonpi.c 
	${MPICC} -o simpsonpi simpsonpi.c -lm

clean: 
	rm -f ${EXECS} 