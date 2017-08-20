import numpy as np


def newton_polynomial(polynomial, x):
    return polynomial.at(x)


newton_polynomial = np.vectorize(newton_polynomial, excluded=["polynomial"])


class NewtonPolynomial:

    def __init__(self, points):
        self.n = len(points)
        self.points = points
        self.c = self._divided_difference()

    def _divided_difference(self):
        c = np.zeros(self.n)

        for i in range(self.n):
            c[i] = self.points[i, 1]

        for i in range(1, self.n):
            for j in range(self.n - 1, i - 1, -1):
                c[j] = (c[j] - c[j - 1]) / (self.points[j, 0] - self.points[j - i, 0])

        return c

    def at(self, x):
        y = self.c[self.n - 1]

        for i in range(self.n - 2, -1, -1):
            y = self.c[i] + (x - self.points[i, 0]) * y

        return y
