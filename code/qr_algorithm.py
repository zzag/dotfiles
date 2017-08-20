import numpy as np


def qr_decomposition(M):
    n = M.shape[1]
    Q = np.array(M)

    # Gram-Schmidt process
    for i in range(n):
        Q[:, i] /= np.linalg.norm(Q[:, i])

        for j in range(i + 1, n):
            Q[:, j] -= Q[:, j].dot(Q[:, i]) * Q[:, i]
    
    # find R
    R = Q.T.dot(M)

    return Q, R


def qr_algorithm(M):
    n = M.shape[1]
    A = np.array(M)
    eigenvectors = np.eye(n)

    for i in range(100):
        Q, R = qr_decomposition(A)
        A = R.dot(Q)
        eigenvectors = eigenvectors.dot(Q)
    
    eigenvalues = np.diagonal(A)
    eigenvectors = [eigenvectors[:, i] for i in range(n)]
    return eigenvalues, eigenvectors


M = np.array([
    [1, 2],
    [3, 4]
], dtype="float32")

eigenvalues, eigenvectors = qr_algorithm(M)

for eigenvalue, eigenvector in zip(eigenvalues, eigenvectors):
    print("Eigenvalue: %s; eigenvector %s" % (str(eigenvalue), str(eigenvector)))

