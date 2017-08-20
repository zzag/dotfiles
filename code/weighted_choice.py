import random
import bisect
import itertools


def choice(choices):
    """
    Weighted random choice

    choices = [(c, w), (c, w), (c, w)]
    """
    weights = map(lambda c: c[1], choices)
    weight_sum = tuple(itertools.accumulate(weights))
    at = random.random() * weight_sum[-1]
    idx = bisect.bisect_right(weight_sum, at)
    return choices[idx][0]
