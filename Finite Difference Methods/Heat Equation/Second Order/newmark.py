import numpy as np
import time

import matplotlib.pyplot as plt

def f(x): 
    return np.sin(x)

def g(x): 
    return np.cos(x)

h = 1 / 100 
c = 1 
k = 1/ 200 
T = 1 
time_steps = round(T / k)
space_steps = round(5 / k) 
x_axis = np.linspace(0, 2 * np.pi, space_steps)
y_axis = np.linspace(0, 1, time_steps)
init_f = np.array([f(x) for x in x_axis])
init_g = np.array([g(x) for x in x_axis])

def Newmark():
    cof = (k*c**2) / (2 * h**2)
    u, w = np.zeros([time_steps, space_steps]), np.zeros([time_steps, space_steps])
    coeff_matrix = np.zeros([space_steps,space_steps])

    u[0, :], w[0, :]  = init_f, init_g
    for n in range(space_steps):
        if n == 0:
            coeff_matrix[n, 1] = coeff_matrix[n, space_steps - 1] = -cof
        elif n == space_steps - 1:
            coeff_matrix[n, n - 1] = coeff_matrix[n, 0] =  -cof
        else:
            coeff_matrix[n, n - 1] = coeff_matrix[n, n + 1] = -cof
        coeff_matrix[n, n] = 2/k + 2*cof
    coeff_matrix = np.linalg.inv(coeff_matrix)
    for t in range(time_steps - 1):
        right = np.ones(space_steps)
        for j in range(space_steps):
            if j == 0:
                r = 2*w[t, j] + cof*(u[t, space_steps-1] - 2*u[t,j] + u[t, j+1]) + 2/k*u[t,j]
            elif j == space_steps-1:
                r = 2*w[t, j] + cof*(u[t, j-1] - 2*u[t,j] + u[t, 0]) + 2/k*u[t,j]
            else:
                r = 2*w[t, j] + cof*(u[t, j-1] - 2*u[t,j] + u[t, j+1]) + 2/k*u[t,j]
            right[j] = r
        u[t+1, :] = np.matmul(coeff_matrix, right)
        w[t+1, :] = 2/k *(u[t+1,:] - u[t, :]) - w[t, :]
    return u

def main(): 
    v = Newmark()
    print(v.shape) 
     
    for k in range (0, 50, 13): 
        t = k / 50
        plt.plot(x_axis, v[k].T, label = "t = %f" % t)
    plt.ylabel('u(x)')
    plt.xlabel('x')
    plt.title('Newmark, with $k/h = 2$')
    plt.legend()
    plt.show()

if __name__ == '__main__':
    main() 