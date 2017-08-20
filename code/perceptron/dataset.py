import numpy as np

# source:
# http://axon.cs.byu.edu/~martinez/classes/478/stuff/labhints/perceptron.html

data = np.array([
    [-0.50, 0.30],
    [-0.30, -0.10],
    [-0.20, 0.40],
    [-0.10, 0.10],
    [0.14, -0.50],
    [0.20, -0.90],
    [0.30, 1.0],
    [0.50, -0.60]
])

target = np.array([
    1.0,
    1.0,
    1.0,
    1.0,
    0.0,
    0.0,
    0.0,
    0.0
])
