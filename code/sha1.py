import struct


INITIAL_DIGEST = (0x67452301,
                  0xEFCDAB89,
                  0x98BADCFE,
                  0x10325476,
                  0xC3D2E1F0)

STAGE_FUNCS = (lambda b, c, d: (b & c) | (d & ~b),
               lambda b, c, d: b ^ c ^ d,
               lambda b, c, d: (b & c) | (b & d) | (c & d),
               lambda b, c, d: b ^ c ^ d)

STAGE_CONSTS = (0x5A827999,
                0x6ED9EBA1,
                0x8F1BBCDC,
                0xCA62C1D6)

NUM_ROUNDS = 80


def rotate_left(n, b, nbits=32):
    mask = 2**nbits - 1
    b %= nbits
    n &= mask
    return ((n << b) & mask) | (n >> (nbits - b))


def compress_block(block, state):
    w = [0] * NUM_ROUNDS
    mask = 0xffffffff
    a, b, c, d, e = state

    for i in range(16):
        w[i], = struct.unpack(b'>I', block[i*4:i*4+4])
    for i in range(16, NUM_ROUNDS):
        w[i] = rotate_left(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1)

    for i in range(NUM_ROUNDS):
        stage = i // 20
        f = STAGE_FUNCS[stage]
        k = STAGE_CONSTS[stage]

        new_e = (f(b, c, d) + e + rotate_left(a, 5) + w[i] + k) & mask
        new_b = rotate_left(b, 30)
        a, b, c, d, e = (new_e, a, new_b, c, d)

    h0, h1, h2, h3, h4 = state
    h0 = (h0 + a) & mask
    h1 = (h1 + b) & mask
    h2 = (h2 + c) & mask
    h3 = (h3 + d) & mask
    h4 = (h4 + e) & mask

    return h0, h1, h2, h3, h4


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


def sha1(message):
    state = INITIAL_DIGEST

    for block in blocks(message):
        state = compress_block(block, state)

    return '%08x%08x%08x%08x%08x' % state


data = bytes('Hello, world!', encoding='latin-1')
print(sha1(data))
