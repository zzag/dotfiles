def _pow(n, b, z=0):
    num_bits = b.bit_length()
    result = 1
    for i in range(num_bits, -1, -1):
        result *= result
        if b & (1 << i):
            result *= n
        if z > 0:
            result %= z
    return result
