import graphviz as gv
import itertools
import random


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

    def add_edge(self, src, dest):
        self.adj_list[src].append(dest)

    def add_edges(self, *edges):
        for edge in edges:
            self.add_edge(*edge)

    def nodes(self):
        return zip(range(len(self.adj_list)), self.labels)

    def edges(self, root=None):
        if root is not None:
            return [(root, v) for v in self.adj_list[root]]
        return itertools.chain.from_iterable([self.edges(v) for v, _ in self.nodes()])

    def show(self, filename):
        g = gv.Digraph(format='svg', engine='fdp')
        for _, node in self.nodes():
            g.node(node)
        for u, v in self.edges():
            g.edge(self.labels[u], self.labels[v])
        g.render(filename)


def dfs(graph, v, meta):
    index, low = meta['index'], meta['low']
    stack, on_stack = meta['stack'], meta['on_stack']
    components = meta['components']

    meta['time'] += 1
    index[v] = meta['time']
    low[v] = meta['time']
    stack.append(v)
    on_stack[v] = True

    for _, w in graph.edges(v):
        if index[w] == 0:
            dfs(graph, w, meta)
            low[v] = min(low[v], low[w])
        elif on_stack[w] is True:
            low[v] = min(low[v], index[w])

    # generate scc
    if index[v] == low[v]:
        meta['component'] += 1
        c = meta['component']
        components[c] = []
        while True:
            w = stack.pop()
            on_stack[w] = False
            components[c].append((w, graph.labels[w]))
            if w == v:
                break


def find_strongly_connected_components(graph):
    meta = {
        'stack': [],
        'on_stack': { node: False for node, _ in graph.nodes() },
        'index': { node: 0 for node, _ in graph.nodes() },
        'low': { node: 0 for node, _ in graph.nodes() },
        'components': {},
        'time': 0,
        'component': 0
    }
    for node, _ in graph.nodes():
        if meta['index'][node] == 0:
            dfs(graph, node, meta)
    return meta['components']


def visualize_components(graph, components, filename):
    n_to_c = {
        node: c
        for c, nodes in components.items()
        for node, _ in nodes
    }
    rand_color = lambda: random.randint(0, 255)
    c_to_color = {
        c: '#%02X%02X%02X' % (rand_color(), rand_color(), rand_color())
        for c, _ in components.items()
    }
    g = gv.Digraph(format='svg', engine='fdp')
    for node, label in graph.nodes():
        g.node(label, color=c_to_color[n_to_c[node]])
    for u, v in graph.edges():
        color = c_to_color[n_to_c[u]] if n_to_c[u] == n_to_c[v] else 'black'
        g.edge(graph.labels[u], graph.labels[v], color=color)
    g.render(filename)


graph = Graph()

for _ in range(15):
    graph.add_node()

graph.add_edges(
    (0, 0),
    (0, 2),
    (1, 0),
    (2, 1),
    (2, 4),
    (3, 1),
    (3, 12),
    (3, 11),
    (4, 5),
    (4, 7),
    (5, 6),
    (5, 9),
    (5, 7),
    (6, 9),
    (7, 8),
    (7, 9),
    (8, 4),
    (8, 10),
    (9, 8),
    (9, 10),
    (9, 13),
    (10, 11),
    (10, 13),
    (11, 12),
    (12, 10),
    (12, 14),
    (13, 12),
    (14, 13)
)

components = find_strongly_connected_components(graph)
visualize_components(graph, components, 'graph')
# graph.show('graph')
