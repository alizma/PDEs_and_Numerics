import numpy as np 
import matplotlib.pyplot as plt 


def f(x): 
    y = np.zeros(np.size(x, 0))

    x1 = x[(x >= 0) & (x <= np.pi)]
    y[(x >= 0) & (x <= np.pi)] = x1 

    x2 = x[(x > np.pi) & (x < 2 * np.pi)]
    y[(x > np.pi) & (x < 2 * np.pi)] = x2 

    y[x<0] = 0
    y[x >= (2 * np.pi)] = 0

    return y 

def construct_matrix(t_0, t_f, time_step, spatial_step): 
    h = 2*np.pi / spatial_step
    x = np.linspace(0, 2*np.pi, spatial_step)
    
    k = (t_f - t_0) / time_step 
    t = np.linspace(t_0, t_f, time_step)

    CFL = k/h 

    u = np.zeros((spatial_step+1, time_step+1))
    u_exact = np.zeros((spatial_step+1, time_step+1))
    u[:, 0] = np.append(f(x), [0], axis=0)
    u_exact[:, 0] = u[:, 0]

    for j in range(0, time_step): 
        for k in range(1, spatial_step): 
            u[k, j+1] = u[k,j] - (CFL/2) * (u[k+1, j] -  u[k-1, j])
    
        u[0,j+1] = 1 
        u[spatial_step, j+1] = 0
        X = x-t[j]
        u_exact[:, j+1] = np.append(f(X), [0], axis=0)

    return (u, u_exact, x, t)


if __name__ == '__main__': 
    (u,u_exact, x,t) = construct_matrix(0, 1, 128, 256)
    rel_u = u[:, 101][0:256]
    rel_u_exact = u[:, 101][0:256]
    plt.plot(x, rel_u, c='b')
    plt.plot(x, rel_u_exact, c='g')
    plt.show()


