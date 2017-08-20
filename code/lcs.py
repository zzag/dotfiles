def lcs(str1, str2):
    """Longest Common Subsequence problem."""
    n, m = len(str1), len(str2)
    lcs = [[0] * (m + 1) for _ in range(n + 1)]

    for i, char1 in enumerate(str1):
        for j, char2 in enumerate(str2):
            if char1 == char2:
                next_lcs = lcs[i][j] + 1
            elif lcs[i + 1][j] > lcs[i][j + 1]:
                next_lcs = lcs[i + 1][j]
            else:
                next_lcs = lcs[i][j + 1]

            lcs[i + 1][j + 1] = next_lcs

    return lcs[n][m]
