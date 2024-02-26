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
        vec.components = (float *)calloc(dimension, sizeof(float)); // Allocate memory for components
    else {
        vec.components = (float *)malloc(dimension * sizeof(float)); // Allocate memory for components
        if (vec.components == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        // Copy values to components
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
    
    Vector result = createVector(vec1.dimension, NULL); // Create a result vector
    // Perform addition component-wise
    for (int i = 0; i < vec1.dimension; i++) {
        result.components[i] = vec1.components[i] + vec2.components[i];
    }
    return result;
}

// Function to perform scalar multiplication of a vector
Vector multiplyScalar(Vector vec, float scalar) {
    Vector result = createVector(vec.dimension, NULL); // Create a result vector
    // Perform scalar multiplication component-wise
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
    // Calculate the sum of squared differences component-wise
    for (int i = 0; i < vec1.dimension; i++) {
        float diff = vec1.components[i] - vec2.components[i];
        sum += diff * diff;
    }
    return sqrt(sum); // Return the square root of the sum
}

// Function to print a Vector
void printVector(Vector vec) {
    printf("(");
    // Print components
    for (int i = 0; i < vec.dimension; i++) {
        printf("%.4f", vec.components[i]);
        if (i < vec.dimension - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

