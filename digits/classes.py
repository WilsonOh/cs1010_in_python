class Digit:
    def __init__(self):
        self.ar = None
        self.label = None

    def read_digits(self) -> None:
        self.label = input()
        self.ar = [[input()] for _ in range(28)]

    def print_digits(self) -> None:
        """For debugging purposes"""
        if self.ar is not None:
            for row in self.ar:
                print(row)


class Neighbour(Digit):
    def __init__(self):
        super().__init__()
        self.distance = 0


