import gift_wrapping
import matplotlib.pyplot as plt


points = [
    [4.5, 3.5],
    [4.0, 2.8],
    [5.5, 4.0],
    [6.0, 2.5],
    [3.5, 3.0],
    [3.5, 4.5],
    [4.0, 4.0],
    [5.0, 3.0],
    [3.0, 2.3],
    [2.0, 3.0]
]


hull = gift_wrapping.gift_wrapping(points)

plt.scatter(*zip(*points))
plt.plot(*zip(*hull))
plt.show()

