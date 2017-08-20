import copy
from pprint import pprint


class Graph:

    def __init__(self, num_nodes):
        self.adj_matrix = [[float('inf')]*num_nodes for _ in range(num_nodes)]
        self.num_nodes = num_nodes

    def add_edge(self, src, dest, weight):
        self.adj_matrix[src][dest] = weight

    def add_edges(self, *edges):
        for edge in edges:
            self.add_edge(*edge)


def floyd_warshall(graph):
    w = copy.deepcopy(graph.adj_matrix)
    n = graph.num_nodes

    for k in range(n):
        for i in range(n):
            for j in range(n):
                w[i][j] = min(w[i][j], w[i][k] + w[k][j])
    return w


if __name__ == '__main__':
    graph = Graph(15)
    graph.add_edges(
        (0, 0, 1),
        (0, 2, 1),
        (1, 0, 1),
        (2, 1, 1),
        (2, 4, 1),
        (3, 1, 1),
        (3, 12, 1),
        (3, 11, 1),
        (4, 5, 4),
        (4, 7, 1),
        (5, 6, 1),
        (5, 9, 2),
        (5, 7, 2),
        (6, 9, 1),
        (7, 8, 1),
        (7, 9, 1),
        (8, 4, 1),
        (8, 10, 1),
        (9, 8, 1),
        (9, 10, 1),
        (9, 13, 1),
        (10, 11, 1),
        (10, 13, 1),
        (11, 12, 1),
        (12, 10, 1),
        (12, 14, 1),
        (13, 12, 1),
        (14, 13, 1)
    )
    pprint(graph.adj_matrix)
    pprint(floyd_warshall(graph))
