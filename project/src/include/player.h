
#ifndef PLAYER
#define PLAYER 1

#include "main.h"
#include "projectile.h"

struct player {
    vector position;
    vector lastPosition;
    vector velocity;
    vector acceleration;

    int direction;  // 1 = right
    int facing;     // "       "
    float lerpSpeed;

    float thrust;
    float maxSpeed;
    float verticalSpeed;

    char characterRight;
    char characterLeft;
    char thrustChar;

    int baseOffset;
    int currentOffset;
    int targetOffset;

    int lifeCount;
    int smartBombs;

    int shootDelay;
    int currentShootTimer;

    projectile projectiles[MAX_PROJECTILES];

    int score;
};

void initialisePlayer(int posX, int posY);
void updatePlayer();
void setPlayerLast();
int movePlayer(char keyPressed);
void deceleratePlayer();
void drawPlayer(int accelerating);
void setPlayerDirection();
int checkPlayerCollision();
void shoot();
int getSign(float x);

extern struct player player;

#endif
