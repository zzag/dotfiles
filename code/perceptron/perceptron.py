import numpy as np


class Perceptron:
    def __init__(self, max_iter=10, threshold=0, learning_rate=0.1):
        self.weights = None
        self.max_iter = max_iter
        self.threshold = threshold
        self.learning_rate = learning_rate

    def round(self, value):
        if value > self.threshold:
            return 1

        return 0

    def train(self, X, y):
        n = X.shape[1]
        self.weights = np.zeros(shape=(n + 1, 1))

        for iteration in range(self.max_iter):
            for x_, desired_output in zip(X, y):
                x = np.hstack((1, x_))
                actual_output = self.round(np.dot(self.weights.transpose(), x)[0])
                error = actual_output - desired_output
                self.weights = self.weights - self.learning_rate * error * x.reshape((n + 1,1))

        return self

    def predict(self, x):
        x = np.hstack((1.0, x))
        output = np.dot(self.weights.transpose(), x)
        return self.round(output[0])

    def __repr__(self):
        msg = "Weights: \n%s" % (str(self.weights))
        return msg
