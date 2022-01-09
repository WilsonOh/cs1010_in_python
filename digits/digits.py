def initialise_objects(cls):
    n = int(input())
    obj_list = [cls() for _ in range(n)]
    for digit in obj_list:
        digit.read_digits()
    return obj_list


def get_diff(digit1, digit2) -> int:
    diff_count = 0
    for row1, row2 in zip(digit1.ar, digit2.ar):
        for string1, string2 in zip(row1, row2):
            for char1, char2 in zip(string1, string2):
                if char1 != char2:
                    diff_count += 1
    return diff_count

def find_k_nearest(training_samples, k):
    k_nearest = sorted(training_samples, key=lambda x: x.distance)[:k]
    return [digit.label for digit in k_nearest]




