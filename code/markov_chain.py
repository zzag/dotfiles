import random
import bisect
import nltk
import re
import pickle
import gzip
import itertools
import json

from tqdm import tqdm
from collections import Counter, defaultdict


train = False


def choice(choices):
    """
    Weighted random choice
    """
    weights = map(lambda c: c[1], choices)
    weight_sum = tuple(itertools.accumulate(weights))
    at = random.random() * weight_sum[-1]
    idx = bisect.bisect_right(weight_sum, at)
    return choices[idx][0]


def save_model(model, filename):
    """
    Save model to the file
    """
    with gzip.open(filename, 'wb') as f:
        pickle.dump(model, f)


def load_model(filename):
    """
    Load model from the file
    """
    with gzip.open(filename, 'rb') as f:
        model = pickle.load(f)
    return model


class MarkovChain:

    def __init__(self, end_token=None):
        self.end_token = end_token if end_token else '<END>'
        self.word_counter = defaultdict(Counter)
        self.start_words = []

    def learn_sentence(self, sentence):
        """
        Learn a sentence
        """
        tokens = nltk.word_tokenize(sentence)
        tokens.append(self.end_token)

        if tokens[0] not in self.start_words:
            self.start_words.append(tokens[0])

        for i, token in enumerate(tokens[:-1]):
            next_token = tokens[i + 1]
            self.word_counter[token][next_token] += 1

    def learn_text(self, text):
        """
        Learn text
        """
        for sentence in nltk.sent_tokenize(text):
            self.learn_sentence(sentence)

    def learn_generator(self, generator, num_samples):
        """
        Learn from a generator
        """
        for i, text in tqdm(enumerate(generator), total=num_samples):
            self.learn_text(text)

    def generate_sentence(self, max_length=100):
        """
        Generate random sentence
        """
        sentence = []
        word = random.choice(self.start_words)
        i = 0

        while word != self.end_token and i < max_length:
            sentence.append(word)
            word = choice(tuple(self.word_counter[word].items()))
            i += 1

        return re.sub(r'\s(\.|,|:|;|\?|!)', r'\1', ' '.join(sentence))

    def generate_text(self, num_sentences=100, max_length=100):
        """
        Generate random text
        """
        sentences = [self.generate_sentence(max_length)
                     for i in range(num_sentences)]
        return ' '.join(sentences)


def reddit_comments(filename):
    with open(filename, 'r') as f:
        for line in f:
            comment = json.loads(line)
            body = comment['body']
            if body == '[deleted]':
                continue
            yield body


if train:
    num_train_comments = 100000
    comments = reddit_comments('comments.json')
    train_comments = itertools.islice(comments, num_train_comments)
    chain = MarkovChain()
    chain.learn_generator(train_comments, num_train_comments)
    save_model(chain, 'model.pkl.gzip')
else:
    chain = load_model('model.pkl.gzip')

for i in range(20):
    print(chain.generate_sentence())
