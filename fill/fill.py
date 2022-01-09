from classes import Color, Image


def initialise_image() -> Image:
    image = Image()
    image.read_image()
    image.read_fill_op()
    return image


def fill_image(fill: Color, image: Image, initial_color: Color) -> None:
    x, y = fill.pos
    if x > 0 and image.image[x - 1][y] == initial_color:  # up, use short-circuiting to make sure we don't access an out-of-bounds index
        image.image[x][y].rgb = fill.rgb
        fill.pos = (x - 1, y)  # moves the fill position accordingly every recursive call
        fill_image(fill, image, initial_color)
    if y < len(image.image[0]) - 1 and image.image[x][y + 1] == initial_color:  # right
        image.image[x][y].rgb = fill.rgb
        fill.pos = (x, y + 1)
        fill_image(fill, image, initial_color)
    if y > 0 and image.image[x][y - 1] == initial_color:  # left
        image.image[x][y].rgb = fill.rgb
        fill.pos = (x, y - 1)
        fill_image(fill, image, initial_color)
    if  x < len(image.image) - 1 and image.image[x + 1][y] == initial_color:  # down
        image.image[x][y].rgb = fill.rgb
        fill.pos = (x + 1, y)
        fill_image(fill, image, initial_color)

    image.image[x][y].rgb = fill.rgb  # if there are no more pixels in the same segment, fill the current pixel and exit

