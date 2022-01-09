from dataclasses import dataclass


@dataclass
class Color:
    rgb: tuple[int, ...]
    pos: tuple[int, ...]

    def __eq__(self, other) -> bool:
        if not isinstance(other, Color):
            return NotImplemented
        for color1, color2 in zip(self.rgb, other.rgb):
            if color1 != color2:
                return False
        return True


class Image:
    def __init__(self):
        self.image = []
        self.fill_ops = []

    def read_image(self) -> None:
        nrows, _ = input().split()  # get the number of rows of the image
        for row in range(int(nrows)):
            s = tuple(map(int, input().split()))  # get a tuple of integer values from each row of the image
            colors = [s[i: i + 3] for i in range(0, len(s), 3)]  # colors is a list of tuples each containing (r, g, b) values
            self.image.append([Color(color, (idx, row)) for idx, color in enumerate(colors)])  # for each rgb tuple in the colors list, construct a list of Color objects
                                                                                               # using the rgb values + row and idex of each tuple and append the list to
    def read_fill_op(self) -> None:                                                            # the self.image list
        n = input()  # get the number of fill ops to be done
        for _ in range(int(n)):
            s = tuple(map(int, input().split()))  # get a tuple of integer values containing the x, y coords and rgb value of the fill op
            self.fill_ops.append(Color(s[-3:], s[:2]))  # construct a Color object based on those values using slicing and append the objects to self.fill_ops

    def print_image(self):
        print(f"P3 {len(self.image[0])} {len(self.image)} 255")
        for row in self.image:
            for color in row:
                print(*color.rgb, end=' ')  # use the unpacking operator '*' to print out rgb values separately
            print()

