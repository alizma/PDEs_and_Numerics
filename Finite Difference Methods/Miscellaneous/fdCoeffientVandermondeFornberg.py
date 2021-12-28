# https://epubs.siam.org/doi/10.1137/S0036144596322507 algorithm due to Fornberg '98 

import numpy as np

def stablefdcoefficients(k, xbar, x): 
    n = x.size # length of the list of values to be interpolated on 

    if k >= n: 
        raise ValueError('length(x) must be greater than k')

    m = k # computing k-th derivative, change to to n-1 if computing all coefficients for all possible derivatives 

    c1 = 1
    c4 = x[1] - xbar 
    C = np.zeros((n-1, m+1))
    C[1][1] = 1

    for i in range(1, n): 
        mn = min(i, m)
        c2 = 1
        c5 = c4
        c4 = x[i+1] - xbar 
        for j in range(0, i): 
            c3 = x[i+1] - x[j+1] 
            c2 = c2*c3 
            if j == i-1: 
                for s in range(mn, 1, -1): 
                    C[i+1][s+1] = c1 * (s * C[i][s] - c5 * C[i][s+1]) / c2

                C[i+1][1] = (-1) * c1 * c5 * C[i][1] / c2 

                for s in range(mn, 1, -1): 
                    C[j+1][s+1] = (c4*C[j+1][s+1] - s*C[j+1][s]) / c3

                C[j+1][1] = c4*C[j+1][1] / c3 

        c1 = c2

    return C[:, -1] # last column 

