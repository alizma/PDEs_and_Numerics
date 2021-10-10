import numpy as np
import matplotlib.pyplot as plt

class Crank_Nicholson: 

    def __init__(self, h, k, tf, f, t0=0, x0=0, xf=2*np.pi):
       self.h = h
       self.k = k
       self.f = f
       self.time_steps = round((tf - t0) / k)
       self.space_steps = round((xf - x0) / h)
       self.x_axis = np.linspace(x0, xf, self.space_steps)
       self.t_axis = np.linspace(t0, tf, self.time_steps)
       self.CFL = k/h
       self.exact_sol = np.zeros([self.time_steps, self.space_steps])

    def crank_nicholson(self, t0):
        self.time_steps = self.space_steps
        tf = self.time_steps * self.k 

        self.t_axis = np.linspace(t0, tf, self.time_steps)
        self.exact_sol = np.zeros([self.time_steps, self.space_steps])
        self.exact()

        l_op_mat = np.zeros([self.time_steps, self.space_steps])
        r_op_mat = np.zeros([self.time_steps, self.space_steps])
        v = np.zeros([self.time_steps, self.space_steps])
        v[0, :] = self.exact_sol[0, :]

        for n in range(self.time_steps):
            if n == 0:
                l_op_mat[n, 0] = 1
                l_op_mat[n, 1] = -self.CFL/4
            elif n == self.time_steps - 1:
                l_op_mat[n, n - 1] = self.CFL/4
                l_op_mat[n, n] = 1
            else:
                l_op_mat[n, n - 1] = self.CFL/4
                l_op_mat[n, n] = 1
                l_op_mat[n, n + 1] = -self.CFL/4

        for n in range(self.time_steps):
            if n == 0:
                r_op_mat[n, 0] = 1
                r_op_mat[n, 1] = self.CFL/4
            elif n == self.time_steps - 1:
                r_op_mat[n, n - 1] = -self.CFL/4
                r_op_mat[n, n] = 1
            else:
                r_op_mat[n, n - 1] = -self.CFL/4
                r_op_mat[n, n] = 1
                r_op_mat[n, n + 1] = self.CFL/4

        l_op_mat = np.linalg.inv(l_op_mat)
        for n in range(self.time_steps - 1):
            v[n+1, :] = np.matmul(np.matmul(l_op_mat, r_op_mat), v[n, :])

        return v