from pylab import *
from dataset import data, target
from perceptron import Perceptron


positive_class = np.array([x for x, y in zip(data, target) if y == 1])
negative_class = np.array([x for x, y in zip(data, target) if y == 0])

perceptron = Perceptron(max_iter=20)
perceptron.train(data, target)

print "Weights: \n", perceptron.weights

x_hyperline = np.array([-4.0, 4.0])
y_hyperline = -(x_hyperline * perceptron.weights[1] + perceptron.weights[0]) / perceptron.weights[2]

figure(figsize=(8, 6))
xlim((-4.0, 4.0))
ylim((-3.0, 3.0))

plot(*zip(*positive_class), color='blue', marker='o', linewidth=0.0)
plot(*zip(*negative_class), color='red', marker='o', linewidth=0.0)
plot(x_hyperline, y_hyperline, color='green')

show()
