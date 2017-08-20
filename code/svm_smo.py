import numpy as np
import matplotlib.pyplot as plt
import random

from sklearn import datasets


def linear_kernel(x, y):
    return x.dot(y)


def quadratic_kernel(x, y, c=0.0, d=2.0):
    return (x.dot(y) + c) ** d


def randomint_except(low, high, except_int):
    while True:
        n = random.randint(low, high - 1)
        if n == except_int:
            continue
        return n


class SVM:
    """
    Soft SVM
    """

    def __init__(self, kernel):
        self.kernel = kernel
        self.params = {}

    def fit(self, X, labels, C=1, max_passes=1000, tol=0.0001, epsilon=0.00001):
        """
        Train SVM with SMO algorithm
        """
        N = X.shape[0]
        passes = 0
        kernel = self.kernel
        alpha = np.zeros((N))
        b = 0

        def f(x):
            return np.sign(kernel(X, x).dot(alpha * labels) + b)

        while passes < max_passes:
            alpha_prev = alpha.copy()
            for i in range(N):
                x_i, y_i = X[i], labels[i]
                E_i = f(x_i) - y_i
                alpha_i = alpha[i]
                if (y_i * E_i < -tol and alpha[i] < C
                        or y_i * E_i > tol and alpha[i] > 0):
                    j = randomint_except(0, N, i)
                    x_j, y_j = X[j], labels[j]
                    E_j = f(x_j) - y_j
                    alpha_j = alpha[j]

                    # compute constraints L and H
                    if y_i == y_j:
                        L = max(0, alpha_i + alpha_j - C)
                        H = min(C, alpha_i + alpha_j)
                    else:
                        L = max(0, alpha_j - alpha_i)
                        H = min(C, C + alpha_j - alpha_i)
                    if L == H:
                        continue

                    # compute eta
                    eta = kernel(x_i, x_i) + kernel(x_j, x_j) - \
                        2 * kernel(x_i, x_j)
                    if eta <= 0:
                        continue

                    # update alpha_i and alpha_j
                    alpha[j] = alpha_j + y_j * (E_i - E_j) / eta
                    alpha[j] = min(alpha[j], H)
                    alpha[j] = max(L, alpha[j])
                    alpha[i] = alpha_i + y_i * y_j * (alpha_j - alpha[j])

                    # update bias
                    b_1 = (b - E_i
                           - y_i * (alpha[i] - alpha_i) * kernel(x_i, x_i)
                           - y_j * (alpha[j] - alpha_j) * kernel(x_i, x_j))
                    b_2 = (b - E_j
                           - y_i * (alpha[i] - alpha_i) * kernel(x_i, x_j)
                           - y_j * (alpha[j] - alpha_j) * kernel(x_j, x_j))
                    b = (b_1 + b_2) / 2

            # check convergence
            diff = np.linalg.norm(alpha - alpha_prev)
            if diff < epsilon:
                break

            # go to the next iteration
            passes += 1

        # save params
        support_vectors = alpha > 0
        self.params['alpha'] = alpha[support_vectors]
        self.params['labels'] = labels[support_vectors]
        self.params['vectors'] = X[support_vectors]
        self.params['bias'] = b

        accuracy = np.mean(labels == self.predict(X))
        return accuracy

    def predict(self, X):
        if not self.params:
            raise Exception('Please train the model.')
        alpha = self.params['alpha']
        labels = self.params['labels']
        vectors = self.params['vectors']
        bias = self.params['bias']
        return np.sign(self.kernel(X, vectors.T).dot(alpha * labels) + bias)


data, labels = datasets.make_circles(n_samples=200, noise=0.1, factor=0.2)
labels[labels == 0] = -1.0
svm = SVM(kernel=lambda x, y: quadratic_kernel(x, y, c=2.0))
svm.fit(data, labels)

h = 0.01
padding = 0.4
x_min, x_max = data[:, 0].min() - padding, data[:, 0].max() + padding
y_min, y_max = data[:, 1].min() - padding, data[:, 1].max() + padding
xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                     np.arange(y_min, y_max, h))
Z = svm.predict(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape)

plt.title('SVM')
plt.axis('off')
plt.xlim(xx.min(), xx.max())
plt.ylim(yy.min(), yy.max())
plt.contourf(xx, yy, Z, cmap=plt.cm.Paired, alpha=0.8)
plt.scatter(*zip(*data[labels == 1]),
            marker='o',
            s=50,
            facecolors='none',
            edgecolors='g')
plt.scatter(*zip(*data[labels == -1]),
            marker='x',
            c='red')
plt.scatter(*zip(*svm.params['vectors']),
            c='blue',
            s=50)
plt.show()
