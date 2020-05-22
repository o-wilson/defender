#include "main.h"
#include <math.h>

// Adds two given vectors together and returns the result
vector addVector(vector v1, vector v2) {
    vector result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}

// Linearly interpolates between two vectors
vector lerpVector(vector v1, vector v2, int lerpSpeed) {
    float distX = v2.x - v1.x;
    float distY = v2.y - v1.y;

    vector result = {distX * lerpSpeed, distY * lerpSpeed};

    return result;
}

// Returns distance between two vectors
float modulus(vector v1, vector v2) {
    float result = 0;
    float a = v2.x - v1.x;
    float b = v2.y - v1.y;
    result += pow(a, 2) + pow(b, 2);
    result = sqrtf(fabs(result));
    return result;
}

// Checks whether two supplied vectors are in range of each other
int isColliding(vector v1, vector v2, float threshold) {
    float dist = modulus(v1, v2);
    return dist < threshold;
}