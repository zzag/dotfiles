def extended_gcd(a, b):
    """Compute gcd and the coefficients of Bezout's identity
    Bezout's identityt is given by gcd(a, b) = as + bt.
    """
    r_prev, r = a, b
    s_prev, s = 1, 0
    t_prev, t = 0, 1

    while r:
        q = r_prev // r
        r_prev, r = r, r_prev - q * r
        s_prev, s = s, s_prev - q * s
        t_prev, t = t, t_prev - q * t

    # r_prev = a * s_prev + b * t_prev
    return r_prev, s_prev, t_prev
