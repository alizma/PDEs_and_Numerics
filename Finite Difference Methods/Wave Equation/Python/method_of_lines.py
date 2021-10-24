import numpy as np 

def f(x): 
    return x - np.pi

h = 1/100 
k = 1/300 

x = np.linspace(0, 2 * np.pi, 100)
t = np.linspace(0, 1, 300)

xs = x.size()
ts = t.size() 

Q6 = np.zeros(xs, xs) 
Q4 = np.zeros(xs, xs)

Q6[1,2] = 45
Q6[1,3] = -9
Q6[1,4] = 1
Q6[1,xs] = -45
Q6[1,xs-1] = 9
Q6[1,xs-2] = -1
Q6[2,3] = 45
Q6[2,4] = -9
Q6[2,5] = 1
Q6[2,1] = -45
Q6[2,xs] = 9
Q6[2,xs-1] = -1
Q6[3,4] = 45
Q6[3,5] = -9
Q6[3,6] = 1
Q6[3,2] = -45
Q6[3,1] = 9
Q6[3,xs] = -1
Q6[xs-2,xs-1] = 45
Q6[xs-2,xs] = -9
Q6[xs-2,1] = 1
Q6[xs-2,xs-3] = -45
Q6[xs-2,xs-4] = 9
Q6[xs-2,xs-5] = -1
Q6[xs-1,xs] = 45
Q6[xs-1,1] = -9
Q6[xs-1,2] = 1
Q6[xs-1,xs-2] = -45
Q6[xs-1,xs-3] = 9
Q6[xs-1,xs-4] = -1
Q6[xs,1] = 45
Q6[xs,2] = -9
Q6[xs,3] = 1
Q6[xs,xs-1] = -45
Q6[xs,xs-2] = 9
Q6[xs,xs-3] = -1

for n in range(4, xs-3): 
    Q6[n, n-3] = -1
    Q6[n, n-2] = 9
    Q6[n, n-1] = -45
    Q6[n, n+1] = 45
    Q6[n, n+2] = -9
    Q6[n, n+3] = 1

Q4[1,2] = 8
Q4[1,3] = -1
Q4[1,xs] = -8
Q4[1,xs-1] = 1
Q4[2,3] = 8
Q4[2,4] = -1
Q4[2,1] = -8
Q4[2,xs] = 1
Q4[xs-1,xs] = 8
Q4[xs-1,1] = -1
Q4[xs-1,xs-2] = -8
Q4[xs-1,xs-3] = 1
Q4[xs,1] = 8
Q4[xs,2] = -1
Q4[xs,xs-1] = -8
Q4[xs,xs-2] = 1

for n in range(3, xs-2): 
    Q4[n, n-2] = 1
    Q4[n, n-1] = -8
    Q4[n, n+1] = 8
    Q4[n, n+2] = -1

Q6 = Q6 / (60 * h)
Q4 = Q4 / (12 * h)

A_six = np.zeros(xs, xs) 
storage = np.identity(xs)

for j in range(0, 4): 
    A_six += ((k^j) * (Q6)^j)/(np.math.factorial(j))
    storage *= Q6

storage = np.identity(xs) 
A_four = np.zeros(xs, xs)

for j in range(0, 4): 
    A_four += ((k^j) * (Q4)^j)/(np.math.factorial(j))
    storage *= Q4  

def method_of_lines(f, A): 
    v = np.zeros(ts, xs)
    v[1, :] = f(x) 
    for n in range(1, ts - 1): 
        v[n+1, :] = A * v[n, :]

    return (v, x, t)