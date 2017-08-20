import numpy as np
import newton_polynomial
import pylab as plt


points = np.array([
    # [-1.0, 10.0],
    # [0.0, -6.0],
    # [1.0, 50.0],
    # [2.0, -10.0]
    [-4.0, 16.0],
    [-3.0, 9.0],
    [-2.0, 4.0],
    [-1.0, 1.0],
    [0.0, 0.0],
    [1.0, 1.0],
    [2.0, 4.0],
    [3.0, 9.0],
    [4.0, 16.0]
])


polynomial = newton_polynomial.NewtonPolynomial(points)

x = np.linspace(-4.0, 4.0, 1000)
y = newton_polynomial.newton_polynomial(polynomial=polynomial, x=x)

plt.plot(x, y)
plt.scatter(*zip(*points))
plt.show()
