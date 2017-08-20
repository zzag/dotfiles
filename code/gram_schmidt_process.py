import numpy as np


def gram_schmidt(vectors):
    basis = np.array(vectors)
    n = basis.shape[0]

    for i in range(n):
        basis[i] = basis[i] / np.linalg.norm(basis[i])
        for j in range(i + 1, n):
            basis[j] -= basis[i].dot(basis[j]) * basis[i]

    return basis


vectors = np.array([
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
], dtype="float32")

print(gram_schmidt(vectors))

