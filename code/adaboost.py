import numpy as np
import operator as op
import matplotlib.pyplot as plt

from sklearn import datasets


class DecisionStump:

    def __init__(self, operator, feature, threshold):
        self.operator = operator
        self.feature = feature
        self.threshold = threshold

    def predict(self, X):
        Xr = np.reshape(X, (-1, X.shape[-1]))
        N, D = Xr.shape
        result = np.ones(N)
        indices = self.operator(Xr[:, self.feature], self.threshold)
        result[indices] = -1
        return result

    @staticmethod
    def from_distribution(D, X, y):
        operators = [op.le, op.gt]
        model = None
        best_acc = float('inf')

        for operator in operators:
            features = range(X.shape[1])
            for feature in features:
                thresholds = X[:, feature]
                for threshold in thresholds:
                    stump = DecisionStump(operator, feature, threshold)
                    y_test = stump.predict(X)
                    test_acc = np.sum(D[y_test != y]) / np.sum(D)
                    if test_acc < best_acc:
                        model = stump
                        best_acc = test_acc

        return best_acc, model


class AdaBoost:

    def __init__(self):
        self.weak_learners = []

    def fit(self, X, y, options=None):
        if not options:
            options = {}

        T = options.get('T', 200)
        N = X.shape[0]

        D = np.ones(N) / N
        for t in range(T):
            err, h = DecisionStump.from_distribution(D, X, y)
            alpha = 0.5 * (np.log(1 - err + 1e-7) - np.log(err + 1e-7))
            D *= np.exp(-alpha * y * h.predict(X))
            D /= np.sum(D)
            self.weak_learners.append((alpha, h))

            y_test = self.predict(X)
            acc = np.mean(y_test == y)
            print('Timestep #%d: accuracy %.2f%%' % (t + 1, acc * 100))

    def predict(self, X):
        Xr = np.reshape(X, (-1, X.shape[-1]))
        N, D = Xr.shape
        result = np.zeros(N)

        for alpha, h in self.weak_learners:
            result += alpha * h.predict(X)

        return np.sign(result)


X, y = datasets.make_classification(
    n_features=2,
    n_redundant=0,
    n_informative=2,
    n_clusters_per_class=1)
y[y == 0] = -1


model = AdaBoost()
model.fit(X, y, { 'T': 30 })


h = 0.01
padding = 0.4
x_min, x_max = X[:, 0].min() - padding, X[:, 0].max() + padding
y_min, y_max = X[:, 1].min() - padding, X[:, 1].max() + padding
xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                     np.arange(y_min, y_max, h))
Z = model.predict(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape)

plt.title('AdaBoost')
plt.xlim(xx.min(), xx.max())
plt.ylim(yy.min(), yy.max())
plt.contourf(xx, yy, Z, cmap=plt.cm.Paired, alpha=0.8)
plt.scatter(*zip(*X[y == -1]),
            edgecolors='none',
            facecolors='green')
plt.scatter(*zip(*X[y == 1]),
            edgecolors='none',
            facecolors='red')
plt.show()
