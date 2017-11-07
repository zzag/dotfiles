import pylab as plt
import numpy as np


x = np.linspace(-3, 3, 100)
y = np.exp(-x**2)

plt.xlim(-5, 5)
plt.ylim(0, 3)
plt.plot(x, y)
plt.fill_between(x, 0, y, color="#33ccdd", alpha=0.4)
plt.show()

