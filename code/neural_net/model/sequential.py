class SequentialModel:

    def __init__(self, max_epoch=10, learning_rate=0.05):
        self.layers = []
        self.criterion = None
        self.max_epoch = 10
        self.learning_rate = learning_rate

    def add(self, layer):
        self.layers.append(layer)

    def set_criterion(self, criterion):
        self.criterion = criterion

    def feed(self, X):
        previous_layer = self.layers[0]
        previous_layer.forward(X)

        for layer in self.layers[1:]:
            layer.forward(previous_layer.z)
            previous_layer = layer

        output_layer = self.layers[-1]
        return output_layer.z

    def train(self, X, y, batch_size=500):
        total_batches = X.shape[0]//batch_size + min(1, X.shape[0] % batch_size)

        for iteration in range(self.max_epoch):
            for batch_index in range(total_batches):
                batch_x = X[batch_index*batch_size:(batch_index + 1)*batch_size]
                batch_y = y[batch_index*batch_size:(batch_index + 1)*batch_size]

                # feedforward
                actual = self.feed(batch_x.T)

                # backpropagation
                next_layer = self.criterion.initialize_error_signal(batch_y.T, actual)
                cost = self.criterion.get_cost(batch_y.T, actual)

                for layer in reversed(self.layers):
                    layer.backward(next_layer.error_signal)
                    layer.update_weights(self.learning_rate)
                    next_layer = layer

                print(cost)
