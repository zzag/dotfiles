import numpy as np


# create pseudo-random generator
rng = np.random.RandomState(1234)

# draw samples from uniform distribution
shape = (3, 4, 2)
samples = np.asarray(
    rng.uniform(low=-0.5, high=0.5, size=shape),
    dtype=np.float32)

print(samples)

