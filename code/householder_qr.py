import numpy as np


def qr(A):
    m, n = A.shape
    Q = np.eye(m)
    R = A.copy()

    for i in range(m):
        x = R[i:, i]
        alpha = -np.linalg.norm(x)
        v = x.copy()
        v[0] += alpha
        v /= np.linalg.norm(v)
        Qi = np.eye(m)
        Qi[i:, i:] = np.eye(m - i) - 2.0 * np.outer(v, v) / np.dot(v, v)
        Q = Q.dot(Qi.T)
        R = Qi.dot(R)

    return Q, R
