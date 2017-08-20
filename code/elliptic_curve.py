def extended_euclidean(a, b):
    r_prev, r = a, b
    s_prev, s = 1, 0
    t_prev, t = 0, 1
    while r:
        q = r_prev // r
        r_prev, r = r, r_prev - q*r
        s_prev, s = s, s_prev - q*s
        t_prev, t = t, t_prev - q*t
    return s_prev, t_prev


def find_inverse(x, p):
    inv, _ = extended_euclidean(x, p)
    return inv


class EllipticCurve:
    """Elliptic curve modulo p.

    y^2 = x^3 + ax + b (mod p)

    Elliptic curve is singular if its discriminant -16*(4a^3 + 27b^2)
    is zero modulo p. If discriminant is zero polynomial has repeated
    roots, cusps, etc.
    """

    POINT_AT_INFINITY = (float('inf'), float('inf'))

    def __init__(self, a, b, p):
        discriminant = 16 * (4 * a**3 + 27 * b**2)
        if discriminant % p == 0:
            raise ValueError('singular elliptic curve')
        self.a = a
        self.b = b
        self.p = p

    def add(self, p1, p2):
        """Add two points p1 and p2."""
        x1, y1 = p1
        x2, y2 = p2
        a, p = self.a, self.p

        if p1 == self.POINT_AT_INFINITY:
            return p2
        elif p2 == self.POINT_AT_INFINITY:
            return p1
        elif x1 == x2 and y1 != y2:
            # We can rewrite this condition as
            # x1 == x2 and find_inverse(-y1, p) == y2
            # but, y1 != y2 is more efficient.
            # This is correct because vertical line intersects
            # the elliptic curve in one or two points.
            return self.POINT_AT_INFINITY

        if p1 == p2:
            m = (3 * x1**2 + a) * find_inverse(2 * y1, p) % p
        else:
            m = (y2 - y1) * find_inverse(x2 - x1, p) % p

        x3 = (m**2 - x1 - x2) % p
        y3 = (m * (x1 - x3) - y1) % p
        result = (x3, y3)

        return result

    def pow(self, n, b):
        """Raise n to the power of b.

        Note:
            With "+" group operation this is the same as
            "add point 'n' b times".
        """
        num_bits = b.bit_length()
        res = self.POINT_AT_INFINITY
        for i in range(num_bits, -1, -1):
            res = self.add(res, res)
            if b & (1 << i):
                res = self.add(res, n)
        return res


curve = EllipticCurve(a=2, b=2, p=17)
a = (5, 1)
print(curve.pow(a, 2))
