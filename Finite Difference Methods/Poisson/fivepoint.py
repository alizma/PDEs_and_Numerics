import numpy as np 
import matplotlib.pyplot as plt 

def u_exact(x1, x2):
    return (np.sin(x1) * np.cos(x1)) * x2 * x2

 