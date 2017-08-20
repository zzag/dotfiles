import networkx as nx
import pygraphviz as pgv


class Node:

    def __init__(self, key):
        self.key = key
        self.degree = 0
        self.mark = False
        self.parent = None
        self.child_list = []


class FibonacciHeap:

    def __init__(self):
        self.min = None
        self.root_list = []
        self.size = 0

    def insert(self, key):
        node = Node(key)
        self.root_list.append(node)
        self.size += 1
        if self.min is None:
            self.min = node
        elif key < self.min.key:
            self.min = node
        return node

    def delete(self, node):
        self.decrease_key(node, float('-inf'))
        self.extract_min()

    def extract_min(self):
        z = self.min
        if z is not None:
            for child in z.child_list:
                self.root_list.append(child)
                child.parent = None
            self.root_list.remove(z)
            if not self.root_list:
                self.min = None
            else:
                self.consolidate()
            self.size -= 1
        return z

    def decrease_key(self, node, new_key):
        node.key = new_key
        parent = node.parent
        if parent is not None and node.key < parent.key:
            self.cut(parent, node)
            self.cascading_cut(parent)
        if node.key < self.min.key:
            self.min = node

    def consolidate(self):
        aux = [None] * self.size
        for root in self.root_list:
            x = root
            d = x.degree
            while aux[d] is not None:
                y = aux[d]
                if y.key < x.key:
                    x, y = y, x
                x.child_list.append(y)
                y.parent = x
                aux[d] = None
                d += 1
            aux[d] = x
        self.min = None
        self.root_list = []
        for node in filter(None, aux):
            self.root_list.append(node)
            if self.min is None:
                self.min = node
            elif node.key < self.min.key:
                self.min = node

    def cut(self, parent, node):
        parent.child_list.remove(node)
        self.root_list.append(node)
        node.parent = None
        node.mark = False

    def cascading_cut(self, parent):
        grandparent = parent.parent
        if grandparent is not None:
            if not parent.mark:
                parent.mark = True
            else:
                self.cut(grandparent, parent)
                self.cascading_cut(grandparent)


def visit(node, graph):
    args = {}
    if node.mark:
        args['fillcolor'] = 'black'
        args['fontcolor'] = 'white'
        args['style'] = 'filled'
    graph.add_node(str(node.key), **args)
    if node.parent:
        graph.add_edge(str(node.parent.key), str(node.key))
    for child in node.child_list:
        visit(child, graph)


def visualize_fibonacci_heap(h, filename):
    G = nx.DiGraph(overlap=False)
    last = None
    for node in h.root_list:
        visit(node, G)
        if last:
            G.add_edge(str(last.key), str(node.key), dir='none', style='dashed')
        last = node
    A = nx.drawing.nx_agraph.to_agraph(G)
    A.node_attr['shape'] = 'circle'
    A.add_subgraph([str(node.key) for node in h.root_list], rank='same')
    A.draw(filename, prog='dot')
