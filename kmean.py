from Vector import Vector
import random


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

    centroids_list = initialize_centroids(vector_list=vector_list, k=k)

    for iter in range(max_iter):

        for vector in vector_list:
            compute_min_distance(vector=vector, centroids_list=centroids_list)

        updated_centroids_list = update_centroids(vector_list=vector_list, centroids_list=centroids_list, d=d, k=k)


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


def initialize_centroids(vector_list: list, k: int):
    """
    randomly choose vectors to initialize centroids.
    :param vector_list: vector list of Vector Class
    :param k: number of clustering
    :return: random sample data vectors from a vector list
    """
    return random.sample(vector_list, k)


def compute_min_distance(vector: Vector, centroids_list: list):
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


def update_centroids(vector_list: list, d: int, k: int):
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


def main():
    with open('input_1.txt', 'r') as file:
        k_mean(k=3, n=800, d=3, input_data=file, max_iter=600)


if __name__ == '__main__':
    main()
