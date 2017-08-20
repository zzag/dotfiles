import struct


INITIAL_DIGEST = (
    0xc1059ed8,
    0x367cd507,
    0x3070dd17,
    0xf70e5939,
    0xffc00b31,
    0x68581511,
    0x64f98fa7,
    0xbefa4fa4)

ROUND_CONSTS = (
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2)

NUM_ROUNDS = 64


def rotate_right(n, b, nbits=32):
    mask = 2**nbits - 1
    b %= nbits
    n &= mask
    return (n >> b | ((n << (nbits - b)) & mask))


def compress_block(block, state):
    w = [0] * NUM_ROUNDS
    mask = 0xffffffff
    a, b, c, d, e, f, g, h = state

    for i in range(16):
        w[i], = struct.unpack(b'>I', block[i*4:i*4+4])
    for i in range(16, NUM_ROUNDS):
        s0 = rotate_right(w[i - 15], 7) ^ rotate_right(w[i - 15], 18) ^ (w[i - 15] >> 3)
        s1 = rotate_right(w[i - 2], 17) ^ rotate_right(w[i - 2], 19) ^ (w[i - 2] >> 10)
        w[i] = (w[i - 16] + s0 + w[i - 7] + s1) & mask

    for i in range(NUM_ROUNDS):
        k = ROUND_CONSTS[i]
        ch = (e & f) ^ (g & ~e)
        s1 = rotate_right(e, 6) ^ rotate_right(e, 11) ^ rotate_right(e, 25)
        temp1 = w[i] + k + h + ch + s1
        maj = (a & b) ^ (a & c) ^ (b & c)
        s0 = rotate_right(a, 2) ^ rotate_right(a, 13) ^ rotate_right(a, 22)
        temp2 = s0 + maj

        new_d = (d + temp1) & mask
        new_h = (temp1 + temp2) & mask
        a, b, c, d, e, f, g, h = (new_h, a, b, c, new_d, e, f, g)

    h0, h1, h2, h3, h4, h5, h6, h7 = state
    h0 = (h0 + a) & mask
    h1 = (h1 + b) & mask
    h2 = (h2 + c) & mask
    h3 = (h3 + d) & mask
    h4 = (h4 + e) & mask
    h5 = (h5 + f) & mask
    h6 = (h6 + g) & mask
    h7 = (h7 + h) & mask
    return h0, h1, h2, h3, h4, h5, h6, h7

def blocks(message, n=64):
    """Split a message into blocks of 512-bits.

    Args:
        message (bytes, bytearray): Input message.
        n (int): Size of each block
    """
    # Preprocess the message:
    #   1. Append '1' bit;
    #   2. Append 0 <= k < 512 bits '0', s.t. the resulting
    #      message length in bits is congurent to 448 (mod 512);
    #   3. Append length of the original message, as a 64-bit
    #      big-endian integer.
    message_size = len(message)
    message += b'\x80'
    message += b'\x00' * (((n - 8) - (message_size + 1)) % n)
    message += struct.pack(b'>Q', message_size * 8)

    view = memoryview(message)
    for i in range(0, len(message), n):
        yield view[i:i+n]


def sha224(message):
    state = INITIAL_DIGEST

    for block in blocks(message):
        state = compress_block(block, state)

    return '%08x%08x%08x%08x%08x%08x%08x' % state[:-1]


data = bytes('Hello, world!', encoding='latin-1')
print(sha224(data))
