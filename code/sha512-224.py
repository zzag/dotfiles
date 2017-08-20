import struct


# calculated with SHA-512/t IV Generation function(see section 5.3.6)
# http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
INITIAL_DIGEST = (
    0x8c3d37c819544da2,
    0x73e1996689dcd4d6,
    0x1dfab7ae32ff9c82,
    0x679dd514582f9fcf,
    0x0f6d2b697bd44da8,
    0x77e36f7304c48942,
    0x3f9d85a86a1d36c8,
    0x1112e6ad91d692a1)

ROUND_CONSTS = (
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
    0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
    0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
    0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
    0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
    0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
    0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
    0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
    0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
    0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
    0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
    0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
    0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
    0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
    0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
    0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
    0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
    0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
    0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
    0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
    0x5fcb6fab3ad6faec, 0x6c44198c4a475817)

NUM_ROUNDS = 80
WORD_SIZE = 8


def rotate_right(n, b, nbits=64):
    mask = 2**nbits - 1
    b %= nbits
    n &= mask
    return (n >> b | ((n << (nbits - b)) & mask))


def compress_block(block, state):
    w = [0] * NUM_ROUNDS
    mask = 2**64 - 1
    a, b, c, d, e, f, g, h = state

    for i in range(16):
        j = i*WORD_SIZE
        w[i], = struct.unpack(b'>Q', block[j:j+WORD_SIZE])
    for i in range(16, NUM_ROUNDS):
        s0 = rotate_right(w[i - 15], 1) ^ rotate_right(w[i - 15], 8) ^ (w[i - 15] >> 7)
        s1 = rotate_right(w[i - 2], 19) ^ rotate_right(w[i - 2], 61) ^ (w[i - 2] >> 6)
        w[i] = (w[i - 16] + s0 + w[i - 7] + s1) & mask

    for i in range(NUM_ROUNDS):
        k = ROUND_CONSTS[i]
        ch = (e & f) ^ (g & ~e)
        s1 = rotate_right(e, 14) ^ rotate_right(e, 18) ^ rotate_right(e, 41)
        temp1 = w[i] + k + h + ch + s1
        maj = (a & b) ^ (a & c) ^ (b & c)
        s0 = rotate_right(a, 28) ^ rotate_right(a, 34) ^ rotate_right(a, 39)
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

def blocks(message, n=128):
    """Split a message into blocks of 512-bits.

    Args:
        message (bytes, bytearray): Input message.
        n (int): Size of each block
    """
    # Preprocess the message:
    #   1. Append '1' bit;
    #   2. Append 0 <= k < 1024 bits '0', s.t. the resulting
    #      message length in bits is congurent to 448 (mod 512);
    #   3. Append length of the original message, as a 128-bit
    #      big-endian integer.
    padded = message + b'\x80'
    padded += b'\x00' * (((n - 16) - len(padded)) % n)
    num_bits = len(message) * 8
    mask = 2**64 - 1
    num_bits_hi = (num_bits >> 64) & mask
    num_bits_lo = num_bits & mask
    padded += struct.pack(b'>QQ', num_bits_hi, num_bits_lo)

    view = memoryview(padded)
    for i in range(0, len(padded), n):
        yield view[i:i+n]


def sha512(message):
    state = INITIAL_DIGEST

    for block in blocks(message):
        state = compress_block(block, state)

    last_dword = state[3]
    return '%08x%08x%08x%08x' % (*state[:3], last_dword >> 32) # nasty code... :(


data = bytes('Holla', encoding='latin-1')
print(sha512(data))
