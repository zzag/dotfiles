import random
import math


def random_points(n):
    for i in range(n):
        yield (random.random(), random.random())


num_epochs = 10000000
inside_circle = sum(1 for p in random_points(num_epochs)
                    if math.hypot(*p) < 1)

pi = 4 * inside_circle / num_epochs
print('PI =', pi)
