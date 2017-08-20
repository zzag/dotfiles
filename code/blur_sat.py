import numpy as np
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt


window_width = 26
window_height = 26
img = plt.imread('image.jpg')
sat = img.astype(int)

# sat
h, w, _ = sat.shape
for i in range(h):
    for j in range(w):
        a = 0 if i == 0 or j == 0 else sat[i - 1, j - 1]
        b = 0 if i == 0 else sat[i - 1, j]
        c = 0 if j == 0 else sat[i, j - 1]
        sat[i, j] = img[i, j] + b + c - a

# blur
blurred = np.zeros(img.shape, dtype=np.float32)
for i in range(h):
    for j in range(w):
        x, y = j - window_width//2, i - window_height//2
        ul = sat[max(y, 0), max(x, 0)]
        ur = sat[max(y, 0), min(x+window_width, w-1)]
        ll = sat[min(y+window_height, h-1), max(x, 0)]
        lr = sat[min(y+window_height, h-1), min(x+window_width, w-1)]
        blurred[i, j] = (lr + ul - ll - ur)/(window_width*window_height)

plt.subplot(1, 2, 1)
plt.title('Original')
plt.axis('off')
plt.imshow(img)
plt.subplot(1, 2, 2)
plt.title('Blurred')
plt.axis('off')
plt.imshow(blurred/blurred.max())
plt.show()

