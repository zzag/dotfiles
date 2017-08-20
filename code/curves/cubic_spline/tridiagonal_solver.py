import numpy as np


def tridiagonal_solve(a, b, c, d):
    """Solve tridiagonal system

    :param a: [0.0, a]
    :param b: [b]
    :param c: [c, 0.0]
    :param d: [d]
    :return:
    """
    n = len(b)
    b_ = np.zeros(n)
    d_ = np.zeros(n)
    x = np.zeros(n)

    # initial setup
    b_[0] = b[0]
    d_[0] = d[0]

    # forward sweep
    for i in range(1, n):
        b_[i] = b[i] - a[i] * c[i - 1] / b_[i - 1]
        d_[i] = d[i] - a[i] * d_[i - 1] / b_[i - 1]

    # backward sweep
    x[n - 1] = d_[n - 1] / b_[n - 1]

    for i in range(n - 2, -1, -1):
        x[i] = d_[i] / b_[i] - c[i] / b_[i] * x[i + 1]

    return x
