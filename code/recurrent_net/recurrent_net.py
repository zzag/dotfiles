import numpy as np
import pickle


def softmax(x):
  probs = np.exp(x - np.min(x))
  return probs / np.sum(probs)


class RecurrentNet:
  """
  Recurrent Neural Network

  Dimensions:
   * input_dim
   * hidden_dim
   * output_dim

  Parameters:
   * U: hidden_dim x input_dim
   * W: hidden_dim x hidden_dim
   * V: output_dim x hidden_dim

  Equations:
   z_t = W*h_{t-1} + U*x_t
   h_t = f(z_t)      / f = tanh
   y_t = V*h_t
   ypred_t = g(y_t)  / g = softmax
  """

  def __init__(self, input_dim, hidden_dim, output_dim):
    self.input_dim = input_dim
    self.hidden_dim = hidden_dim
    self.output_dim = output_dim

    self.params = {}
    self.params['U'] = np.random.randn(hidden_dim, input_dim) / np.sqrt(input_dim)
    self.params['W'] = np.random.randn(hidden_dim, hidden_dim) / np.sqrt(hidden_dim)
    self.params['V'] = np.random.randn(output_dim, hidden_dim) / np.sqrt(hidden_dim)

  def forward(self, x, timesteps):
    h = np.zeros((timesteps + 1, self.hidden_dim))
    y = np.zeros((timesteps, self.output_dim))
    U, W, V = self.params['U'], self.params['W'], self.params['V']

    for t in range(timesteps):
      h[t] = np.tanh(W.dot(h[t - 1]) + U[:, x[t]])
      y[t] = softmax(V.dot(h[t]))

    return h, y

  def backward(self, dout, x, h, y):
    U, W, V = self.params['U'], self.params['W'], self.params['V']
    dU, dW, dV = np.zeros_like(U), np.zeros_like(W), np.zeros_like(V)
    timesteps = len(x)

    for t in reversed(range(timesteps)):
      dV += np.outer(dout[t], h[t])
      dh_j = V.T.dot(dout[t]) * (1 - h[t]**2)
      for j in reversed(range(t + 1)):
        dW += np.outer(dh_j, h[j-1])
        dU[:, x[j]] += dh_j # dU += np.outer(dh_j, x[j]) if x[j] is a vector
        dh_j = W.T.dot(dh_j) * (1 - h[j-1]**2)

    return dU, dW, dV

  def loss(self, X, y):
    # do forward pass
    timesteps = len(X)
    h, y_pred = self.forward(X, timesteps)

    # cross entropy loss
    loss = -np.sum(np.log(y_pred[np.arange(timesteps), y]))

    # gradients...
    grads = {}
    dout = y_pred
    dout[np.arange(timesteps), y] -= 1
    grads['U'], grads['W'], grads['V'] = self.backward(dout, X, h, y_pred)

    return loss, grads


class StochasticGradientDescentTrainer:

  def __init__(self, model, batch_size, total_epochs, learning_rate):
    self.model = model
    self.batch_size = batch_size
    self.total_epochs = total_epochs
    self.learning_rate = learning_rate

  def train(self, X, y):
    train_params = self.model.params.keys()
    for epoch in range(self.total_epochs):
      training_samples = np.random.randint(0, len(X), self.batch_size)

      for i in training_samples:
        loss, grads = self.model.loss(X[i], y[i])
        for param in train_params:
          # TODO: cut gradient
          self.model.params[param] -= self.learning_rate*grads[param]
        print("Epoch %d, sample %d, loss %.2f" % (epoch, i, loss))


def numerical_gradient(model, x, y, param_name, dx=1e-3):
  param = model.params[param_name]
  grad = np.zeros_like(param)
  it = np.nditer(param, flags=['multi_index'], op_flags=['readwrite'])
  while not it.finished:
    ix = it.multi_index
    oldvalue = param[ix]
    param[ix] = oldvalue - dx
    loss_neg, _ = model.loss(x, y)
    param[ix] = oldvalue + dx
    loss_plus, _ = model.loss(x, y)
    param[ix] = oldvalue
    grad[ix] = (loss_plus - loss_neg)/(2*dx)
    it.iternext()
  return grad


def gradient_checking(model, x, y):
  params = model.params.keys()

  for param in params:
    _, grad = model.loss(x, y)
    num_grad = numerical_gradient(model, x, y, param)
    error = np.sum((np.abs(grad[param]) - np.abs(num_grad))/(np.abs(grad[param]) + np.abs(num_grad) + 1e-6))
    print('d%s relative error: %f' % (param, error))


def test_gradients():
  input_dim = 200
  hidden_dim = 100
  output_dim = 20
  timesteps = 2

  x = np.random.randint(0, input_dim, timesteps)
  y = np.random.randint(0, output_dim, timesteps)

  model = RecurrentNet(input_dim, hidden_dim, output_dim)
  gradient_checking(model, x, y)


def load_dataset(filename):
  with open(filename, 'rb') as f:
    dataset = pickle.load(f)

  return dataset['x_train'], dataset['y_train']


def load_vocabulary(filename):
  with open(filename, 'rb') as f:
    vocabulary = pickle.load(f)

  return vocabulary


def save_model(model, filename):
  with open(filename, 'wb') as f:
    pickle.dump(model, f, pickle.HIGHEST_PROTOCOL)


vocabulary = load_vocabulary('vocabulary.pickle')
X_train, y_train = load_dataset('reviews.pickle')

input_dim = len(vocabulary['index_to_word'])
hidden_dim = 400
output_dim = input_dim
del vocabulary

model = RecurrentNet(input_dim, hidden_dim, output_dim)
trainer = StochasticGradientDescentTrainer(
  model=model,
  batch_size=16,
  total_epochs=100,
  learning_rate=0.005)
trainer.train(X_train, y_train)

save_model(model, 'model.pickle')
