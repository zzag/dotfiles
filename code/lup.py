def identity_matrix(n):
    matrix = [[0 for j in range(n)] for i in range(n)]

    for i in range(n):
        matrix[i][i] = 1

    return matrix


def lup_decomposition(a):
    n = len(a)
    p = [i for i in range(n)]

    for i in range(n):
        max_value = 0
        max_position = 0

        for j in range(i, n):
            if abs(a[j][i]) > max_value:
                max_value = abs(a[j][i])
                max_position = j

        if max_value == 0:
            return None, None, None

        p[i], p[max_position] = p[max_position], p[i]
        a[i], a[max_position] = a[max_position], a[i]

        for j in range(i + 1, n):
            a[j][i] = a[j][i] / a[i][i]
            for k in range(i + 1, n):
                a[j][k] = a[j][k] - a[j][i] * a[i][k]

    # extract l and u
    l = identity_matrix(n)
    u = identity_matrix(n)

    for i in range(n):
        for j in range(i):
            l[i][j] = a[i][j]

    for i in range(n):
        for j in range(i, n):
            u[i][j] = a[i][j]

    return l, u, p

a = [
    [2, 0, 2, 0.6],
    [3, 3, 4, -2],
    [5, 5, 4, 2],
    [-1, -2, 3.4, -1]
]

l, u, p = lup_decomposition(a)
print(l)
print(u)
print(p)
