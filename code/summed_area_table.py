import numpy as np
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec


#
# Summed Area Table
#
#  S(x, y) = I(x, y) + S(x-1, y) + S(x, y-1) - S(x-1, y-1)
#  I(x, y) = S(x, y) + S(x-1, y-1) - S(x-1, y) - S(x, y-1)
#

img = plt.imread('image.jpg')
sat = img.astype(int)

# summed area table
h, w, _ = sat.shape
for i in range(h):
    for j in range(w):
        b = 0 if i == 0 else sat[i - 1, j]
        c = 0 if j == 0 else sat[i, j - 1]
        a = 0 if i == 0 or j == 0 else sat[i - 1, j - 1]
        sat[i, j] = sat[i, j] + c + b - a

# reconstruct image
rec = np.copy(img)
for i in range(h):
    for j in range(w):
        b = 0 if i == 0 else sat[i - 1, j]
        c = 0 if j == 0 else sat[i, j - 1]
        a = 0 if i == 0 or j == 0 else sat[i - 1, j - 1]
        rec[i, j] = sat[i, j] + a - b - c

# error
print('Reconstruction error: %f' % np.sqrt(np.sum((img-img)**2)))

gs = gridspec.GridSpec(2, 2)
plt.subplot(gs[0, :])
plt.title('Summed Area Table')
plt.axis('off')
plt.imshow((sat - sat.min())/(sat.max() - sat.min()))
plt.subplot(gs[1, 0])
plt.title('Original')
plt.axis('off')
plt.imshow(img)
plt.subplot(gs[1, 1])
plt.title('Reconstructed')
plt.axis('off')
plt.imshow(rec)

plt.show()
