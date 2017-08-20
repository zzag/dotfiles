#include <iostream>
#include <bitset>
#include <cstdint>
#include <vector>
#include <climits>
#include <algorithm>


typedef uint8_t uint4_t;
typedef uint8_t uint6_t;
typedef uint32_t uint28_t;
typedef uint64_t uint48_t;
typedef uint64_t uint56_t;

#define INT64_HIGH(x) ((x) >> 32)
#define INT64_LOW(x) ((x) & 0xffffffff)
#define UINT64_MIX(hi, lo) (((uint64_t)(hi) << 32) | (lo))


using std::cout;
using std::endl;
using std::vector;


vector<uint32_t> INITIAL_PERMUTATION = {
    58,  50,  42,  34,  26,  18,  10,  2,
    60,  52,  44,  36,  28,  20,  12,  4,
    62,  54,  46,  38,  30,  22,  14,  6,
    64,  56,  48,  40,  32,  24,  16,  8,
    57,  49,  41,  33,  25,  17,  9,   1,
    59,  51,  43,  35,  27,  19,  11,  3,
    61,  53,  45,  37,  29,  21,  13,  5,
    63,  55,  47,  39,  31,  23,  15,  7
};


vector<uint32_t> FINAL_PERMUTATION = {
    40,  8,  48,  16,  56,  24,  64,  32,
    39,  7,  47,  15,  55,  23,  63,  31,
    38,  6,  46,  14,  54,  22,  62,  30,
    37,  5,  45,  13,  53,  21,  61,  29,
    36,  4,  44,  12,  52,  20,  60,  28,
    35,  3,  43,  11,  51,  19,  59,  27,
    34,  2,  42,  10,  50,  18,  58,  26,
    33,  1,  41,  9,   49,  17,  57,  25
};


vector<uint32_t> EXPANSION_TABLE = {
    32,  1,   2,   3,   4,   5,
    4,   5,   6,   7,   8,   9,
    8,   9,   10,  11,  12,  13,
    12,  13,  14,  15,  16,  17,
    16,  17,  18,  19,  20,  21,
    20,  21,  22,  23,  24,  25,
    24,  25,  26,  27,  28,  29,
    28,  29,  30,  31,  32,  1
};


vector<uint4_t> S1 = {
    14,  4,   13,  1,  2,   15,  11,  8,   3,   10,  6,   12,  5,   9,   0,  7,
    0,   15,  7,   4,  14,  2,   13,  1,   10,  6,   12,  11,  9,   5,   3,  8,
    4,   1,   14,  8,  13,  6,   2,   11,  15,  12,  9,   7,   3,   10,  5,  0,
    15,  12,  8,   2,  4,   9,   1,   7,   5,   11,  3,   14,  10,  0,   6,  13
};

vector<uint4_t> S2 = {
    15,  1,   8,   14,  6,   11,  3,   4,   9,   7,  2,   13,  12,  0,  5,   10,
    3,   13,  4,   7,   15,  2,   8,   14,  12,  0,  1,   10,  6,   9,  11,  5,
    0,   14,  7,   11,  10,  4,   13,  1,   5,   8,  12,  6,   9,   3,  2,   15,
    13,  8,   10,  1,   3,   15,  4,   2,   11,  6,  7,   12,  0,   5,  14,  9
};

vector<uint4_t> S3 = {
    10,  0,   9,   14,  6,  3,   15,  5,   1,   13,  12,  7,   11,  4,   2,   8,
    13,  7,   0,   9,   3,  4,   6,   10,  2,   8,   5,   14,  12,  11,  15,  1,
    13,  6,   4,   9,   8,  15,  3,   0,   11,  1,   2,   12,  5,   10,  14,  7,
    1,   10,  13,  0,   6,  9,   8,   7,   4,   15,  14,  3,   11,  5,   2,   12
};

vector<uint4_t> S4 = {
    7,   13,  14,  3,  0,   6,   9,   10,  1,   2,  8,  5,   11,  12,  4,   15,
    13,  8,   11,  5,  6,   15,  0,   3,   4,   7,  2,  12,  1,   10,  14,  9,
    10,  6,   9,   0,  12,  11,  7,   13,  15,  1,  3,  14,  5,   2,   8,   4,
    3,   15,  0,   6,  10,  1,   13,  8,   9,   4,  5,  11,  12,  7,   2,   14
};

