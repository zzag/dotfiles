def hash(text, base, prime):
    h = 0

    for i in range(len(text)):
        h = (h + ord(text[-i - 1]) * (base ** i)) % prime

    return h


def rabin_karp(text, word):
    ret_val = []
    n = len(word)
    rolling_hash = hash(text[:n], n, 997)
    word_hash = hash(word, n, 997)

    text = text + " " # rehash at the end
    for i, x in enumerate(text[n:]):
        if rolling_hash == word_hash:
            if text[i:n+i] == word:
                ret_val.append(i)

        # rehash
        # TODO: def rolling_hash(): ...
        prev_char = text[i]
        rolling_hash = n * (rolling_hash - ord(prev_char) * (n ** (n - 1)))
        rolling_hash = rolling_hash + ord(x)
        rolling_hash = rolling_hash % 997

    return ret_val


if __name__ == '__main__':
    print(rabin_karp("hello whellorldhello", "hello"))
