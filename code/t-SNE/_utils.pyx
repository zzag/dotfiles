import numpy as np
cimport numpy as np
from libc cimport math

cdef extern from "numpy/npy_math.h":
    float NPY_INFINITY


cdef float ENTROPY_TOLERANCE = 1e-4
cdef float EPSILON = 1e-8


cpdef np.ndarray[np.float64_t, ndim=2] binary_search_perplexity(
        np.ndarray[np.float64_t, ndim=2] distances,
        float desired_perplexity):
    cdef long num_steps = 100
    cdef long num_samples = distances.shape[0]
    cdef np.ndarray[np.float64_t, ndim=2] P = np.zeros(
        (num_samples, num_samples),
        dtype=np.float64)

    cdef float entropy
    cdef float entropy_diff
    cdef float desired_entropy = math.log(desired_perplexity)

    cdef float sigma
    cdef float sigma_min
    cdef float sigma_max

    cdef float sum_P
    cdef float sum_dist_P

    for i in range(num_samples):
        sigma = 1.0
        sigma_min = -NPY_INFINITY
        sigma_max = NPY_INFINITY

        for t in range(num_steps):
            for j in range(num_samples):
                P[i, j] = math.exp(-distances[i, j] * sigma)
            P[i, i] = 0.0

            sum_P = 0.0
            for j in range(num_samples):
                sum_P += P[i, j]

            if sum_P < EPSILON:
                sum_P = EPSILON

            sum_dist_P = 0.0
            for j in range(num_samples):
                P[i, j] /= sum_P
                sum_dist_P += P[i, j] * distances[i, j]

            entropy = sigma * sum_dist_P + math.log(sum_P)
            entropy_diff = entropy - desired_entropy

            if math.fabs(entropy_diff) <= ENTROPY_TOLERANCE:
                break

            if entropy_diff > 0.0:
                sigma_min = sigma
                if sigma_max == NPY_INFINITY:
                    sigma *= 2.0
                else:
                    sigma = (sigma_min + sigma_max) / 2.0
            else:
                sigma_max = sigma
                if sigma_min == -NPY_INFINITY:
                    sigma /= 2.0
                else:
                    sigma = (sigma_min + sigma_max) / 2.0

    return P
