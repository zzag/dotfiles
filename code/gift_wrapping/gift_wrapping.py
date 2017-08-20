import numpy as np
import math


def _angle(x, y, z):
    x = np.array(x)
    y = np.array(y)
    z = np.array(z)
    xy = x - y
    zy = z - y
    return math.acos(xy.dot(zy) / (np.linalg.norm(xy) * np.linalg.norm(zy)) - 0.001)


def _next(hull, points):
    next_point = None
    angle = -1

    for p in points:
        if p in hull[1:]:
            continue

        # calculate angle
        a = _angle(hull[-2], hull[-1], p)
        if a > angle:
            angle = a
            next_point = p

    return next_point


def gift_wrapping(points):
    hull = []
    s = min(points, key=lambda x: x[1])
    q = max(points, key=lambda x: math.atan2(x[1] - s[1], x[0] - s[0]))
    hull.append(s)
    hull.append(q)

    while True:
        p = _next(hull, points)
        hull.append(p)

        if p == s:
            break


    return hull

