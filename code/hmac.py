import hashlib


BLOCK_SIZE = 64
IPAD = 0x36
OPAD = 0x5c


def hmac(key, message, digestmod=hashlib.sha1):
    if len(key) > BLOCK_SIZE:
        key = digestmod(key).digest()
    if len(key) < BLOCK_SIZE:
        key = key.ljust(BLOCK_SIZE, b'\0')

    inner_key = bytes((x ^ IPAD) for x in key)
    outer_key = bytes((x ^ OPAD) for x in key)
    return digestmod(outer_key + digestmod(inner_key + message).digest())


print(hmac(b'foo', b'bar', digestmod=hashlib.sha256).hexdigest())
