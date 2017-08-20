import numpy as np


def lagrange(points, x):
    y = 0.0

    for i, point in enumerate(points):
        y += point[1] * _lagrange_polynomial(points, x, i)

    return y

lagrange = np.vectorize(lagrange, excluded=["points"])


def _lagrange_polynomial(points, x, i):
    value = 1.0

    for j, point in enumerate(points):
        if i == j:
            continue

        value *= (x - point[0]) / (points[i][0] - point[0])

    return value
