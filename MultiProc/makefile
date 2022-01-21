EXECS = modular_greeter 
MPICC = mpicc 

all: ${EXECS}
modular_greeter: modular_greeter.c 
	${MPICC} -o modular_greeter modular_greeter.c 

clean: 
	rm -f ${EXECS} 