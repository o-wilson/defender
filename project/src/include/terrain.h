#ifndef MAX_TER_H

#define MIN_TER_H 0
#define MAX_TER_H 5
#define MAX_TER_STEP 3
#define TER_START_H 2
#define TER_GND_LEVEL 3
#define TER_SCALE 3.0

int inBounds(int y);
int stepValid(int currY, int prevY);
void clearTerrain(int windowHeight);
void drawLine(int startX, int startY, int endY, int groundLevel);

extern int terrainWidth;

#endif