vector<uint4_t> S5 = {
    2,   12,  4,   1,   7,   10,  11,  6,   8,   5,   3,   15,  13,  0,  14,  9,
    14,  11,  2,   12,  4,   7,   13,  1,   5,   0,   15,  10,  3,   9,  8,   6,
    4,   2,   1,   11,  10,  13,  7,   8,   15,  9,   12,  5,   6,   3,  0,   14,
    11,  8,   12,  7,   1,   14,  2,   13,  6,   15,  0,   9,   10,  4,  5,   3
};

vector<uint4_t> S6 = {
    12,  1,   10,  15,  9,  2,   6,   8,   0,   13,  3,   4,   14,  7,   5,   11,
    10,  15,  4,   2,   7,  12,  9,   5,   6,   1,   13,  14,  0,   11,  3,   8,
    9,   14,  15,  5,   2,  8,   12,  3,   7,   0,   4,   10,  1,   13,  11,  6,
    4,   3,   2,   12,  9,  5,   15,  10,  11,  14,  1,   7,   6,   0,   8,   13
};

vector<uint4_t> S7 = {
    4,   11,  2,   14,  15,  0,  8,   13,  3,   12,  9,  7,   5,   10,  6,  1,
    13,  0,   11,  7,   4,   9,  1,   10,  14,  3,   5,  12,  2,   15,  8,  6,
    1,   4,   11,  13,  12,  3,  7,   14,  10,  15,  6,  8,   0,   5,   9,  2,
    6,   11,  13,  8,   1,   4,  10,  7,   9,   5,   0,  15,  14,  2,   3,  12
};

vector<uint4_t> S8 = {
    13,  2,   8,   4,  6,   15,  11,  1,   10,  9,   3,   14,  5,   0,   12,  7,
    1,   15,  13,  8,  10,  3,   7,   4,   12,  5,   6,   11,  0,   14,  9,   2,
    7,   11,  4,   1,  9,   12,  14,  2,   0,   6,   10,  13,  15,  3,   5,   8,
    2,   1,   14,  7,  4,   10,  8,   13,  15,  12,  9,   0,   3,   5,   6,   11
};

vector<uint32_t> BLOCK_PERMUTATION = {
    16,  7,   20,  21,  29,  12,  28,  17,
    1,   15,  23,  26,  5,   18,  31,  10,
    2,   8,   24,  14,  32,  27,  3,   9,
    19,  13,  30,  6,   22,  11,  4,   25
};

vector<uint32_t> PC1_LEFT = {
    57,  49,  41,  33,  25,  17,  9,
    1,   58,  50,  42,  34,  26,  18,
    10,  2,   59,  51,  43,  35,  27,
    19,  11,  3,   60,  52,  44,  36
};

vector<uint32_t> PC1_RIGHT = {
    63,  55,  47,  39,  31,  23,  15,
    7,   62,  54,  46,  38,  30,  22,
    14,  6,   61,  53,  45,  37,  29,
    21,  13,  5,   28,  20,  12,  4
};


vector<uint32_t> PC2 {
    14,  17,  11,  24,  1,   5,
    3,   28,  15,  6,   21,  10,
    23,  19,  12,  4,   26,  8,
    16,  7,   27,  20,  13,  2,
    41,  52,  31,  37,  47,  55,
    30,  40,  51,  45,  33,  48,
    44,  49,  39,  56,  34,  53,
    46,  42,  50,  36,  29,  32
};

vector<uint32_t> NUM_ROTATIONS = {
    1,  1,  2,  2,
    2,  2,  2,  2,
    1,  2,  2,  2,
    2,  2,  2,  1
};


template <typename T>
T
rorl(T n, int b, int num_bits=32) {
    const T one = 1;
    const T mask = (one << num_bits) - 1;
    b %= num_bits;
    return ((n << b) & mask) | (n >> (num_bits - b));
}


template<typename From, typename To>
To
permuted_choice(From n, vector<uint32_t>& table, uint32_t from_bits=0) {
    To result = 0;
    const To one = 1;
    auto to_bits = table.size();

    if (from_bits == 0) {
        from_bits = to_bits;
    }

    for (auto i = 0; i < to_bits; i++) {
        result <<= 1;
        if (n & (one << (from_bits - table[i]))) {
            result |= 1;
        }
    }
    return result;
}


uint4_t lookup_sbox(uint6_t n, vector<uint4_t>& table) {
    // Given 6-bit input, 4-bit output is found by selecting
    // row using outer two bits, and the column using the
    // inner four bits.
    uint32_t row = (n >> 4) + (n & 1);
    uint32_t column = (n >> 1) & 0xf;
    return table[row * 16 + column];
}


