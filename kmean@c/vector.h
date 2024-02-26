#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    int dimension;      // Dimension of the vector
    double *components;  // Array of components
    int centroid;       // Index of the centroid (-1 if not assigned)
} Vector;

// Function prototypes
Vector createVector(int dimension, double *values);
Vector add(Vector vec1, Vector vec2);
Vector multiplyScalar(Vector vec, double scalar);
double euclidean_distance(Vector vec1, Vector vec2);
void printVector(Vector vec);

#endif /* VECTOR_H */
