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
        return self.adj_list[src]

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


def initialize_dfs(graph):
    return {
        'processed': { node: False for node in range(len(graph.labels)) },
        'discovered': { node: False for node in range(len(graph.labels)) },
        'parent': { node: None for node in range(len(graph.labels)) }
    }


def dfs(graph, v, meta=None):
    processed, discovered, parent = meta['processed'], meta['discovered'], meta['parent']
    adj = graph.adj_edges(v)

    if discovered[v]:
        return

    process_before(graph, v, meta)
    discovered[v] = True

    for d, _ in adj:
        if not discovered[d]:
            parent[d] = v
            process_edge(graph, v, d, meta)
            dfs(graph, d, meta)
        elif not processed[d] or graph.directed:
            process_edge(graph, v, d, meta)

    process_after(graph, v, meta)
    processed[v] = True


def process_before(graph, v, meta):
    pass


def process_edge(graph, src, dst, meta):
    discovered, parent = meta['discovered'], meta['parent']
    if discovered[dst] and parent[src] != dst:
        print('Cycle %s - %s' % (graph.labels[src], graph.labels[dst]))


def process_after(graph, v, meta):
    pass


def find_cycle(graph):
    meta = initialize_dfs(graph)
    num_nodes = len(graph.labels)
    for v in range(num_nodes):
        dfs(graph, v, meta)


if __name__ == '__main__':
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

    find_cycle(graph)
