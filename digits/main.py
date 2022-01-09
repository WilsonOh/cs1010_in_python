from classes import Digit, Neighbour
import digits
from collections import Counter

K = 5

def main():
    """Creates a list of test samples and training samples by reading from stdin"""
    training_samples = digits.initialise_objects(Neighbour)
    test_samples = digits.initialise_objects(Digit)

    matched = 0
    for test_sample in test_samples:
        for training_sample in training_samples:
            training_sample.distance = digits.get_diff(test_sample, training_sample)  # gets the 'distance' between training sample and test sample and stores it in neighbour.distance

        most_common = Counter(digits.find_k_nearest(training_samples, K)).most_common(1)[0][0]
        if most_common == test_sample.label:
            matched += 1
        print(test_sample.label, most_common)
    print(f"{matched/len(test_samples) * 100:.4f}")  # prints percentage of samples matched to 4 decimal points


if __name__ == "__main__":
    main()

