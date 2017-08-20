def combinations(it, r):
    indices = list(range(r))
    items = tuple(it)
    n = len(items)
    C = [items[:r]]
    while indices[0] < n-r:
        for i in reversed(range(r)):
            if indices[i] != i+n-r:
                break
        indices[i] += 1
        for j in range(i+1, r):
            indices[j] = indices[j-1] + 1
        C.append(tuple(items[i] for i in indices))
    return C


print(combinations(range(5), 1))
print(combinations(range(5), 2))
print(combinations(range(5), 3))
print(combinations(range(5), 4))
