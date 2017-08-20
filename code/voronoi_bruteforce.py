import numpy as np
from matplotlib import pyplot as plt
import math


def euclidean_distance(p0, p1):
    return math.sqrt((p0[0] - p1[0])**2 + (p0[1] - p1[1])**2)


def manhattan_distance(p0, p1):
    return abs(p0[0] - p1[0]) + abs(p0[1] - p1[1])


def canberra_distance(p0, p1):
    return abs(p0[0] - p1[0])/(abs(p0[0]) + abs(p1[0])) + abs(p0[1] - p1[1])/(abs(p0[1]) + abs(p1[1]))


def voronoi_diagram(size, centroids, metric):
    width, height = size
    img = np.zeros((height, width, 3))

    for i in range(height):
        for j in range(width):
            p = j, i
            distances = list(map(lambda centroid: metric(centroid[0], p), centroids))
            centroid_index = distances.index(min(distances))
            img[i, j, :] = centroids[centroid_index][1]

    return img


image_size = 1024, 1024
centroids = []

for i in range(15):
    pos = np.random.randint(0, image_size[0]), np.random.randint(0, image_size[1])
    color = np.random.randn(3)*0.5 + 0.5
    centroids.append((pos, color))

img = voronoi_diagram(image_size, centroids, euclidean_distance)

centroid_positions = [centroid[0] for centroid in centroids]
plt.imshow(img)
plt.scatter(*zip(*centroid_positions), color="white")
plt.show()

