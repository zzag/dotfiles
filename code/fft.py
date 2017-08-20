import numpy as np


def fft(x):
    N, *_ = x.shape

    if N % 2 != 0:
        raise 'is not power of 2'
    elif N < 16:
        n = np.arange(N)
        k = np.reshape(n, (N, 1))
        D = np.exp(-2j * np.pi*k*n/N)
        return np.dot(D, x)
    else:
        xe = fft(x[::2])
        xo = fft(x[1::2])
        w = np.exp(-2j * np.pi*np.arange(N//2)/N)
        return np.concatenate([xe + w*xo, xe - w*xo])


def _ifft(X):
    N = X.shape[0]

    if N % 2 != 0:
        raise 'is not power of 2'
    elif N < 16:
        n = np.arange(N)
        k = np.reshape(n, (N, 1))
        ID = np.exp(2j * np.pi*k*n/N)
        return np.dot(ID, X)
    else:
        xe = _ifft(X[::2])
        xo = _ifft(X[1::2])
        w = np.exp(2j * np.pi*np.arange(N//2)/N)
        return np.concatenate([xe + w*xo, xe - w*xo])


def ifft(X):
    N = X.shape[0]
    return _ifft(X)/N
