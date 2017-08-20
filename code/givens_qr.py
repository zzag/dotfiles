import numpy as np
import math


def givens(X, i, j):
    a, b = X[j, j], X[i, j]
    c, s = 1.0, 0.0

    if abs(b) > np.finfo(np.float).eps:
        if abs(b) > abs(a):
            r = -a / b
            s = 1 / math.sqrt(1 + r*r)
            c = s * r
        else:
            r = -b / a
            c = 1 / math.sqrt(1 + r*r)
            s = c * r

    G = np.eye(X.shape[0])
    G[i, i] = c
    G[j, j] = c
    G[j, i] = -s
    G[i, j] = s

    return G


def qr(A):
    n = A.shape[0]
    Q = np.eye(n)
    R = A.copy()

    for i in range(1, n):
        for j in range(i):
            G = givens(R, i, j)
            Q = Q.dot(G.T)
            R = G.dot(R)

    return Q, R
