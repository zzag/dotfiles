import _utils
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
import moviepy.editor as mpy

from sklearn.datasets import load_digits
from scipy.spatial.distance import pdist, squareform
from moviepy.video.io.bindings import mplfig_to_npimage

mpl.rcParams['toolbar'] = 'None'


MACHINE_EPSILON = np.finfo(np.float64).eps


def _joint_probabilities(distances, desired_perplexity):
    cpt_P = _utils.binary_search_perplexity(
        squareform(distances), desired_perplexity)
    P = cpt_P + cpt_P.T
    sum_P = max(np.sum(P), MACHINE_EPSILON)
    P = np.maximum(squareform(P) / sum_P, MACHINE_EPSILON)
    return P


def _kl_divergence_loss(Y, P):
    num_samples = Y.shape[0]

    t = pdist(Y, metric='sqeuclidean')
    t += 1.0
    t **= -1.0
    Q = np.maximum(t / (2.0 * np.sum(t)), MACHINE_EPSILON)

    loss = np.dot(P, np.log(P / Q))

    grad = np.empty_like(Y)
    P_Q_t = squareform((P - Q) * t)
    for i in range(num_samples):
        grad[i] = np.dot(P_Q_t[i], Y[i] - Y)
    grad *= 4.0

    return loss, grad


def optimize(param, options, verbose=False):
    loss_func = options['loss']
    history = options['history']
    momentum = options.get('momentum', 0.5)
    learning_rate = options.get('learning_rate', 100)
    num_iters = options.get('num_iters', 100)
    description = options.get('desc', 'Optimizing')
    optimized = param.copy()

    update = np.zeros_like(param)
    gains = np.ones_like(param)

    for it in range(num_iters):
        history.append(optimized.copy())
        loss, grad = loss_func(optimized)

        inc = update * grad >= 0
        dec = np.invert(inc)
        gains[inc] += 0.05
        gains[dec] *= 0.95
        np.clip(gains, 0.01, np.inf)
        update = momentum * update - learning_rate * gains * grad
        optimized += update

    return optimized


class TSNE:

    def __init__(self, num_components=2, early_exaggeration=4.0,
                 perplexity=30.0, num_iters=100,
                 learning_rate=1000.0, random_state=0,
                 num_exaggeration_iters=50):
        self.num_iters = num_iters
        self.num_exaggeration_iters = num_exaggeration_iters
        self.learning_rate = learning_rate
        self.early_exaggeration = early_exaggeration
        self.perplexity = perplexity
        self.random_state = np.random.RandomState(random_state)
        self.num_components = num_components

    def fit_transform(self, X):
        N = X.shape[0]
        options = {}
        options['learning_rate'] = self.learning_rate
        options['loss'] = lambda param: _kl_divergence_loss(param, P)
        options['history'] = []

        distances = pdist(X, metric='sqeuclidean')
        P = _joint_probabilities(distances, self.perplexity)
        Y = self.random_state.randn(N, self.num_components) * 1e-4

        # with early exaggeration
        P *= self.early_exaggeration
        options['momentum'] = 0.5
        options['num_iters'] = self.num_exaggeration_iters
        options['desc'] = 'With exaggeration'
        Y = optimize(Y, options)

        # without early exaggeration
        P /= self.early_exaggeration
        options['momentum'] = 0.8
        options['num_iters'] = self.num_iters - self.num_exaggeration_iters
        options['desc'] = 'Without exaggeration'
        Y = optimize(Y, options)

        return Y, options['history']


def numerical_gradient(f, param, dx=1e-4):
    num_grad = np.empty_like(param)
    it = np.nditer(Y, flags=['multi_index'], op_flags=['readwrite'])
    total_steps = np.prod(param.shape)
    with tqdm(total=total_steps) as pbar:
        pbar.set_description(' [Numerical gradient]')
        while not it.finished:
            ix = it.multi_index
            oldvalue = param[ix]
            param[ix] = oldvalue - dx
            loss_neg = f(param)
            param[ix] = oldvalue + dx
            loss_plus = f(param)
            param[ix] = oldvalue
            num_grad[ix] = (loss_plus - loss_neg)/(2*dx)
            it.iternext()
            pbar.update(1)
    return num_grad


def check_gradient(Y, P, grad):
    def loss(y):
        t = pdist(y, metric='sqeuclidean')
        t += 1.0
        t **= -1.0
        Q = np.maximum(t / (2.0 * np.sum(t)), MACHINE_EPSILON)
        return 2.0 * np.dot(P, np.log(P / Q))

    num_grad = numerical_gradient(loss, Y.copy())

    dist = np.linalg.norm(num_grad - grad, ord='fro')
    if dist < 1e-4:
        print('Analytical gradient is OK')
    else:
        print('Analytical gradient is wrong')


def scatter_dataset(points, labels):
    f = plt.figure()
    colors = np.array(sns.hls_palette(10, l=0.3, s=0.8))
    plt.xlim(-25.0, 25.0)
    plt.ylim(-25.0, 25.0)
    sc = plt.scatter(*zip(*points), c=colors[labels])
    return f, sc


def make_movie(history, labels, out):
    f, sc = scatter_dataset(history[0], labels)

    def make_frame(t):
        i = int(t * 50)
        sc.set_offsets(history[i])
        return mplfig_to_npimage(f)

    animation = mpy.VideoClip(make_frame,
                              duration=len(history)/50)
    animation.write_gif(out, fps=24)


dataset = load_digits()
X, y = dataset['data'], dataset['target']

tsne = TSNE(num_iters=100, early_exaggeration=10.0)
X_embedded, history = tsne.fit_transform(X)
make_movie(history, y, 'anim.gif')
