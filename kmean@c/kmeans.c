#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "vector.h"
#include "vector.c"

#define MAX_ROW_LEN 512

// Define bool type if not already defined
#ifndef bool
typedef enum { false, true } bool;
#endif

// Function declarations
bool testValidation(int k, int n, int d, int max_iter);
Vector* convertToVectors(const char* filename, int n, int d);
Vector* initializeCentroids(Vector *vectorList, int k);
void computeMinDistance(Vector *vectorList, Vector *centroids_list, int k, int n);
Vector* updateCentroids(Vector *vectorList, int n, int d, int k);
bool isConverged(Vector *centroidsList, Vector *updatedCentroidsList, int k);

// This function validates the input parameters for the k-means algorithm.
bool testValidation(int k, int n, int d, int max_iter) {
    // Validate number of clusters
    if (!(1 < k && k < n)) {
        printf("Invalid number of clusters!\n");
        return false;
    }

    // Validate number of points
    if (n < 1) {
        printf("Invalid number of points!\n");
        return false;
    }

    // Validate data dimension
    if (d < 1) {
        printf("Invalid dimension of point!\n");
        return false;
    }

    // Validate maximum iteration
    if (!(1 < max_iter && max_iter < 1000)) {
        printf("Invalid maximum iteration!\n");
        return false;
    }

    // All validations passed, return true
    return true;
}


