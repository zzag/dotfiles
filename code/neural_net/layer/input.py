import numpy as np


class InputLayer:

    def __init__(self):
        self.z = None

    def forward(self, x):
        self.z = x

    def backward(self, error_signal):
        pass

    def update_weights(self, learning_rate):
        pass
