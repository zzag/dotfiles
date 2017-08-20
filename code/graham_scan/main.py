import graham_scan
import matplotlib.pyplot as plt


points = [
    [4.5, 3.5],
    [4.0, 2.0],
    [5.5, 4.0],
    [6.0, 2.5],
    [3.5, 3.0],
    [3.5, 4.5],
    [4.0, 4.0],
    [5.0, 3.0],
    [3.0, 2.3],
    [2.0, 3.0]
]

convex_hull = graham_scan.graham_scan(points)
convex_hull.append(convex_hull[0])


# plot points
plt.scatter(*zip(*points))

# plot convex hull
plt.plot(*zip(*convex_hull))

plt.show()

