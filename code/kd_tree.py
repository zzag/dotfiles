class Node:

    def __init__(self, point=None):
        self.point = point
        self.left = None
        self.right = None

    def __repr__(self):
        return "<Point: %s>" % str(self.point)


def build_kd_tree(points, level=0, max_level=2):
    if len(points) == 0:
        return None

    points = sorted(points, key=lambda x: x[level])
    
    i = len(points) // 2
    before = points[:i]
    after = points[i+1:]
    node = Node(points[i])

    level = (level + 1) % max_level
    node.left = build_kd_tree(before, level, max_level)
    node.right = build_kd_tree(after, level, max_level)
    return node

