#include "main.h"
#include "terrain.h"
#include "player.h"
#include "screen.h"

// Converts a world x coord to a screen x coord
int getScreenX (float xPos) {
    float screenOrigin = player.position.x - player.currentOffset;
    int screenX = (int)(xPos - screenOrigin);

    while (screenX < 0) {
        screenX += terrainWidth;
    }
    screenX %= terrainWidth;

    return screenX;
}

// Checks whether a given screen coordinate is on screen
int isOnScreen (float screenX) {
    return (((int)screenX >= 0) && ((int)screenX < windowWidth));
}