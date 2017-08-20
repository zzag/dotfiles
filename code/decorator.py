class decorator:
    def __init__(self, function):
        self.function = function
    
    def __call__(self, *args, **n_args):
        print(args)
        print(n_args)
        return self.function(*args, **n_args)


@decorator
def f(a, **args):
    print("fsfasa", a, args['b'])


f("gigity", b="bbbbbb")
