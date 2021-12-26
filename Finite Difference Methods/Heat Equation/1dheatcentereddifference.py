import numpy as np 
import matplotlib.pyplot as plt 

L = 1.0 
nx = 65
dx = L / (nx - 1)
alpha = 1 # thermal diffusivity 

x = np.linspace(0.0, L, num=nx)

# initial temperature 
T0 = np.zeros(nx)
T0[0] = 100.0

def central_scheme(T0, nt, dt, dx, alpha): 
    T = T0.copy()
    CFL = alpha * dt / dx**2
    for n in range(nt): 
        T[1:-1] = (T[1:-1] + CFL * (T[2:] - 2.0 * T[1:-1] + T[:-2]))
    
    return T 

nt = 100 
sigma = 1 # breaks down over CFL condition, set to <= 1/2 for conditional convergence 
dt = sigma * dx**2 / alpha 

T = central_scheme(T0, nt, dt, dx, alpha)

plt.figure(figsize=(6.0, 4.0))
plt.xlabel('Distance in m')
plt.ylabel('Temperature in C')
plt.grid()
plt.plot(x, T, color='C0', linestyle='-', linewidth=2)
plt.xlim(0.0, L)
plt.ylim(0.0, 100.0)

plt.show()