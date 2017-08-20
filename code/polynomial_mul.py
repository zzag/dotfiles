import itertools
import numpy as np


def mul_naive(a, b):
    n = len(a) + len(b) - 1
    a_ = np.pad(a, ((0, n - len(a)),), 'constant')
    b_ = np.pad(b, ((0, n - len(b)),), 'constant')
    c = np.zeros(n)
    for k in range(n):
        c[k] = sum(a_[j]*b_[k-j] for j in range(k + 1)) # a[j]*b[k-j] looks like convolution :)
    return c


def fft(x):
    N = x.shape[0]
    if N % 2 != 0:
        raise 'not power of 2'
    elif N <= 16:
        n = np.arange(N)
        k = np.reshape(n, (N, 1))
        D = np.exp(-2j*np.pi*k*n/N)
        return np.dot(D, x)
    else:
        xe = fft(x[::2])
        xo = fft(x[1::2])
        w = np.exp(-2j*np.pi*np.arange(N//2)/N)
        return np.concatenate([xe + w*xo, xe - w*xo])


def _ifft(X):
    N = X.shape[0]
    if N % 2 != 0:
        raise 'not power of 2'
    elif N <= 16:
        n = np.arange(N)
        k = np.reshape(n, (N, 1))
        ID = np.exp(2j*np.pi*k*n/N)
        return np.dot(ID, X)
    else:
        xe = _ifft(X[::2])
        xo = _ifft(X[1::2])
        w = np.exp(2j*np.pi*np.arange(N//2)/N)
        return np.concatenate([xe + w*xo, xe - w*xo])


def ifft(X):
    return _ifft(X)/X.shape[0]


def mul_fft(a, b):
    n = len(a) + len(b) - 1
    p = 1 << (n - 1).bit_length()
    A = fft(np.pad(a, ((0, p - len(a)),), 'constant'))
    B = fft(np.pad(b, ((0, p - len(b)),), 'constant'))
    c = ifft(A*B)
    return c[:n]


a = np.random.randint(1, 5, 8)
b = np.random.randint(1, 5, 10)
mn = mul_naive(a, b)
mf = np.abs(mul_fft(a, b))

np.set_printoptions(linewidth=100)
print('Naive == FFT: %s' % str(np.allclose(mn, mf)))
print('%20s: %s' % ('Naive', mn))
print('%20s: %s' % ('FFT', mf))
