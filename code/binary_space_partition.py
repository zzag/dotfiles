class Segment:

    def __init__(self, start, end):
        self.start = start
        self.end = end

    def split(self, other):
        A = []
        b = []

        # get line equations
        A.append([self.start[1] - self.end[1], self.end[0] - self.start[0]])
        A.append([other.start[1] - other.end[1], other.end[0] - other.start[0]])
        b.append(self.end[0]*self.start[1] - self.start[0]*self.end[1])
        b.append(other.end[0]*other.start[1] - other.start[0]*other.end[1])

        # calc determinant
        det = A[0][0]*A[1][1] - A[0][1]*A[1][0]
        if abs(det) < 0.0001:
            return None

        det_x = b[0]*A[1][1] - b[1]*A[0][1]
        det_y = A[0][0]*b[1] - A[1][0]*b[0]
        p = det_x/det, det_y/det
        return Segment(self.start, p), Segment(p, self.end)

    def facing(self, point):
        u = self.end[0] - self.start[0], self.end[1] - self.start[1]
        v = point[0] - self.start[0], point[1] - self.start[1]
        return u[0]*v[1] - u[1]*v[0]

    def side(self, other):
        num_front = 0
        num_behind = 0

        # classify each point
        if self.facing(other.start) < -0.0001:
            num_behind += 1
        elif self.facing(other.start) > 0.0001:
            num_front += 1

        if self.facing(other.end) < -0.0001:
            num_behind += 1
        elif self.facing(other.end) > 0.0001:
            num_front += 1

        if num_front == 0 and num_behind > 0:
            return "behind"
        elif num_behind == 0 and num_front > 0:
            return "infront"
        elif num_behind == 0 and num_front == 0:
            return "coincident"

        return "spanning"
    
    def __repr__(self):
        return "<Segment: s: %s, e: %s>" % (str(self.start), str(self.end))


class Node:

    def __init__(self):
        self.segments = []
        self.left = None
        self.right = None


def binary_space_partition(segments):
    if len(segments) == 0:
        return None

    n = len(segments) // 2
    s = segments[n]
    infront = []
    behind = []

    node = Node()

    for segment in segments:
        side = s.side(segment)

        if side == "coincident":
            node.segments.append(segment)
        elif side == "infront":
            infront.append(segment)
        elif side == "behind":
            behind.append(segment)
        else:
            s1, s2 = segment.split(s)
            
            if s.side(s1) == "behind":
                behind.append(s1)
                infront.append(s2)
            elif s.side(s1) == "infront":
                infront.append(s1)
                behind.append(s2)
            else:
                raise Exception("sounds like it has a bug")

    node.left = binary_space_partition(infront)
    node.right = binary_space_partition(behind)
    return node

