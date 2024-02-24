#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_ROW_LEN 256

// Define bool type if not already defined
#ifndef bool
    typedef enum { false, true } bool;
#endif



// Function to validate input parameters
bool test_validation(int k, int n, int d, int max_iter) {
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


// Function to read data from file and store each row in a separate array
float **readDataFromFile(const char *filename, int n, int d) {
    // Open input data file (txt)
    FILE *dataFile = fopen(filename, "r");

    if (dataFile == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    // Allocate memory for the 2D array dynamically
    float **vectorList = (float **)malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        vectorList[i] = (float *)malloc(d * sizeof(float));
    }

    char chunk[MAX_ROW_LEN];
    int count = 0;

    while(fgets(chunk, sizeof(chunk), dataFile) != NULL) {
        int dimension = 0;

        // Split the row by ","
        char *token = strtok(chunk, ",");

        while (token != NULL) {
            vectorList[count][dimension++] = atof(token);
            token = strtok(NULL, ",");
        }
        count++;
    }

    // Close the file
    fclose(dataFile);

    return vectorList;
}

// Selects the first K vectors
float **initializeCentroids(float **vectorList, int k, int d){
    float **CentroidsList = (float **)malloc(k * sizeof(float *));
    
    for (int i = 0; i < k; i++) {
        CentroidsList[i] = (float *)malloc(d * sizeof(float));
    }

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < d; j++) {
            CentroidsList[i][j] = vectorList[i][j];
        }
    }

    return CentroidsList;
}


// Compute the Euclidean distance between two arrays
float euclidean_distance(float *array1, float *array2, int d) {
    float sum = 0.0;

    for (int i = 0; i < d; i++) {
        float diff = array1[i] - array2[i];
        sum += diff * diff;
    }

    return sqrt(sum);
}


int main() {

    char *filename = "input_1.txt";
    int  k=3, n=800, d=3, max_iter=600;


    bool isValid = test_validation(k, n, d, max_iter);

    // If the validation succeeds, print a success message
    if (isValid) {
        float **vectorList = readDataFromFile(filename, n, d);

        float **CentroidsList = initializeCentroids(vectorList, k, d);


         // Printing the array for verification
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                printf("%f ", CentroidsList[i][j]);
            }
        printf("\n");
        }

        printf("\n done");
    }
    return 0;
}
