import numpy as np 
import matplotlib.pyplot as plt 

# the explicit scheme for the 1D heat equation is conditionally stable 
# under the CFL condition dt <= dx^2 * 1/(2v)

# the assumption here is that dt / dx^2 = 1, which violates the CFL condition and leads to blow-up; 
# this can be seen from the larger values that each successive generation of the CA takes on 

powers_of_two = np.array([[4], [2], [1]])  # shape (3, 1)

def step(x, rule_binary):
    x_shift_right = np.roll(x, 1)  # circular shift to right
    x_shift_left = np.roll(x, -1)  # circular shift to left
    y = np.vstack((x_shift_right, x, x_shift_left)).astype(np.int8)  # stack row-wise, shape (3, cols)
    z = np.sum(powers_of_two * y, axis=0).astype(np.int8)  # LCR pattern as number

    return rule_binary[7 - z]

def cellular_automaton(rule_number, size, steps,
                       init_cond='random', impulse_pos='center'):
    rule_binary_str = np.binary_repr(rule_number, width=8)
    rule_binary = np.array([int(ch) for ch in rule_binary_str], dtype=np.int8)
    x = np.zeros((steps, size), dtype=np.int8)

    if init_cond == 'random':  # random init of the first step
        x[0, :] = np.array(np.random.rand(size) < 0.5, dtype=np.int8)

    if init_cond == 'impulse':  # starting with an initial impulse
        if impulse_pos == 'left':
            x[0, 0] = 1
        elif impulse_pos == 'right':
            x[0, size - 1] = 1
        else:
            x[0, size // 2] = 1

    for i in range(steps - 1):
        x[i + 1, :] = step(x[i, :], rule_binary)

    return x

if __name__ == '__main__':
    rule_number = 150 
    size = 100 
    steps = 63
    init_cond='impulse'
    impulse_pos='left'

    x = cellular_automaton(rule_number, size, steps, init_cond, impulse_pos)

    fig = plt.figure(figsize=(10,10))
    ax = plt.axes()
    ax.set_axis_off() 

    ax.imshow(x, interpolation='none',cmap='YlGn')
    plt.savefig('elementary_cellular_automaton.png', dpi=300, bbox_inches='tight')