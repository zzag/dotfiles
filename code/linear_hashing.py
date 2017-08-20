class LinearMap:

    def __init__(self, load_factor=0.7, per_bucket=2):
        self.level = 1
        self.pointer = 0
        self.buckets = [{} for i in range(2 ** self.level)]
        self.load_factor = load_factor
        self.per_bucket = per_bucket

    def get(self, key):
        bucket = self._get_bucket(key)
        return bucket[key]

    def _get_bucket(self, key):
        h = hash(key)
        i = h % pow(2, self.level)
        if i < self.pointer:
            i = h % pow(2, self.level + 1)
        return self.buckets[i]

    def put(self, key, value):
        bucket = self._get_bucket(key)
        bucket[key] = value

        num_items = sum(len(bucket) for bucket in self.buckets)
        max_capacity = self.per_bucket * len(self.buckets)
        curr_load_factor = num_items / max_capacity

        if curr_load_factor > self.load_factor:
            self._split()

    def _split(self):
        """Split bucket at `pointer` position"""
        split_idx = self.pointer
        split_bucket = self.buckets[split_idx]
        old_bucket = dict()
        new_bucket = dict()

        self.buckets[split_idx] = old_bucket
        self.buckets.append(new_bucket)

        for key, value in split_bucket.items():
            i = hash(key) % pow(2, self.level + 1)
            b = old_bucket if i == split_idx else new_bucket
            b[key] = value

        # move the pointer forward
        # if it hits 2**L then reset it to 0
        self.pointer += 1
        pointer_bound = 2 ** self.level
        if self.pointer >= pointer_bound:
            self.pointer = 0
            self.level += 1

    def remove(self, key):
        """Dummy deletion from the hash table
        
        TODO: maybe, it is a good idea to shrink table,
        when the load factor is too small.
        """
        bucket = self._get_bucket(key)
        del bucket[key]

    def __getitem__(self, key):
        return self.get(key)

    def __setitem__(self, key, value):
        self.put(key, value)

    def items(self):
        for bucket in self.buckets:
            yield from bucket.items()
