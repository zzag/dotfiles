def knapsack(weights, values, max_weight):
    """0/1 knapsack problem."""
    n = len(values)
    T = [[0] * (max_weight + 1) for _ in range(n + 1)]

    for i in range(n + 1):
        for w in range(max_weight + 1):
            if i == 0 or w == 0:
                continue
            elif weights[i - 1] <= w:
                T[i][w] = max(values[i - 1] + T[i - 1][w - weights[i - 1]],
                              T[i - 1][w])
            else:
                T[i][w] = T[i - 1][w]

    return T[n][max_weight]