// This function reads data from a file, tokenizes each line, and stores the values as components of Vector objects.
Vector* convertToVectors(const char* filename, int n, int d) {
    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for storing pointers to Vector objects
    Vector *vectorsList = (Vector *)malloc(n * sizeof(Vector));
    if (vectorsList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Read data from file line by line
    char line[MAX_ROW_LEN];
    int i = 0;

    while (fgets(line, sizeof(line), file)) {
        // Tokenize the line
        char *token = strtok(line, ",");
        int dimension = 0;
        double *components = (double *)malloc(d * sizeof(double)); 

        if (components == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        // Parse each token and store it in the components array
        while (token != NULL) {
            components[dimension++] = atof(token);
            token = strtok(NULL, ",");
        }

        // Create a Vector object and store it in the array
        vectorsList[i].dimension = dimension;
        vectorsList[i].components = components;
        vectorsList[i].centroid = 0;
        i++;
    }

    // Close the file
    fclose(file);

    return vectorsList;
}


// This function initializes centroids using the first k vectors from the input vector list.
Vector* initializeCentroids(Vector *vectorList, int k) {
    // Allocate memory for the centroid list
    Vector *centroidList = (Vector *)malloc(k * sizeof(Vector));
    if (centroidList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Copy the first k vectors from the vector list to the centroid list
    for (int i = 0; i < k; i++) {
        centroidList[i].dimension = vectorList[i].dimension;
        centroidList[i].centroid = i;
        
        // Allocate memory for components of the centroid
        centroidList[i].components = (double *)malloc(vectorList[i].dimension * sizeof(double));
        if (centroidList[i].components == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        
        // Copy components from the corresponding vector
        memcpy(centroidList[i].components, vectorList[i].components, vectorList[i].dimension * sizeof(double));
    }

    return centroidList;
}

// This function iterates through each vector in the vector list,
// and computes the Euclidean distance between that vector and each centroid.
void computeMinDistance(Vector *vectorList, Vector *centroidsList, int k, int n) {
    // Iterate through all vectors
    for (int i = 0; i < n; i++) {
        double minDistance = INFINITY; // Initialize min distance to infinity
        int closestCentroidIndex = -1; // Initialize closest centroid index

        // Compute the distance from the current vector to each centroid
        for (int j = 0; j < k; j++) {
            double distance = euclidean_distance(vectorList[i], centroidsList[j]); // Compute Euclidean distance
            // Update min distance and closest centroid index if a closer centroid is found
            if (distance < minDistance) {
                minDistance = distance;
                closestCentroidIndex = j;
            }
        }

        // Assign the closest centroid index to the vector
        vectorList[i].centroid = closestCentroidIndex;
    }
}

// This function updates the centroids based on the assigned clusters.
//It calculates the mean of each cluster by summing all vectors' coordinates assigned to that cluster,
// and dividing by the number of vectors in that cluster.
Vector* updateCentroids(Vector *vectorList, int n, int d, int k) {
    // Allocate memory for the updated centroids list
    Vector *updatedCentroidsList = (Vector *)malloc(k * sizeof(Vector));
    if (updatedCentroidsList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for keeping track of the number of vectors in each cluster
    double *kNumbers = (double *)calloc(k, sizeof(double));
    if (kNumbers == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize mean centroids and kNumbers
    for (int i = 0; i < k; i++) {
        updatedCentroidsList[i] = createVector(d, NULL); // Create an empty vector
    }

    // Sum all vectors' coordinates
    for (int i = 0; i < n; i++) {
        Vector currentVector = vectorList[i];
        int centroidIndex = currentVector.centroid;

        // Add the current vector to the corresponding centroid
        updatedCentroidsList[centroidIndex] = add(currentVector, updatedCentroidsList[centroidIndex]);
        kNumbers[centroidIndex]++;
    }

    // Calculate the mean of each cluster
    for (int i = 0; i < k; i++) {
        if (kNumbers[i] != 0) {
            updatedCentroidsList[i] = multiplyScalar(updatedCentroidsList[i], 1.0f / kNumbers[i]);
        }
    }

    // Free memory allocated for kNumbers
    free(kNumbers);

    return updatedCentroidsList;
}


// Function to check if the cluster mean point is converged
bool isConverged(Vector *centroidsList, Vector *updatedCentroidsList, int k) {
    for (int index = 0; index < k; index++) {
        // Compare the components of each centroid
        for (int i = 0; i < centroidsList[index].dimension; i++) {
            if (centroidsList[index].components[i] != updatedCentroidsList[index].components[i]) {
                return false; // If any component is different, return false
            }
        }
    }
    return true; // All centroids are converged
}

// Function to perform K-means clustering
void Kmeans(const char *filename, int k, int n, int d, int maxIter) {
    // Set default value for max_iter if not provided
    if (maxIter <= 0)
        maxIter = 200;

    // Validate input parameters
    bool isValid = testValidation(k, n, d, maxIter);
    if (!isValid) {
        fprintf(stderr, "Input parameters validation failed\n");
        exit(EXIT_FAILURE);
    }

    // Read vectors from file
    Vector *vectorList = convertToVectors(filename, n, d);

    // Initialize centroids
    Vector *centroidList = initializeCentroids(vectorList, k);

   

    // Perform K-means iterations
    for (int iter = 0; iter < maxIter; iter++) {
        printf("iter %d\n", iter);

        // Assign vectors to the nearest centroids
        computeMinDistance(vectorList, centroidList, k, n);

        // Update centroids based on the assigned clusters
        Vector *updatedCentroidsList = updateCentroids(vectorList, n, d, k);

        // Check for convergence
        if (isConverged(centroidList, updatedCentroidsList, k)) {
            free(updatedCentroidsList);
            break;
        }

        // Free memory for the previous centroid list
        free(centroidList);

        // Update the centroid list
        centroidList = updatedCentroidsList;
    }

    // Print final centroids
    for (int i = 0; i < k; i++) {
        printf("Centroid %d (", i + 1);
        printVector(centroidList[i]);
    }  
    printf("\n");

    // Free memory allocated for vectors and centroids
    for (int i = 0; i < n; i++) {
        free(vectorList[i].components);
    }
    free(vectorList);
    free(centroidList);
}

// Main function; Perform K-means clustering
int main() {
    // ### from inputs
    // Kmeans("input_1.txt", 3, 800, 3, 600);
    // Kmeans("input_2.txt", 7, 430, 11, 0);
    // Kmeans("input_3.txt", 15, 5000, 5, 300);

    char filename[100];
    int k, n, d, maxIter;

    // Prompt user to enter inputs
    printf("Enter filename: ");
    scanf("%s", filename);

    printf("Enter value of k: ");
    scanf("%d", &k);

    printf("Enter value of n: ");
    scanf("%d", &n);

    printf("Enter value of d: ");
    scanf("%d", &d);

    printf("Enter value of maxIter: ");
    scanf("%d", &maxIter);

    // Call Kmeans function with user-provided inputs
    Kmeans(filename, k, n, d, maxIter);
    
    return 0;
}