uint32_t
build_mask(uint32_t data, uint48_t key) {
    // Expand 32-bit half-block to 48 bits.
    uint48_t expanded = permuted_choice<uint32_t, uint48_t>(
        data, EXPANSION_TABLE, 32);

    uint48_t key_mixed = expanded ^ key;

    // Divide block into 8 6-bit pieces and process the 
    // blocks with substitution boxes. Each of the eight
    // S-boxes replaces its six input bits with four output
    // bits according a lookup table.
    uint6_t mask = (1 << 6) - 1;
    uint4_t s1 = lookup_sbox((key_mixed >> 42) & mask, S1);
    uint4_t s2 = lookup_sbox((key_mixed >> 36) & mask, S2);
    uint4_t s3 = lookup_sbox((key_mixed >> 30) & mask, S3);
    uint4_t s4 = lookup_sbox((key_mixed >> 24) & mask, S4);
    uint4_t s5 = lookup_sbox((key_mixed >> 18) & mask, S5);
    uint4_t s6 = lookup_sbox((key_mixed >> 12) & mask, S6);
    uint4_t s7 = lookup_sbox((key_mixed >> 6) & mask, S7);
    uint4_t s8 = lookup_sbox((key_mixed >> 0) & mask, S8);

    // Combine together outputs of each S-boxes.
    uint32_t result = ((uint32_t)s1 << 28) | ((uint32_t)s2 << 24) |
                      ((uint32_t)s3 << 20) | ((uint32_t)s4 << 16) |
                      ((uint32_t)s5 << 12) | ((uint32_t)s6 << 8) |
                      ((uint32_t)s7 << 4) | s8;

    // Rearrange outputs from S-boxes according to a fixed permutation.
    result = permuted_choice<uint32_t, uint32_t>(result, BLOCK_PERMUTATION);

    return result;
}


uint64_t
process_round(uint64_t cipher, uint48_t key) {
    uint32_t left = INT64_HIGH(cipher);
    uint32_t right = INT64_LOW(cipher);

    // `build_mask` is computing output of the F-function.
    uint32_t mask = build_mask(right, key);

    uint32_t result_left = right;
    uint32_t result_right = left ^ mask;
    uint64_t result = UINT64_MIX(result_left, result_right);

    return result;
}


vector<uint48_t>
compute_subkeys(uint64_t key) {
    vector<uint48_t> subkeys;

    // Select 56 bits of the key. The remaining 8 bits are discarded.
    uint28_t bits_left = permuted_choice<uint64_t, uint28_t>(key, PC1_LEFT);
    uint28_t bits_right = permuted_choice<uint64_t, uint28_t>(key, PC1_RIGHT);

    for (int i = 0; i < 16; i++) {
        bits_left = rorl(bits_left, NUM_ROTATIONS[i], 28);
        bits_right = rorl(bits_right, NUM_ROTATIONS[i], 28);
        uint56_t state = ((uint56_t)bits_left << 28) | bits_right;
        uint48_t subkey = permuted_choice<uint56_t, uint48_t>(state, PC2);
        subkeys.push_back(subkey);
    }

    return subkeys;
}


uint64_t
encode_decode_block(uint64_t block, vector<uint48_t> subkeys) {
    // Encoding and decoding looks pretty much the same. The only
    // difference is order of subkeys.

    uint64_t cipher = block;

    cipher = permuted_choice<uint64_t, uint64_t>(cipher, INITIAL_PERMUTATION);

    for (auto subkey : subkeys) {
        cipher = process_round(cipher, subkey);
    }

    uint32_t left = INT64_HIGH(cipher);
    uint32_t right = INT64_LOW(cipher);
    cipher = UINT64_MIX(right, left);

    cipher = permuted_choice<uint64_t, uint64_t>(cipher, FINAL_PERMUTATION);

    return cipher;
}


uint64_t
encrypt_block(uint64_t plaintext, uint64_t key) {
    vector<uint48_t> subkeys = compute_subkeys(key);
    return encode_decode_block(plaintext, subkeys);
}


uint64_t
decrypt_block(uint64_t cipher, uint64_t key) {
    vector<uint48_t> subkeys = compute_subkeys(key);
    reverse(subkeys.begin(), subkeys.end());
    return encode_decode_block(cipher, subkeys);
}


int
main() {
    uint64_t value = 334000000;
    uint64_t key = 13;
    uint64_t cipher = encrypt_block(value, key);
    uint64_t plain = decrypt_block(cipher, key);
    cout << ":: Value = " << value << endl;
    cout << ":: Cipher = " << cipher << endl;
    cout << ":: Decrypted = " << plain << endl;
    return 0;
}
