#ifndef ALIEN
#define ALIEN 1

#define MAX_ALIENS 15

#include "main.h"
#include "person.h"

typedef struct alien {
    int id;
    int onscreen;
    vector pos;
    vector nextPos;
    int moveSpeed;
    int arrived;
    int alive;
    int points;
    int range;
    int targeting;
    int captive;
} alien;

void initialiseAliens();
void updateAliens();
void setTarget(alien *a, float x, float y);
vector getCaptorPos(int id);
void moveAlien (alien *a);
int checkForPeople(alien *a);
void captureCitizen(alien *a, person *p);
void drawAlien(alien *a);
void killAlien(int id);
void killAliensOnScreen();

extern alien aliens[MAX_ALIENS];

#endif