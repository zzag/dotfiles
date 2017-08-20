import numpy as np
import sys
from pylab import imsave
from numba import autojit


@autojit
def print_progress(progress):
    sys.stdout.write("\rGenerating mandelbrot set... \t%.3f%%" % progress)
    sys.stdout.flush()


@autojit
def mandelbrot_func(x, y, max_iters):
    c = complex(x, y)
    z = 0.0j
    s = 0

    for i in range(max_iters):
        z = z * z + c
        s = z.real ** 2 + z.imag ** 2

        if s >= 4:
            return i, s

    return max_iters, s


@autojit
def mandelbrot(w, h, max_iters):
    r = np.zeros((w, h), dtype=np.uint8)
    g = np.zeros((w, h), dtype=np.uint8)
    b = np.zeros((w, h), dtype=np.uint8)
    min_x = -2.5
    max_x = 1.0
    min_y = -2.0
    max_y = 2.0

    delta_x = (max_x - min_x) / w
    delta_y = (max_y - min_y) / h

    for x in range(w):
        re = min_x + x * delta_x

        for y in range(h):
            im = min_y + y * delta_y
            iters, zn = mandelbrot_func(re, im, max_iters)

            # write color data
            r[y, x] = min(iters, 255)
            g[y, x] = min(iters, 255)
            b[y, x] = min(iters, 255)

        # print progress
        print_progress((x + 1) / w * 100)

    # merge r, g and b
    print("\nGenerating image... ", end="")
    sys.stdout.flush()
    image = np.dstack((r, g, b))
    print("done")

    return image


if __name__ == '__main__':
    image = mandelbrot(200, 200, 150)
    print("Saving image... ", end="")
    sys.stdout.flush()
    imsave("mandelbrot.png", image)
    print("done")
