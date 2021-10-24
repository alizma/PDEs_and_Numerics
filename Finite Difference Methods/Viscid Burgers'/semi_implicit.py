import numpy as np

import matplotlib.pyplot as plt

def f(x): 
    return x - np.pi 
h = 1 / 100 
k = 1/33 
eta = 1
T = 50 
time_steps = round(T / k)
space_steps =  round(5 / h)
x_axis = np.linspace(0, 5, space_steps)
t_axis = np.linspace(0, 50, time_steps)
init_values = [f(x) for x in x_axis]
sigma = k / (h**2)

def Semi_Implicit():
    v = np.zeros([time_steps, space_steps])
    v[0, :], cof, half_lam = init_values, eta * sigma, k / (2 * h)
    coeff_matrix = np.zeros([space_steps, space_steps])

    for n in range(space_steps):
        if n == 0:
            coeff_matrix[n, 1] = coeff_matrix[n, space_steps - 1] = -cof
        elif n == space_steps - 1:
            coeff_matrix[n, n - 1] = coeff_matrix[n, 0] =  -cof
        else:
            coeff_matrix[n, n - 1] = coeff_matrix[n, n + 1] = -cof
        coeff_matrix[n, n] = 1 + 2*cof

    coeff_matrix = np.linalg.inv(coeff_matrix)

    for t in range(time_steps - 1):
        right = np.ones(space_steps)
        for j in range(space_steps):
            if j == 0:
                r = v[t, j] + half_lam * v[t, j] * (v[t, j + 1] - v[t, space_steps-1])
            elif j + 1 == space_steps:
                r = v[t, j] + half_lam * v[t, j] * (v[t, 0] - v[t, j-1])
            else:
                r = v[t, j] + half_lam * v[t, j] * (v[t, j+1] - v[t, j-1])
            right[j] = r
        v[t+1, :] = np.matmul(coeff_matrix, right)
    return v


def main(): 
    v = Semi_Implicit()
    
    for k in range (0, 500, 75): 
        t = k / 10
        plt.plot(x_axis, v[k].T, label = "t = %f" % t)
    plt.plot(x_axis, v[10].T, label=" t = %d " % 1)
    plt.plot(x_axis, v[500].T, label="t = %d " % 50)
    plt.ylabel('u(x)')
    plt.xlabel('x')
    plt.title('Semi Implicit Viscid Burgers, at $\eta = 1$')
    plt.legend()
    plt.show()

if __name__ == '__main__':
    main() 