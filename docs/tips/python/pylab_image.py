import numpy as np
import pylab

# read image
img = pylab.imread('Pictures/new-wallpaper-2.jpeg')
img = np.asarray(img, dtype=np.float32) / 256.0

# display
pylab.subplot(1, 2, 1)
pylab.axis('off')
pylab.imshow(img)

pylab.gray()
pylab.subplot(1, 2, 2)
pylab.axis('off')
pylab.imshow(img[:, :, 0]) # display red channel

pylab.show()

