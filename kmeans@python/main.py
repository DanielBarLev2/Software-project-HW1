from kmeans import kmeans


def main():
    # from inputs
    # kmeans("input_1.txt", 3, 800, 3, 600)
    # kmeans("input_2.txt", 7, 430, 11)
    # kmeans("input_3.txt", 15, 5000, 5, 300)

    input_data = input("Enter data path")
    k = int(input("Enter number of centroids"))
    n = int(input("Enter number of data vectors"))
    d = int(input("Enter number of dimension"))
    max_iter = int(input("Enter max iter"))

    kmeans(input_data=input_data, k=k, n=n, d=d, max_iter=max_iter)


if __name__ == "__main__":
    main()