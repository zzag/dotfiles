import math


def lower_square_root(u):
    """get number of the most significant bits"""
    return 2**math.ceil(math.log2(u)/2)


def upper_square_root(u):
    """get number of the least significant bits"""
    return 2**math.floor(math.log2(u)/2)


def high(x, u):
    return math.floor(x/lower_square_root(u))


def low(x, u):
    return x % lower_square_root(u)


def index(x, y, u):
    return x*lower_square_root(u) + y


class Tree:

    def __init__(self, u):
        self.u = u
        self.min = None
        self.max = None
        self.summary = None
        self.clusters = None
        if u > 2:
            child_size = lower_square_root(u)
            num_child = upper_square_root(u)
            self.summary = Tree(num_child)
            self.clusters = list([Tree(child_size) for _ in range(num_child)])

    def search(self, x):
        if self.min == x or self.max == x:
            return True
        elif self.u == 2:
            return False
        i = high(x, self.u)
        offset = low(x, self.u)
        return self.clusters[i].search(offset)

    def insert(self, x):
        if self.min is None:
            self.min = x
            self.max = x
            return
        if x < self.min:
            x, self.min = self.min, x
        if x > self.max:
            self.max = x
        if self.u > 2:
            i = high(x, self.u)
            offset = low(x, self.u)
            if self.clusters[i].min is None:
                self.summary.insert(i)
                self.clusters[i].min = offset
                self.clusters[i].max = offset
            else:
                self.clusters[i].insert(offset)

    def delete(self, x):
        if self.min == self.max:
            self.min = None
            self.max = None
        elif self.u == 2:
            self.min = 1 if x == 0 else 0
            self.max = self.min
        else:
            if self.min == x:
                first = self.summary.min
                x = index(first, self.clusters[first].min, self.u)
                self.min = x
            c = high(x, self.u)
            offset = low(x, self.u)
            self.clusters[c].delete(offset)
            if self.clusters[c].min is None:
                self.summary.delete(c)
            if self.max == x:
                last = self.summary.max
                if last is None:
                    self.max = self.min
                else:
                    self.max = index(last, self.clusters[last].max, self.u)

    def successor(self, x):
        if self.u == 2:
            return 1 if x == 0 and self.max == 1 else None
        elif self.min is not None and x < self.min:
            return self.min
        i = high(x, self.u)
        offset = low(x, self.u)
        sup = self.clusters[i].max
        if sup is not None and offset < sup:
            return index(i, self.clusters[i].successor(offset), self.u)
        succ_claster = self.summary.successor(i)
        if succ_claster is None:
            return None
        return index(succ_claster, self.clusters[succ_claster].min, self.u)

    def predecessor(self, x):
        if self.u == 2:
            return 0 if self.min == 0 and x == 1 else None
        elif self.max is not None and x > self.max:
            return self.max
        i = high(x, self.u)
        offset = low(x, self.u)
        inf = self.clusters[i].min
        if inf is not None and offset > inf:
            return index(i, self.clusters[i].predecessor(offset), self.u)
        pred_cluster = self.summary.predecessor(i)
        if pred_cluster is None:
            if self.min is not None and x > self.min:
                return self.min
            return None
        return index(pred_cluster, self.clusters[pred_cluster].max, self.u)
