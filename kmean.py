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

    # select the first k elements and assign them as centroids
    vector_list = input_data[:k]

    centroids_list = initialize_centroids(vector_list=vector_list, k=k)

    # mainloop
    for iter in range(max_iter):

        for vector in vector_list:
            compute_min_distance(vector=vector, centroids_list=centroids_list)

        updated_centroids_list = update_centroids(vector_list=vector_list, d=d, k=k)

        if is_converged(centroids_list=centroids_list, updated_centroids_list=updated_centroids_list):
            break
        centroids_list = updated_centroids_list

    # print the centroid points
    for index in range(len(centroids_list)):
        print(centroids_list[index])


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


def convert_to_vectors(input_data) -> list:
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


def initialize_centroids(vector_list: list, k: int) -> list:
    """
    randomly choose vectors to initialize centroids.
    :param vector_list: vector list of Vector Class
    :param k: number of clustering
    :return: random sample data vectors from a vector list
    """
    return vector_list[:k]


def compute_min_distance(vector: Vector, centroids_list: list) -> float:
    """
    computes the distances to all centroids.
    :param vector: vector
    :param centroids_list: centroids list
    :return: the closest centroid number
    """
    min_distance = float('inf')

    for index, centroid in enumerate(centroids_list):
        distance = vector.euclidean_distance(centroid)

        if distance < min_distance:
            min_distance = distance
            vector.centroid = index

    return min_distance


def update_centroids(vector_list: list, d: int, k: int) -> list:
    """
    compute the mean of each vector cluster
    :param vector_list: vector list of Vector Class
    :param d: data dimension
    :param k: number of a clustering
    :return: new centroid vector list according to the mean of each cluster data point
    """
    updated_centroids_list = []
    k_numbers = [0] * k

    # init mean centroids
    for vector in range(k):
        updated_centroids_list.append(Vector(*([0] * d)))

    # sum all vectors coordinates
    for vector in vector_list:
        updated_centroids_list[vector.centroid] += vector
        k_numbers[vector.centroid] += 1

    # calculate mean
    for index in range(k):
        updated_centroids_list[index] = round(updated_centroids_list[index] * (1 / k_numbers[index]), 4)

    return updated_centroids_list


def is_converged(centroids_list: list, updated_centroids_list: list) -> bool:
    """
    check if the cluster mean point is converged.
    :param centroids_list: centroids list
    :param updated_centroids_list: new centroids list
    :return: true if the lists are equal.
    """
    for index in range(len(centroids_list)):

        if not centroids_list[index].__eq__(updated_centroids_list[index]):
            return False
    return True
