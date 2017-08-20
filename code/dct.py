import numpy as np
import math


def dctmtx(N):
    t = np.arange(N)
    k = np.arange(N)
    T = np.outer(0.5 * k * np.pi / N, 2.0 * t + 1.0)
    T = np.cos(T)
    T *= math.sqrt(2.0 / N)
    T[0] *= math.sqrt(1.0 / 2.0)
    return T


def dct(a):
    N = len(a)
    T = dctmtx(N)
    X = T.dot(a)
    return X


def idct(X):
    N = len(X)
    T = dctmtx(N).T
    a = T.dot(X)
    return a
