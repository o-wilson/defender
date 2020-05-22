#ifndef PERSON
#define PERSON 1

#include "main.h"

#define MAX_PEOPLE 10
#define MIN_MD 40
#define MAX_MD 70
#define GRAV_FACTOR 4

typedef struct person {
    int id;
    int alive;
    vector pos;
    vector nextPos;
    int arrived;
    int speed;
    int minMoveDelay;
    int maxMoveDelay;
    int moveDelay;
    char personChar;
    int captor;
} person;

extern person people[MAX_PEOPLE];

void initialisePeople();
void updatePeople();
void drawPerson(person *p);
void releasePerson(int id);
person* getPerson(int id);
void removePerson(int id);
vector getPersonPosition(int id);

#endif