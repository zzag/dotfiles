def least_bits(i, n):
    """Get n least significant bits"""
    mask = (1 << n) - 1
    return i & mask


def test_bit(i, n):
    """Returns True if nth bit is set"""
    return (i >> n) & 1 == 1


class Bucket:

    def __init__(self, depth=1):
        self.data = {}
        self.depth = depth

    def get(self, key):
        return self.data[key]

    def put(self, key, value):
        self.data[key] = value

    def remove(self, key):
        del self.data[key]

    def __len__(self):
        return len(self.data)


class ExtendibleMap:

    def __init__(self):
        self.global_depth = 1
        self.max_capacity = 2
        self.size = 0
        num_buckets = pow(2, self.global_depth)
        self.dictionary = [Bucket() for i in range(num_buckets)]

    def get(self, key):
        bucket = self._get_bucket(key)
        return bucket.get(key)

    def _get_bucket(self, key):
        i = least_bits(hash(key), self.global_depth)
        bucket = self.dictionary[i]
        return bucket

    def put(self, key, value):
        bucket = self._get_bucket(key)
        if key not in bucket.data:
            self.size += 1
        bucket.put(key, value)

        if len(bucket) > self.max_capacity:
            self._split(bucket)

    def _split(self, bucket):
        """Split overflowed bucket and maybe double hash table"""

        # when local depth of a bucket is equal to
        # global depth of dictionary double the hash table
        if bucket.depth == self.global_depth:
            self.dictionary *= 2
            self.global_depth += 1

        bucket_a = Bucket(depth=bucket.depth + 1)
        bucket_b = Bucket(depth=bucket.depth + 1)

        for key, value in bucket.data.items():
            i = least_bits(hash(key), self.global_depth)
            dest_bucket = bucket_b if test_bit(i, bucket.depth) else bucket_a
            dest_bucket.put(key, value)

        # update pointers to old bucket
        for i, other_bucket in enumerate(self.dictionary):
            if other_bucket != bucket:
                continue
            new_bucket = bucket_b if test_bit(i, bucket.depth) else bucket_a
            self.dictionary[i] = new_bucket

    def remove(self, key):
        """Delete given key from the hash table"""
        bucket = self._get_bucket(key)
        bucket.remove(key)
        self.size -= 1

    def __getitem__(self, key):
        return self.get(key)

    def __setitem__(self, key, value):
        self.put(key, value)

    def __len__(self):
        return self.size
