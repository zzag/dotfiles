import graphviz as gv


class Graph:

    def __init__(self, directed=True):
        self.directed = directed
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
            indices.append(self.add_node(*node if isinstance(node, tuple) else node))
        return indices

    def add_edge(self, src, dst, weight=None):
        self.adj_list[src].append((dst, weight))
        if not self.directed:
            self.adj_list[dst].append((src, weight))

    def add_edges(self, *edges):
        for edge in edges:
            self.add_edge(*edge)

    def adj_edges(self, src):
        return self.adj_edges[src]

    def show(self, filename, format='svg'):
        if self.directed:
            g = gv.Digraph(format=format)
        else:
            g = gv.Graph(format=format)

        for label in self.labels:
            g.node(label)

        seen = { node: False for node in range(len(self.adj_list)) }
        for src, adj in enumerate(self.adj_list):
            for dst, _ in adj:
                if not seen[dst] or self.directed:
                    g.edge(self.labels[src], self.labels[dst])
            seen[src] = True
        g.render(filename)


graph = Graph()
a, b, c, d, e, f, g, h = graph.add_nodes('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h')

graph.add_edges(
    (a, b),
    (a, c),
    (b, d),
    (b, e),
    (e, f),
    (e, g),
    (f, h),
    (h, a)
)

graph.show('graph')
