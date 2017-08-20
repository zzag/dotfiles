import tridiagonal_solver
import numpy as np


def cubic_spline(spline, x):
    return spline.at(x)


cubic_spline = np.vectorize(cubic_spline, excluded=["spline"])


class CubicSpline:

    def __init__(self, knots):
        self.knots = knots
        self.n = len(self.knots)
        self.polynomials = np.zeros((self.n - 1, 4))

        h = np.zeros(self.n - 1)
        k = np.zeros(self.n - 1)
        for i in range(self.n - 1):
            h[i] = self.knots[i + 1][0] - self.knots[i][0]
            k[i] = self.knots[i + 1][1] - self.knots[i][1]

        y = self._solve_y(h, k)
        self._build_polynomials(h, k, y)

    def _solve_y(self, h, k):
        a = np.hstack((0.0, h[1:]))
        b = np.zeros(self.n - 2)
        c = np.hstack((h[1:], 0.0))
        d = np.zeros(self.n - 2)

        for i in range(self.n - 2):
            b[i] = 2 * (h[i] + h[i + 1])
            d[i] = 6 * (k[i + 1] / h[i + 1] - k[i] / h[i])

        y = tridiagonal_solver.tridiagonal_solve(a, b, c, d)
        y = np.hstack((0.0, y, 0.0))

        return y

    def _build_polynomials(self, h, k, y):
        for i in range(self.n - 1):
            self.polynomials[i, 0] = y[i] / (6 * h[i])
            self.polynomials[i, 1] = y[i + 1] / (6 * h[i])
            self.polynomials[i, 2] = self.knots[i][1] / h[i] - y[i] * h[i] / 6
            self.polynomials[i, 3] = self.knots[i + 1][1] / h[i] - y[i + 1] * h[i] / 6

    def at(self, x):
        for i in range(self.n - 1):
            x_i = self.knots[i][0]
            x_ii = self.knots[i + 1][0]

            if x < x_i or x > x_ii:
                continue

            x_vector = np.array([
                (x_ii - x) ** 3,
                (x - x_i) ** 3,
                x_ii - x,
                x - x_i
            ])

            return np.dot(self.polynomials[i], x_vector)

        return 0.0
