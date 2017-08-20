import random


def partition(arr, p, q):
    """Deterministic partition"""
    pivot = arr[p]
    i = p
    for j in range(i + 1, q):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i], arr[p] = arr[p], arr[i]
    return i


def rand_partition(arr, p, q):
    """Randomized partition"""
    i = random.randrange(p, q)
    arr[p], arr[i] = arr[i], arr[p]
    return partition(arr, p, q)


def _rselect(arr, p, q, k):
    """Find k-th order statistic"""
    i = rand_partition(arr, p, q)

    if i == k:
        return arr[i]
    elif k < i:
        return _rselect(arr, p, i, k)
    else:
        return _rselect(arr, i + 1, q, k)


def rselect(arr, k):
    arr_copy = list(arr)
    return _rselect(arr_copy, 0, len(arr_copy), k)
