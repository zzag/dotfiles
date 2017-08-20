import graphviz as gv
import itertools


class Graph:

    def __init__(self):
        self.adj_list = []
        self.labels = []

    def add_node(self, label=None):
        index = len(self.adj_list)
        self.adj_list.append([])
        self.labels.append(label if label is not None else str(index))
        return index

    def add_nodes(self, *nodes):
        indices = []
        for node in nodes:
            indices.append(self.add_node(*node))
        return indices

    def add_edge(self, src, dest, weight):
        self.adj_list[src].append((dest, weight))

    def add_edges(self, *edges):
        for edge in edges:
            self.add_edge(*edge)

    def nodes(self):
        return zip(range(len(self.adj_list)), self.labels)

    def edges(self, root=None):
        if root is not None:
            return [(root, v, w) for v, w in self.adj_list[root]]
        return itertools.chain.from_iterable([self.edges(v) for v, _ in self.nodes()])

    def show(self, filename):
        g = gv.Digraph(format='svg', engine='fdp')
        for _, node in self.nodes():
            g.node(node)
        for u, v, _ in self.edges():
            g.edge(self.labels[u], self.labels[v])
        g.render(filename)


def dijkstra(graph, s):
    parent = [-1 for _ in graph.nodes()]
    visited = [False for _ in graph.nodes()]
    distance = [float('inf') for _ in graph.nodes()]

    distance[s] = 0
    u = s

    while not visited[u]:
        visited[u] = True
        for _, v, w in graph.edges(u):
            if distance[v] > distance[u] + w:
                distance[v] = distance[u] + w
                parent[v] = u
        dist = float('inf')
        for node, _ in graph.nodes():
            if not visited[node] and dist > distance[node]:
                dist = distance[node]
                u = node

    return s, parent, distance


def visualize_short_path(graph, root, parent, distance, filename):
        g = gv.Digraph(format='svg', engine='sfdp')
        g.graph_attr['K'] = '1.1'
        g.graph_attr['repulsiveforce'] = '5.0'
        for _, node in graph.nodes():
            g.node(node)
        for u, v, w in graph.edges():
            color = 'red' if parent[v] == u else 'black'
            label = '%s/%s' % (str(w), str(distance[v]) if parent[v] == u else 'inf')
            g.edge(graph.labels[u], graph.labels[v], color=color, label=label)
        g.render(filename)


if __name__ == '__main__':
    graph = Graph()

    for _ in range(15):
        graph.add_node()

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

    root, parent, distance = dijkstra(graph, 0)
    visualize_short_path(graph, root, parent, distance, 'graph')
    print(parent)
