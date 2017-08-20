import networkx as nx
import matplotlib.pyplot as plt


class BinomialNode:

    def __init__(self, key, parent=None, degree=0, child=None, sibling=None):
        self.key = key
        self.parent = parent
        self.degree = degree
        self.child = child
        self.sibling = sibling

    def add_child(self, child):
        child.parent = self
        child.sibling = self.child
        self.child = child
        self.degree += 1


def _merge_head_list(a, b):
    head = None
    tail = None
    while a or b:
        if not b and a:
            root = a
            a = a.sibling
        elif not a and b:
            root = b
            b = b.sibling
        else:
            if a.degree < b.degree:
                root = a
                a = a.sibling
            else:
                root = b
                b = b.sibling
        if not head:
            head = root
            tail = root
        else:
            tail.sibling = root
            tail = root
    return head


class BinomialHeap:

    def __init__(self, head=None):
        self.head = head

    def search_min(self):
        min_node = None
        min_key = float('inf')
        curr_node = self.head
        while curr_node:
            if curr_node.key < min_key:
                min_node = curr_node
                min_key = curr_node.key
            curr_node = curr_node.sibling
        return min_node

    def insert(self, *keys):
        for key in keys:
            another_heap = BinomialHeap(BinomialNode(key=key))
            self.union(another_heap)

    def union(self, other):
        head = _merge_head_list(self.head, other.head)
        self.head = head
        if not self.head:
            return
        prev_node = None
        curr = head
        next_node = curr.sibling
        while next_node:
            if (curr.degree != next_node.degree or
                    next_node.sibling is not None and
                    next_node.sibling.degree == curr.degree):
                # degree of current node is not equal to degree of
                # the next node or 3 nodes with the same degree in a row
                prev_node = curr
                curr = next_node
                next_node = curr.sibling
                continue
            if curr.key <= next_node.key:
                # next node becomes a child of current node
                curr.sibling = next_node.sibling
                curr.add_child(next_node)
            else:
                # current node becomes a child of the next node
                # 2 cases:
                #  * current node is head:
                #        next node becomes head
                #  * current node is not head:
                #        sibling of the previous node is the next node
                if prev_node is None:
                    self.head = next_node
                else:
                    prev_node.sibling = next_node
                next_node.add_child(curr)
                curr = next_node
            next_node = curr.sibling

    def __remove_node(self, node):
        if node == self.head:
            self.head = self.head.sibling
            return
        prev = self.head
        while prev:
            if prev.sibling == node:
                prev.sibling = node.sibling
                break
            prev = prev.sibling

    def extract_min(self):
        # find min node
        min_node = self.search_min()
        if not min_node:
            return
        # remove it from root list
        self.__remove_node(min_node)
        # reverse its children
        head = None
        curr = min_node.child
        while curr:
            curr.parent = None
            sibling = curr.sibling
            curr.sibling = head
            head = curr
            curr = sibling
        # unite two heaps
        other = BinomialHeap(head)
        self.union(other)

    def decrease_key(self, node, key):
        node.key = key
        child = node
        parent = child.parent
        while parent is not None and parent.key > child.key:
            child.key, parent.key = parent.key, child.key
            child = parent
            parent = child.parent

    def delete(self, node):
        self.decrease_key(node, float('-inf'))
        self.extract_min()


def visualize_tree(root, G, pos):
    pos_x, pos_y = pos
    pos_y += 1
    G.add_node(str(root.key))
    if root.parent:
        G.add_edge(str(root.parent.key), str(root.key))
    if not root.child:
        pos_x += 2
    else:
        child = root.child
        while child:
            pos_x, _ = visualize_tree(child, G, (pos_x, pos_y))
            child = child.sibling
    # set its own pos
    G.node[str(root.key)]['pos'] = (pos_x, pos_y)
    return (pos_x, pos_y)


def visualize_binomial_heap(h):
    G = nx.Graph()
    root_node = h.head
    pos_x, pos_y = -1, -1
    prev = None
    while root_node:
        pos_x, _ = visualize_tree(root_node, G, (pos_x, pos_y))
        if prev:
            G.add_edge(str(prev.key), str(root_node.key))
        prev = root_node
        root_node = root_node.sibling
    nx.draw(G,
            nx.get_node_attributes(G, 'pos'),
            with_labels=True,
            node_size=400,
            node_color='#aaaaaa')
    plt.gca().invert_yaxis()
    plt.show()


heap = BinomialHeap()
heap.insert(*range(71))
heap.extract_min()
visualize_binomial_heap(heap)
