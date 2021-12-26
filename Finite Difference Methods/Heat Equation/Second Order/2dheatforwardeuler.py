import numpy as np 
import matplotlib.pyplot as plt 

h = 1/20 
k = 1/(4*20*2) 
#k = 1/20

def mesh(x0, xf, y0, yf): 
    x = np.append(np.arange(x0, xf, h), 1)
    y = np.append(np.arange(y0, yf, h), 1)
    x_mesh, y_mesh = np.meshgrid(x, y)
    return x_mesh, y_mesh 

x_mesh, y_mesh = mesh(0, 1, 0, 1)
n = len(x_mesh) - 2
m = int(1/k)

def init(x, y): 
    return x*(x-1)*y*(y-1)

initial_matrix = init(x_mesh[1:-1, 1:-1], y_mesh[1:-1, 1:-1])

def construct_M(): 
    diagonal = np.eye(n )* (-4)
    diagonal += np.diag(np.ones(n-1), -1)
    diagonal += np.diag(np.ones(n-1), 1)

    diagonal= np.kron(np.eye(n), diagonal) + np.diag(np.ones((n-1) * n), n)
    diagonal += np.diag(np.ones((n-1) * n), -n)

    return diagonal  

def forward_euler():
    v_t = [initial_matrix]

    operation_matrix = np.eye(n**2) + (k / h**2) * construct_M()
    for i in range(m): 
        v = np.reshape(v_t[i].T, n**2)
        v = np.reshape(np.matmul(operation_matrix, v), (n, n))
        v_t.append(v)

    return v_t

def backward_euler(): 
    v_t = [initial_matrix]

    operation_matrix = np.linalg.inv(np.eye(n**2) - (k / h**2) * construct_M())

    for i in range(m): 
        v = np.reshape(v_t[i].T, n**2)
        v = np.reshape(np.matmul(operation_matrix, v), (n, n))
        v_t.append(v)

    return v_t 

def get_norm(v): 
    return np.linalg.norm(v, 2)

if __name__ == '__main__': 
    v_t = forward_euler()
    print(get_norm(v_t[1]))
    plt.contourf(x_mesh[1:-1, 1:-1], y_mesh[1:-1, 1:-1], v_t[1])
    plt.colorbar()
    plt.show()