class MSECriterion:

    def __init__(self):
        self.error_signal = None

    def initialize_error_signal(self, target, actual):
        self.error_signal = -2*(actual - target) * actual * (1 - actual)
        return self

    def get_cost(self, target, actual):
        cost = ((actual - target)**2).sum()/target.shape[1]
        return cost
