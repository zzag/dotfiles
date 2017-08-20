import numpy as np


def f(a):
    return 1/(1 + np.exp(-a))


def f_derivative(a):
    return a*(1 - a)


class SigmoidLayer:

    def __init__(self, input_size, total_neurons):
        self.shape = (total_neurons, input_size)
        self.theta = np.random.randn(*self.shape)
        self.bias = np.random.randn(total_neurons)
        self.error_signal = None

    def forward(self, x):
        self.x = x
        self.z = f(self.theta.dot(self.x))

    def backward(self, error_signal):
        mini_batch_size = self.x.shape[1]
        self.error_signal = self.theta.T.dot(error_signal) * f_derivative(self.x)
        self.delta_theta = error_signal.dot(self.x.T)/mini_batch_size
        self.delta_bias = error_signal.sum(axis=1)/mini_batch_size

    def update_weights(self, learning_rate):
        self.theta += learning_rate*self.delta_theta
        self.bias += learning_rate*self.delta_bias
