def median(arr):
    n = len(arr)
    return sorted(arr)[n // 2]


def partition(arr):
    pivot = arr[0]
    i = 0
    for j in range(i + 1, len(arr)):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i], arr[0] = arr[0], arr[i]
    return i


def find_median_of_medians(arr):
    medians = [median(arr[i:i+5]) for i in range(0, len(arr), 5)]
    n = len(medians)
    if n < 5:
        return median(medians)
    return dselect(medians, n // 2)


def dselect(arr, k):
    # HACK: `dselect` returns only an item from the list.
    #       So, we need somehow to get index of the item in
    #       order to swap with the first item in the list and
    #       then do partition.
    #       It's a little bit of hacky but still working :)
    pivot = arr.index(find_median_of_medians(arr))

    arr[pivot], arr[0] = arr[0], arr[pivot]
    i = partition(arr)

    if i == k:
        return arr[i]
    elif k < i:
        return dselect(arr[:i], k)
    else:
        return dselect(arr[i+1:], k - i - 1)
