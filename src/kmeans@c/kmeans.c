#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "vector.h"
#include "vector.c"
#define INF 999999999999
#define MAX_ROW_LEN 512

#ifndef bool
typedef enum { false, true } bool;
#endif

bool testValidation(int k, int n, int d, int max_iter);
Vector* convertToVectors(const char* filename, int n, int d);
Vector* initializeCentroids(Vector *vectorList, int k);
void computeMinDistance(Vector *vectorList, Vector *centroids_list, int k, int n);
Vector* updateCentroids(Vector *vectorList, int n, int d, int k);
bool isConverged(Vector *centroidsList, Vector *updatedCentroidsList, int k);

bool testValidation(int k, int n, int d, int max_iter) {
    if (!(1 < k && k < n)) {
        printf("Invalid number of clusters!\n");
        return false;
    }

    if (n < 1) {
        printf("Invalid number of points!\n");
        return false;
    }

    if (d < 1) {
        printf("Invalid dimension of point!\n");
        return false;
    }

    if (!(1 < max_iter && max_iter < 1000)) {
        printf("Invalid maximum iteration!\n");
        return false;
    }

    return true;
}

Vector* convertToVectors(const char* filename, int n, int d) {

    Vector *vectorsList;
    char line[MAX_ROW_LEN];
    int i = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    vectorsList = malloc(n * sizeof(Vector));

    if (vectorsList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        int dimension = 0;
        double *components = malloc(d * sizeof(double)); 
        if (components == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        while (token != NULL) {
            components[dimension++] = atof(token);
            token = strtok(NULL, ",");
        }

        vectorsList[i].dimension = dimension;
        vectorsList[i].components = components;
        vectorsList[i].centroid = 0;
        i++;
    }

    fclose(file);

    return vectorsList;
}

Vector* initializeCentroids(Vector *vectorList, int k) {

    int i;
    Vector *centroidList;
    centroidList = (Vector *)malloc(k * sizeof(Vector));

    if (centroidList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < k; i++) {
        centroidList[i].dimension = vectorList[i].dimension;
        centroidList[i].centroid = i;

        centroidList[i].components = (double *)malloc(vectorList[i].dimension * sizeof(double));
        if (centroidList[i].components == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        memcpy(centroidList[i].components, vectorList[i].components, vectorList[i].dimension * sizeof(double));
    }

    return centroidList;
}

void computeMinDistance(Vector *vectorList, Vector *centroidsList, int k, int n) {
    int i, j = 0;
    int closestCentroidIndex = -1;
    double minDistance = 100000000000000;

    for (i = 0; i < n; i++) {
        minDistance = INF;
        closestCentroidIndex = -1; 

        for (j = 0; j < k; j++) {
            double distance = euclidean_distance(vectorList[i], centroidsList[j]); 
            if (distance < minDistance) {
                minDistance = distance;
                closestCentroidIndex = j;
            }
        }
        vectorList[i].centroid = closestCentroidIndex;
    }
}

Vector* updateCentroids(Vector *vectorList, int n, int d, int k) {
    int i, j, l, centroidIndex;
    double *kNumbers;
    Vector currentVector;
    Vector *updatedCentroidsList;

    kNumbers = calloc(k, sizeof(double));
    updatedCentroidsList = (Vector *)malloc(k * sizeof(Vector));

    if (updatedCentroidsList == NULL || kNumbers == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < k; i++) {
        updatedCentroidsList[i] = createVector(d, NULL); 
    }

    for (j = 0; j < n; j++) {
        currentVector = vectorList[j];
        centroidIndex = currentVector.centroid;

        updatedCentroidsList[centroidIndex] = add(currentVector, updatedCentroidsList[centroidIndex]);
        kNumbers[centroidIndex]++;
    }

    for (l = 0; l < k; l++) {
        if (kNumbers[l] != 0) {
            updatedCentroidsList[l] = multiplyScalar(updatedCentroidsList[l], 1.0f / kNumbers[l]);
        }
    }

    free(kNumbers);

    return updatedCentroidsList;
}

bool isConverged(Vector *centroidsList, Vector *updatedCentroidsList, int k) {
    int index, i;
    for (index = 0; index < k; index++) {
        for (i = 0; i < centroidsList[index].dimension; i++) {
            if (centroidsList[index].components[i] != updatedCentroidsList[index].components[i]) {
                return false;
            }
        }
    }
    return true; 
}

void Kmeans(const char *filename, int k, int n, int d, int maxIter) {

    Vector *vectorList;
    Vector *centroidList;
    bool isValid;
    int iter, i, j;
    Vector *updatedCentroidsList;

    if (maxIter <= 0)
        maxIter = 200;

    isValid = testValidation(k, n, d, maxIter);

    if (!isValid) {
        fprintf(stderr, "Input parameters validation failed\n");
        exit(EXIT_FAILURE);
    }

    vectorList = convertToVectors(filename, n, d);
    centroidList = initializeCentroids(vectorList, k);

    for (iter = 0; iter < maxIter; iter++) {
        computeMinDistance(vectorList, centroidList, k, n); 
        updatedCentroidsList = updateCentroids(vectorList, n, d, k);

        if (isConverged(centroidList, updatedCentroidsList, k)) {
            free(updatedCentroidsList);
            break;
        }

        free(centroidList);
        centroidList = updatedCentroidsList;
    }

    for (i = 0; i < k; i++) {
        printVector(centroidList[i]);
    }  
    printf("\n");

    for (j = 0; j < n; j++) {
        free(vectorList[j].components);
    }

    free(vectorList);
    free(centroidList);
}

int main(int argc, char *argv[]) {
    int k, n, d, max_iter = -1;
    char *input_data;

    if (argc < 5) {
        printf("Usage: ./executable k n d [iter] input_data\n");
        return 1;
    }

    k = atoi(argv[1]);
    n = atoi(argv[2]);
    d = atoi(argv[3]);

    if (argc > 5) {
        max_iter = atoi(argv[4]);
        input_data = argv[5];
    } else {
        input_data = argv[4];
    }

    Kmeans(input_data, k, n, d, max_iter);

    return 0;
}
