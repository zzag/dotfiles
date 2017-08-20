import lagrange
from pylab import *


points = [
    [-4.0, 7.0],
    [0.0, -1.0],
    [1.0, 1.0],
    [2.0, 4.0],
    [3.0, 9.0],
    [5.0, -10.0]
]

x = np.linspace(-5.0, 5.0, 1000)
y = lagrange.lagrange(points=points, x=x)


plot(x, y)
scatter(*zip(*points))
show()

