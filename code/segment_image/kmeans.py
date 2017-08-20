import numpy as np


class KMeans:

    def __init__(self, total_clusters=2, total_iterations=5):
        self.total_clusters = total_clusters
        self.total_iterations = total_iterations
        self.clusters = []

    def _pick_clusters(self, X):
        index = np.random.choice(range(X.shape[0]), self.total_clusters, False)
        self.clusters = list(map(lambda i: X[i], index))

    def _closest_cluster(self, x):
        distance = np.array([np.linalg.norm(x - centroid) for centroid in self.clusters])
        return distance.argmin()

    def _centroid_of(self, X, clusters, cluster):
        tmp = np.zeros(self.shape)
        l = 0

        for i in range(X.shape[0]):
            if clusters[i] != cluster:
                continue

            tmp += X[i]
            l += 1

        return tmp / l

    def fit(self, X):
        self.shape = X[0].shape
        cluster = np.zeros(X.shape[0])
        self._pick_clusters(X)

        for iteration in range(self.total_iterations):
            for i in range(X.shape[0]):
                cluster[i] = self._closest_cluster(X[i])

            for k in range(self.total_clusters):
                self.clusters[k] = self._centroid_of(X, cluster, k)

        return self

    def predict(self, X):
        result = np.empty(X.shape)

        for i in range(X.shape[0]):
            result[i] = self.clusters[self._closest_cluster(X[i])]

        return result
