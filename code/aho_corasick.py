import collections


class Node:

    def __init__(self, key, parent=None, fail=None):
        self.key = key
        self.fail = fail
        self.parent = parent
        self.children = []
        self.accept = False

    def find_child_with(self, key):
        for child in self.children:
            if child.key == key:
                return child

        return None

    def __lt__(self, other):
        # is needed to sort nodes in children list
        return self.key < other.key

    def __repr__(self):
        if self.key == "":
            return "*"

        node_str = str(self.key)
        if self.accept is True:
            node_str += "+"

        if self.fail is None:
            node_str += " -> *"
        elif self.fail.key == "":
            node_str += " -> *"
        else:
            node_str += " -> " + str(self.fail.key)

        return node_str


class Trie:

    def __init__(self):
        self.root = Node('')
        self.root.fail = self.root

    def add(self, keyword):
        self.add_to_trie(keyword)
        self.fill_fail_field(self.root)

    def add_to_trie(self, keyword):
        node = self.root

        for sym in keyword:
            # try to find child with key @sym
            transition = node.find_child_with(sym)

            # create node if there is no such child
            if transition is None:
                transition = Node(sym, node, self.root)
                node.children.append(transition)
                node.children.sort()

            node = transition

        # last node is last char of the keyword
        node.accept = True
        return node

    def fill_fail_field(self, node):
        # example of failure function(BFS):
        # http://blog.ivank.net/aho-corasick-algorithm-in-as3.html

        q = collections.deque()
        q.append(node)

        while len(q):
            node = q.popleft()

            # append children
            for child in node.children:
                q.append(child)

            if node == self.root:
                continue
            elif node.parent == self.root:
                node.fail = self.root
                continue

            fail = node.parent.fail

            while fail.find_child_with(node.key) is None and fail != self.root:
                # move up in the trie(try to extend longest suffix)
                fail = fail.fail

            node.fail = fail.find_child_with(node.key)

            if node.fail is None:
                node.fail = self.root

    def search(self, text):
        node = self.root

        text = text + " "
        for char in text:
            transition = node.find_child_with(char)

            if transition is None:
                transition = node.fail

            if node.accept is True:
                print("Matched " + self.get_keyword(node) + "!")

            node = transition

    def get_keyword(self, node):
        keyword = ""

        while node != self.root:
            keyword = node.key + keyword
            node = node.parent

        return keyword

    def to_string(self, node, level=0):
        # to_string current node
        ret_val = " " * level + str(node) + "\n"

        # to_string all children
        for child in node.children:
            ret_val += self.to_string(child, level + 2)

        return ret_val

    def __repr__(self):
        return self.to_string(self.root)


def aho_corasick(keywords, text):
    trie = Trie()

    for keyword in keywords:
        trie.add(keyword)

    trie.search(text)


if __name__ == '__main__':
    aho_corasick(["she", "hers", "his"], "hersloloshelohis")
