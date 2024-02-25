#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float *components;
    int dimension;
    int centroid;
} Vector;

// Function declarations
Vector createVector(int dimension, float *values);
void freeVector(Vector *vec);
void printVector(Vector vec);
Vector add(Vector vec1, Vector vec2);
Vector subtract(Vector vec1, Vector vec2);
Vector multiplyScalar(Vector vec, float scalar);
Vector power(Vector vec, double exponent);
Vector roundVector(Vector vec, int n);
float sum(Vector vec);
float euclidean_distance(Vector vec1, Vector vec2);

#endif /* VECTOR_H */
