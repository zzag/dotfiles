def is_a_solution(a, k, data):
    return k == len(data)


def process_solution(a, k, data):
    subset = map(lambda x: x[1],
                 filter(lambda p: p[0],
                        zip(a, data)))
    print('{', ', '.join(str(i) for i in subset), '}')


def construct_candidates(a, k, data):
    return [False, True]


def backtrack(a, k, data):
    if is_a_solution(a, k, data):
        process_solution(a, k, data)
    else:
        candidates = construct_candidates(a, k, data)
        for c in candidates:
            a[k] = c
            backtrack(a, k + 1, data)


def all_subsets(data):
    a = [False for __ in data]
    backtrack(a, 0, data)
