#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "vector.h" // Include the Vector header file

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

// Function to validate input parameters
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

// Function to read data from data file and store it in [row][col][data, centroid]
Vector* convertToVectors(const char* filename, int n, int d) {
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
        float *components = (float *)malloc(d * sizeof(float)); 

        if (components == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

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

    fclose(file);

    return vectorsList;
}

// Function to get the first k vectors from the vector list
Vector* initializeCentroids(Vector *vectorList, int k) {
    Vector *centroidList = (Vector *)malloc(k * sizeof(Vector));

    for (int i = 0; i < k; i++) {
        centroidList[i].dimension = vectorList[i].dimension;
        centroidList[i].centroid = i;
        centroidList[i].components = (float *)malloc(vectorList[i].dimension * sizeof(float));
    
        memcpy(centroidList[i].components, vectorList[i].components, vectorList[i].dimension * sizeof(float));
    }

    return centroidList;
}

// Function to compute the minimum distance to all centroids
void computeMinDistance(Vector *vectorList, Vector *centroids_list, int k, int n) {
    for (int i = 0; i < n; i++) {
        float min_distance = INFINITY; // Reset min_distance for each vector
        int closestCentroidIndex = -1;

        // Compute the minimum distance for the current vector
        for (int j = 0; j < k; j++) {
            float distance = euclidean_distance(vectorList[i], centroids_list[j]);
            if (distance < min_distance) {
                min_distance = distance;
                closestCentroidIndex = j;
            }
        }
        // Update the centroid field of the vector
        vectorList[i].centroid = closestCentroidIndex;
    }
}

// Function to compute the mean of each vector cluster
Vector* updateCentroids(Vector *vectorList, int n, int d, int k) {

    // Allocate memory for the updated centroids list
    Vector *updatedCentroidsList = (Vector *)malloc(k * sizeof(Vector));
    if (updatedCentroidsList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for keeping track of the number of vectors in each cluster
    float *kNumbers = (float *)calloc(k, sizeof(float));
    if (kNumbers == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize mean centroids and kNumbers
    for (int i = 0; i < k; i++) {
        float *componantes = (float *)calloc(d, sizeof(float));
        updatedCentroidsList[i] = createVector(d, NULL);
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

    return updatedCentroidsList;
}

//
bool isConverged(Vector *centroidsList, Vector *updatedCentroidsList, int k) {
    for (int index = 0; index < k; index++) {
        for (int i = 0; i < centroidsList[index].dimension; i++) {
            if (centroidsList[index].components[i] != updatedCentroidsList[index].components[i]) {
                return false;
            }
        }
    }
    return true;
}

// Function to create a new Vector
Vector createVector(int dimension, float *values) {
    Vector vec;
    vec.dimension = dimension;
    vec.centroid = -1; // Initialize centroid to -1 (indicating not assigned)
    
    if (values == NULL)
        vec.components = (float *)calloc(dimension, sizeof(float));
    else{
        vec.components = (float *)malloc(dimension * sizeof(float));
        for (int i = 0; i < dimension; i++) {
            vec.components[i] = values[i];
        }
    }
    return vec;
}

// Function to perform vector addition
Vector add(Vector vec1, Vector vec2) {
    if (vec1.dimension != vec2.dimension) {
        fprintf(stderr, "Vectors must have the same dimension for addition\n");
        exit(EXIT_FAILURE);
    }
    
    Vector result = createVector(vec1.dimension, NULL);
    for (int i = 0; i < vec1.dimension; i++) {
        result.components[i] = vec1.components[i] + vec2.components[i];
    }
    return result;
}

// Function to perform scalar multiplication of a vector
Vector multiplyScalar(Vector vec, float scalar) {
    Vector result = createVector(vec.dimension, NULL);
    for (int i = 0; i < vec.dimension; i++) {
        result.components[i] = vec.components[i] * scalar;
    }
    return result;
}

// Function to calculate the Euclidean distance between two vectors
float euclidean_distance(Vector vec1, Vector vec2) {
    if (vec1.dimension != vec2.dimension) {
        fprintf(stderr, "Vectors must have the same dimension for calculating Euclidean distance\n");
        exit(EXIT_FAILURE);
    }
    
    float sum = 0.0;
    for (int i = 0; i < vec1.dimension; i++) {
        float diff = vec1.components[i] - vec2.components[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

// Function to print a Vector
void printVector(Vector vec) {
    printf("(");
    for (int i = 0; i < vec.dimension; i++) {
        printf("%.2f", vec.components[i]);
        if (i < vec.dimension - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}


int main() {
    const char *filename = "input_1.txt";
    int  k=3, n=800, d=3, max_iter=600;

    bool isValid = testValidation(k, n, d, max_iter);

    // If the validation succeeds, print a success message
    if (isValid) {
        Vector *vectorList = convertToVectors(filename, n, d);

        Vector *centroidList = initializeCentroids(vectorList, k);
        
        for (int iter = 0; iter < max_iter; iter++) {
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

            // Free the memory for the previous centroid list
            free(centroidList);

            // Update the centroid list
            centroidList = updatedCentroidsList;
        }

        
        // Final print
        for (int i = 0; i < 10; i++) {
            printf("Vector %d (Centroid %d): (", i + 1, vectorList[i].centroid);
            printVector(vectorList[i]);
        }

        for (int i = 0; i < k; i++) {
            printf("Centroid %d (", i + 1);
            printVector(centroidList[i]);
        }

        // Free memory allocated for vectors and centroids
        for (int i = 0; i < n; i++) {
            free(vectorList[i].components);
        }
        free(vectorList);
        free(centroidList);
    }

    
    return 0;
}
