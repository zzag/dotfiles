#!/usr/bin/python3


def sieve(max_prime):
    primes = [True for x in range(max_prime)]
    primes[0] = False
    primes[1] = False
    for n in range(2, int(max_prime ** 0.5) + 1):
        if primes[n] is True:
            for m in range(n * n, max_prime, n):
                if primes[m] is True and m % n == 0:
                    primes[m] = False

    primes = list(
        map(
            lambda x: x[0],
            filter(lambda x: x[1] is True, enumerate(primes)))
    )

    return primes


if __name__ == '__main__':
    print(sieve(400))
