def binomial_coefficient(n, k):
    table = [[1 if j == i or j == 0 else 0 for j in range(n + 1)] for i in range(n + 1)]
    for i in range(1, n + 1):
        for j in range(1, i):
            table[i][j] = table[i - 1][j - 1] + table[i - 1][j]
    return table[n][k]
