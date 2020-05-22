#ifndef PROJECTILE
#define PROJECTILE 1

#include "main.h"

#define MAX_PROJECTILES 10

typedef struct projectile {
    int active;
    vector origin;
    float xPos;
    int speed;
    int dir;
    int index;
} projectile;

void updateProjectiles();
void drawProjectile(projectile *p);
int projectileCollision(projectile *p);
void removeProjectile(int index);
void addProjectile(int index);
int firstAvailableIndex();

#endif