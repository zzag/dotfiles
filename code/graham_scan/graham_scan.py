import numpy as np
import math


def turn(x, y, z):
    return (x[0] - y[0]) * (z[1] - y[1]) - (z[0] - y[0]) * (x[1] - y[1])


def graham_scan(points):
    hull = []
    start = min(points, key=lambda x: x[1])
    points = sorted(
            list(filter(lambda x: x != start, points)),
            key=lambda x: math.atan2(x[1] - start[1], x[0] - start[0]))
    
    hull.append(start)
    hull.append(points[0])
    hull.append(points[1])

    for i in points[2:]:
        while turn(hull[-2], hull[-1], i) > 0:
            hull.pop()

        hull.append(i)

    return hull

