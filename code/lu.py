def identity_matrix(n):
    matrix = [[0 for x in range(n)] for y in range(n)]
    for i in range(n):
        matrix[i][i] = 1

    return matrix

def lu_decomposition(a):
    n = len(a)
    
    l = identity_matrix(n)
    u = identity_matrix(n)

    for k in range(n):
        u[k][k] = a[k][k]

        for i in range(k + 1, n):
            # v
            l[i][k] = a[i][k] / a[k][k]

            # w
            u[k][i] = a[k][i]

        # Shur complement
        for i in range(k + 1, n):
            for j in range(k + 1, n):
                a[i][j] = a[i][j] - l[i][k] * u[k][j]

    return l, u


a = [
    [1, 2, 1],
    [2, 1, 1],
    [1, -1, 2]
]

l, u = lu_decomposition(a)
print(l)
print(u)
