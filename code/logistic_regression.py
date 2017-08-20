import numpy as np
import theano
from theano import tensor as T
import cPickle
import gzip

#
# load dataset
# http://deeplearning.net/data/mnist/mnist.pkl.gz
#
with gzip.open("mnist.pkl.gz", "rb") as f:
    train_set, _, __ = cPickle.load(f)

#
# define some variables
#
class_ = 3
learning_rate = 0.6
reg_lambda = 0.01
max_epoch = 1000
x_train, y_train = train_set
y_train[y_train != class_] = 0
y_train[y_train == class_] = 1
m, n = x_train.shape

#
# symbolic variables
#
x = T.matrix("x")
y = T.vector("y")
w = theano.shared(np.random.randn(n), name="w")
b = theano.shared(0.0, name="b")

#
# graph
#
p = 1 / (1 + T.exp(-T.dot(x, w) - b))
prediction = p > 0.5
cross_entropy = -y*T.log(p) - (1 - y)*T.log(1 - p)
cost = cross_entropy.mean() + reg_lambda*(w ** 2).sum()
grad_w, grad_b = T.grad(cost, [w, b])

#
# compile
#
train = theano.function(
    inputs=[x, y],
    outputs=[prediction, cross_entropy, cost],
    updates=[(w, w - learning_rate*grad_w), (b, b - learning_rate*b)]
)

#
# train
#
print "Training..."
for i in xrange(max_epoch):
    pred, entropy, cost = train(x_train, y_train)
    print "Epoch #%d Cost: %f" % (i + 1, cost)
