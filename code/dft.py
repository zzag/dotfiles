import numpy as np


def dft(x):
    N, *_ = x.shape
    n = np.arange(N)
    k = np.reshape(n, (N, 1))
    D = np.exp(-2j * np.pi*k*n/N)
    return np.dot(D, x)


def idft(X):
    N, *_ = X.shape
    n = np.arange(N)
    k = np.reshape(n, (N, 1))
    ID = np.exp(2j * np.pi*k*n/N)
    return np.dot(ID, X)/N
