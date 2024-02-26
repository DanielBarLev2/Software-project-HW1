#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h" // Include the Vector header file


// Function to create a new Vector
Vector createVector(int dimension, float *values) {
    Vector vec;
    vec.dimension = dimension;
    vec.centroid = -1; // Initialize centroid to -1 (indicating not assigned)
    
    if (values == NULL)
        vec.components = (float *)calloc(dimension * sizeof(float), 0.0000f);
    else{
        vec.components = (float *)malloc(dimension * sizeof(float));
        for (int i = 0; i < dimension; i++) {
            vec.components[i] = values[i];
        }
    }
    return vec;
}

// Function to free memory allocated for a Vector
void freeVector(Vector *vec) {
    free(vec->components);
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

// Function to perform vector subtraction
Vector subtract(Vector vec1, Vector vec2) {
    if (vec1.dimension != vec2.dimension) {
        fprintf(stderr, "Vectors must have the same dimension for subtraction\n");
        exit(EXIT_FAILURE);
    }
    
    Vector result = createVector(vec1.dimension, NULL);
    for (int i = 0; i < vec1.dimension; i++) {
        result.components[i] = vec1.components[i] - vec2.components[i];
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

// Function to raise each component of a vector to a given power
Vector power(Vector vec, double exponent) {
    Vector result = createVector(vec.dimension, NULL);
    for (int i = 0; i < vec.dimension; i++) {
        result.components[i] = pow(vec.components[i], exponent);
    }
    return result;
}

// Function to round each component of a vector to a given number of decimal places
Vector roundVector(Vector vec, int n) {
    Vector result = createVector(vec.dimension, NULL);
    for (int i = 0; i < vec.dimension; i++) {
        result.components[i] = round(vec.components[i] * pow(10, n)) / pow(10, n);
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

// Function to calculate the sum of components of a vector
float sum(Vector vec) {
    float sum = 0.0;
    for (int i = 0; i < vec.dimension; i++) {
        sum += vec.components[i];
    }
    return sum;
}
