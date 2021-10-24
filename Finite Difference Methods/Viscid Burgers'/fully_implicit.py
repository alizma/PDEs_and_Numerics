import numpy as np
#import time

import matplotlib.pyplot as plt

def f(x): 
    return x - np.pi 
h = 1 / 100 
k = 1/33 
eta = 0.005
T = 50 
time_steps = round(T / k)
space_steps =  round(5 / h)
x_axis = np.linspace(0, 5, space_steps)
t_axis = np.linspace(0, 50, time_steps)
init_values = [f(x) for x in x_axis]
sigma = k / (h**2)

def Fully_Implicit():
    #ti = time.perf_counter()
    v = np.zeros([time_steps, space_steps])
    v[0, :] = init_values
    cof = eta * sigma
    half_lam = k / (2 * h)
    
    for t in range(time_steps - 1):
        coeff_matrix = np.zeros([space_steps, space_steps])
        for n in range(space_steps):
            if n == 0:
                coeff_matrix[n, space_steps - 1] = -cof - half_lam * v[t, 0]
                coeff_matrix[n, 0] = 1 + 2*cof
                coeff_matrix[n, 1] = -cof + half_lam * v[t, 0]
            elif n == space_steps - 1:
                coeff_matrix[n, n - 1] = -cof - half_lam * v[t, n]
                coeff_matrix[n, n] = 1 + 2*cof
                coeff_matrix[n, 0] =  -cof + half_lam * v[t, n]
            else:
                coeff_matrix[n, n - 1] = -cof - half_lam * v[t, n]
                coeff_matrix[n, n] = 1 + 2*cof
                coeff_matrix[n, n + 1] = -cof + half_lam * v[t, n]
        coeff_matrix = np.linalg.inv(coeff_matrix)
        v[t+1, :] = np.matmul(coeff_matrix, v[t, :])
    #tf = time.perf_counter()
    #print(f"Fully Implicit Time: {ti - tf:0.4f}")
    return v

def main(): 
    v = Fully_Implicit()

    plt.plot(x_axis, v[0].T, label=" t = %d " % 0)
    plt.plot(x_axis, v[10].T, label=" t = %d " % 1)
    plt.plot(x_axis, v[50].T, label=" t = %d " % 5)
    plt.plot(x_axis, v[100].T, label=" t = %d " % 10)
    plt.plot(x_axis, v[250].T, label=" t = %d " % 25)
    plt.plot(x_axis, v[500].T, label="t = %d " % 50)
    plt.ylabel('u(x)')
    plt.xlabel('x')
    plt.title('Fully Implicit Viscid Burgers, at $\eta = 0.005$')
    plt.legend()
    plt.show()