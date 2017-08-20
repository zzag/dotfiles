class Edge:
    def __init__(self, source, sink, capacity):
        self.source = source
        self.sink = sink
        self.capacity = capacity
        self.backward = None


class MaxFlowNetwork:
    def __init__(self, weight_matrix):
        self.adjacency = {}
        self.residual = {}
        self.flow = {}

        for source, links in weight_matrix:
            self.adjacency[source] = []
            self.residual[source] = []

        for source, links in weight_matrix:
            for sink, capacity in links:
                # direct link
                direct = Edge(source, sink, capacity)
                self.adjacency[source].append(direct)

                # backward link
                backward = Edge(sink, source, 0)
                self.residual[sink].append(backward)

                # flow
                self.flow[direct] = 0
                self.flow[backward] = 0

                # backward
                direct.backward = backward
                backward.backward = direct


    def __get_path(self, source, sink, path):
        if source == sink:
            return path

        for edge in self.adjacency[source]:
            residual = edge.capacity - self.flow[edge]

            if residual > 0 and residual not in path:
                ret_val = self.__get_path(edge.sink, sink, path + [edge])
                if ret_val is not None:
                    return ret_val

        return None

    def sum_flow(self, iterable, **args):
        sum_function = lambda x: x

        if "key" in args:
            sum_function = args["key"]

        ret_val = 0
        for item in iterable:
            ret_val += sum_function(item)

        return ret_val

    def max_flow(self):
        path = self.__get_path("s", "t", [])

        while path is not None:
            residual_path = [edge.capacity - self.flow[edge] for edge in path]
            flow = min(residual_path)

            for edge in path:
                self.flow[edge] += flow
                self.flow[edge.backward] -= flow

            path = self.__get_path("s", "t", [])

        sum_source = lambda x: self.flow[x] if x.source == "s" else 0
        return self.sum_flow(self.flow, key=sum_source)

if __name__ == '__main__':
    # source graph:
    # http://en.wikipedia.org/wiki/Maximum_flow_problem
    weight_matrix = [
        ("s", [("o", 3), ("p", 3)]),
        ("o", [("q", 3), ("p", 2)]),
        ("p", [("r", 2)]),
        ("q", [("t", 2), ("r", 4)]),
        ("r", [("t", 3)]),
        ("t", [])
    ]

    net = MaxFlowNetwork(weight_matrix)
    print(net.max_flow())
