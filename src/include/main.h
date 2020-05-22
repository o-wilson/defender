#ifndef MAIN
#define MAIN 1
#define VECTOR 1

extern int gameState;

typedef struct vector {
    float x;
    float y;
} vector;

vector addVector(vector v1, vector v2);
vector lerpVector(vector v1, vector v2, int lerpSpeed);
float modulus(vector v1, vector v2);
int isColliding(vector v1, vector v2, float threshold);



// Can't have more than one array in a struct
// so each key is its own struct
struct keyData {
    int key;
    int keyLast;
};

typedef struct input {
    int size;
    struct keyData keys[];
} input;

void setInput(struct input input, int value, int keySet);
void setKeysLast(struct input input);

int isKey(struct input input, int key);
int isKeyDown(struct input input, int key);
int isKeyUp(struct input input, int key);



typedef struct star {
    vector position;
    int zDepth;
} star;

#endif