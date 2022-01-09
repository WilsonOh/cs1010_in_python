from fill import fill_image, initialise_image
import copy


def main():
    image = initialise_image()
    for fill_op in image.fill_ops:
        fill_image(fill_op, image, copy.deepcopy(image.image[fill_op.pos[0]][fill_op.pos[1]]))
    image.print_image()


if __name__ == '__main__':
    main()
