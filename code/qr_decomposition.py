import numpy as np


def qr_decomposition(M):
    n = M.shape[1]
    Q = np.array(M)

    # Gram-Schmidt process
    for i in range(n):
        Q[:, i] /= np.linalg.norm(Q[:, i])

        for j in range(i + 1, n):
            Q[:, j] -= Q[:, j].dot(Q[:, i]) * Q[:, i]
    
    # find R
    R = Q.T.dot(M)

    return Q, R


M = np.array([
    [1, 2],
    [3, 4]
], dtype="float32")
Q, R = qr_decomposition(M)
print(Q)
print(R)

