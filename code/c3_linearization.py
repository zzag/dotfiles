def merge(*seqs):
    """C3 linearization

    # Examples

    L(O) = [O]
    L(X) = [X, O]
    L(Y) = [Y, O]

    L(A) = [A] + merge([X, O], [Y, O], [X, Y])
         = [A, X, Y, O]

    L(B) = [B] + merge([Y, O], [X, O], [Y, X])
         = [B, Y, X, O]

    L(C) = [C] + merge([A, X, Y, O], [B, Y, X, O], [A, B])
         = [C, A] + merge([X, Y, O], [B, Y, X, O], [B])
         = [C, A, B] + merge([X, Y, O], [Y, X, O])
         = unable to merge [X, Y, O] and [Y, X, O]
    """
    _seqs = [list(seq) for seq in seqs]
    res = []

    while True:
        _seqs = [seq for seq in _seqs if seq]
        if not _seqs:
            return res

        for seq in _seqs:
            head = seq[0]
            good_head = all(head not in s[1:] for s in _seqs)
            if good_head:
                break
            head = None

        if not head:
            raise ValueError('unable to merge input sequences')

        res.append(head)
        for seq in _seqs:
            if seq[0] == head:
                del seq[0]

    return res
