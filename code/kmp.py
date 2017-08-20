def preprocess(word):
    #
    # preprocess word
    # (length of longest 
    #     proper preffix and
    #     proper suffix)
    #
    ret_val = [0 for x in range(len(word) + 1)]
    ret_val[0] = -1
    length = -1

    for i, char in enumerate(word):
        while length >= 0 and char != word[length]:
            length = ret_val[length]

        length = length + 1
        ret_val[i + 1] = length

    return ret_val


def kmp(text, word):
    #
    # match word
    #
    ret_val = []
    j = 0
    preprocessed = preprocess(word)
    for i, char in enumerate(text):
        while j >= 0 and char != word[j]:
            j = preprocessed[j]

        j = j + 1
        if j == len(word):
            ret_val.append(i - j + 1)
            j = preprocessed[j]

    return ret_val


if __name__ == '__main__':
    print(kmp("    hello, wordhello", "hello"))
