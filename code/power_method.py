import numpy as np


def power_method(M):
    b = np.ones(M.shape[0])

    for i in range(10):
        _b = b / abs(b).max()
        b = M.dot(_b)

    eigenvalue = b.dot(M.dot(b)) / b.dot(b) # Rayleigh quotient
    eigenvector = b / np.linalg.norm(b)
    return eigenvalue, eigenvector


M = np.array([
    [1, 2],
    [3, 4]
], dtype="float32")

eigenvalue, eigenvector = power_method(M)
print("Dominant eigenvalue:", eigenvalue)
print("Eigenvector:", eigenvector)
print("M*v =", M.dot(eigenvector))
print("lambda*v =", eigenvector * eigenvalue)

