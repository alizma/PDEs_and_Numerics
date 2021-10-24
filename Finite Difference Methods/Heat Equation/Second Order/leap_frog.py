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

def Leap_Frog():
    coeff_matrix = (k**2 * c **2)/(h**2)
    v = np.zeros([time_steps, space_steps])
    v[0, :] = init_f
    v[1, :] = init_f + k * init_g
    for n in range(1, time_steps - 1):
        l = space_steps-1
        v[n+1, 0] = coeff_matrix*v[n,l] + (-2*coeff_matrix+2)*v[n, 0] + coeff_matrix*v[n, 1] - v[n-1, 0]
        v[n+1, l] = coeff_matrix*v[n,l-1] + (-2*coeff_matrix+2)*v[n, l] + coeff_matrix*v[n, 0] - v[n-1, l]
        
        for j in range(1, space_steps - 1):
            v[n+1, j] = coeff_matrix*v[n,j-1] + (-2*coeff_matrix+2)*v[n, j] + coeff_matrix*v[n, j+1] - v[n-1, j]
    return v

def main(): 
    v = Leap_Frog()
    print(v.shape) 
    
    for k in range (0, 50, 18): 
        t = k / 50
        plt.plot(x_axis, v[k].T, label = "t = %f" % t)
    plt.plot(x_axis, v[49].T, label = "t = %f" % 1)
    plt.ylabel('u(x)')
    plt.xlabel('x')
    plt.title('Leap Frog, with $k/h = 1/2$')
    plt.legend()
    plt.show()

if __name__ == '__main__':
    main() 