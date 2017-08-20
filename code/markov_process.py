import numpy as np


CPT = np.array([
    [0.9, 0.3],
    [0.1, 0.7]
])
X = np.array([1.0, 0.0])
n = 100


for i in range(n):
    X = CPT.dot(X)
    print "Iteration #%d: " % (i + 1), X

