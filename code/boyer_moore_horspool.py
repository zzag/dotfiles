def bad_rule_preprocess(pattern):
    n = len(pattern)
    bad_rule = dict.fromkeys(pattern, 0)
    bad_rule['other'] = n

    for i, char in enumerate(pattern):
        bad_rule[char] = max(1, n - i - 1)

    return bad_rule


def bad_rule_skip(bad_rule, char):
    if char in bad_rule.keys():
        return bad_rule[char]
    else:
        return bad_rule['other']


def compare(pattern_text, pattern):
    pattern_text_rev = list(reversed(pattern_text))
    pattern_rev = list(reversed(pattern))
    for i, char in enumerate(pattern_text_rev):
        if char != pattern_rev[i]:
            return char

    return ''


def boyer_moore_horspool(text, pattern):
    bad_rule = bad_rule_preprocess(pattern)
    pattern_len = len(pattern)
    text_len = len(text)
    offset = 0
    i = 0
    matched = []
    
    while i <= text_len - pattern_len:
        pattern_text = text[i:i+pattern_len]
        last_fail_char = compare(pattern_text, pattern)
        if last_fail_char == '':
            matched.append(i)
            last_fail_char = pattern[0]

        i += bad_rule_skip(bad_rule, last_fail_char)

    return matched

if __name__ == '__main__':
    print(boyer_moore_horspool('HEREXAMPLE IS A SIMPLE EXAMPLE', 'EXAMPLE'))

