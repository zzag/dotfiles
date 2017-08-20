import numpy as np
import matplotlib.pyplot as plt

from sklearn import datasets


def linear_feature_map(x):
    """
    Linear feature mapping.
    """
    return x


def quadratic_feature_map(x, c=0):
    """
    Quadratic feature mapping.

    X in R^2
    """
    D = x.shape[1]
    y = x.reshape((-1, D))
    y_1, y_2 = y[:, 0], y[:, 1]
    return np.c_[y_1 ** 2, y_2 ** 2, np.sqrt(2 * c) * y_1 * y_2]


class SVM:
    """
    Binary Support Vector Machine
    """

    def __init__(self, feature_map):
        self.feature_map = feature_map
        self.params = {}

    def fit(self, X, labels, options=None):
        """
        Train SVM with hinge loss function.
        """
        if not options:
            options = {}
        learning_rate = options.get('learning_rate', 0.05)
        reg = options.get('reg', 0.001)
        max_epoch = options.get('max_epoch', 100)
        feature_dim = options.get('feature_dim', 3)
        epsilon = options.get('epsilon', 0.0001)
        feature_map = self.feature_map

        N, D = X.shape
        W = np.random.rand(feature_dim)
        b = 0
        epoch = 0
        prev_loss = float('inf')

        while epoch < max_epoch:
            scores = W.dot(feature_map(X).T) + b
            margins = np.maximum(0, 1 - labels * scores)

            # calc loss
            loss = np.sum(margins) / N
            loss += 0.5 * reg * np.sum(X ** 2)

            # calc gradients
            grad_flow = margins > 0
            dW = -np.sum(feature_map(X[grad_flow])
                         * labels[grad_flow, np.newaxis]) / N
            dW += reg * W
            db = -np.sum(labels[grad_flow]) / N

            # check convergence
            if abs(prev_loss - loss) < epsilon:
                break
            prev_loss = loss

            # update parameters
            W -= learning_rate * dW
            b -= learning_rate * db

        # save parameters
        self.params['W'] = W
        self.params['b'] = b

    def predict(self, X):
        W = self.params['W']
        b = self.params['b']
        feature_map = self.feature_map
        return np.sign(W.dot(feature_map(X).T) + b)


data, labels = datasets.make_circles(n_samples=100, factor=0.2, noise=0.13)
labels[labels == 0] = -1.0
svm = SVM(feature_map=quadratic_feature_map)
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
plt.show()
