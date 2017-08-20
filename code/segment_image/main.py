from skimage import io
from kmeans import KMeans


img = io.imread("Lenna.png") / 255
w, h, d = img.shape
reshaped = img.reshape((w * h, d))

classificator = KMeans(total_clusters=2, total_iterations=5).fit(reshaped)
labels = classificator.predict(reshaped)
segmented = labels.reshape((w, h, d))

io.imshow(segmented)
io.show()
