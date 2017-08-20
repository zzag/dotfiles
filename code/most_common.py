words = "Hello pizza world pizza pizza".split()
word_counter = {}

for word in words:
    if word in word_counter:
        word_counter[word] += 1
    else:
        word_counter[word] = 1

most_common = sorted(word_counter, key=word_counter.get, reverse=True)[:2]

print(most_common)
