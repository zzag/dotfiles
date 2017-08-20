import cubic_spline
import numpy as np
import pylab as plt


knots = np.array([
    # [-1.0, 3.0],
    # [0.0, 0.0],
    # [1.0, 3.0],
    # [2.5, 3.0],
    # [4.0, 4.0],
    # [5.0, 5.0],
    # [6.0, -1.0]
    [-3.0, 9.0],
    [-2.0, 4.0],
    [-1.0, 1.0],
    [0.0, 0.0],
    [1.0, 1.0],
    [2.0, 4.0],
    [3.0, 9.0]
])

spline = cubic_spline.CubicSpline(knots)

x = np.linspace(np.min(knots, axis=0)[0], np.max(knots, axis=0)[0], 1000)
y = cubic_spline.cubic_spline(x=x, spline=spline)

plt.plot(x, y)
plt.scatter(*zip(*knots))
plt.show()
