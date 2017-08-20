def is_a_solution(a, k, data):
    return k == len(data)


def process_solution(a, k, data):
    perm = map(lambda i: data[a[i]], range(k))
    print('{', ', '.join(str(x) for x in perm), '}')


def construct_candidates(a, k, data):
    return filter(lambda x: x not in a[:k],
                  range(len(data)))


def backtrack(a, k, data):
    if is_a_solution(a, k, data):
        process_solution(a, k, data)
    else:
        candidates = construct_candidates(a, k, data)
        for c in candidates:
            a[k] = c
            backtrack(a, k + 1, data)


def permutations(data):
    a = [0 for __ in range(len(data))]
    backtrack(a, 0, data)
