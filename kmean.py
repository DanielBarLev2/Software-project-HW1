from Vector import Vector


def k_mean(k: int, n: int, d: int, input_data, max_iter=200):
    """
    clustering data into K groups based on similarities between data points.
    using Euclidean distance.
    :param k: number of clustering
    :param n: number of data points
    :param d: data dimension
    :param input_data: text file containing the data; assuming valid
    :param max_iter: Max iterations of optimization
    """

    test_validation(k=k, n=n, d=d, max_iter=max_iter)

    vector_list = convert_to_vectors(input_data=input_data)


def test_validation(k: int, n: int, d: int, max_iter: int):
    """
    validates the input params according to the requirements
    :param k: number of clustering
    :param n: number of data points
    :param d: data dimension
    :param max_iter: Max iterations of optimization
    """
    if not (1 < k < n):
        raise Exception("Invalid number of clusters!")

    if 1 > n:
        raise Exception("Invalid number of points!")

    if not (type(d) is int):
        raise Exception("Invalid dimension of point!")

    if not ((1 < max_iter < 1000) or (type(d) is int)):
        raise Exception("Invalid maximum iteration!")


def convert_to_vectors(input_data):
    """
    convert rows from a text file, line by line to a Vector Class instances.
    :param input_data: text file containing the data; assuming a valid
    :return: vector list of Vector Class
    """
    vectors = []

    for line in input_data:
        components = [float(x) for x in line.strip().split(',')]
        vector = Vector(*components)
        vectors.append(vector)

    return vectors


def main():
    with open('input_1.txt', 'r') as file:
        k_mean(k=3, n=800, d=3, input_data=file, max_iter=600)


if __name__ == '__main__':
    main()
