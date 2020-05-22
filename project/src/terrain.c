#include <ncurses.h>

#include "terrain.h"
#include "screen.h"
#include "player.h"
#include "hud.h"

int maxStarCount;
star *stars;

// Check whether a generated point on the heightmap is
// within the defined minimum and maximum terrain heights
int inBounds(int y) {
    return (y >= MIN_TER_H && y <= MAX_TER_H);
}

// Check whether a generated point, currY, is within the max
// step difference of the previous point, prevY
int stepValid(int currY, int prevY) {
    return abs(prevY - currY) <= MAX_TER_STEP;
}

// Generate an array of height values based on parameters
// #defined in terrain.h
void generateTerrain(int* heightMap, int mapWidth) {
    for (int i = 0; i < mapWidth; i++) {
        int yVal = MIN_TER_H - 1;
        int randMod = MAX_TER_H - MIN_TER_H + 1;

        if (!i) {
            yVal = TER_START_H;
        } else {
            int yPrevVal = heightMap[i-1];
            while (!inBounds(yVal) || !stepValid(yVal, yPrevVal)) {
                yVal = rand() % randMod + MIN_TER_H;
            }
        }

        heightMap[i] = yVal;
    }
}

// Clear the previously displayed terrain and display the section
// that should currently be visible
void updateTerrain(int* heightMap, int mapWidth) {
    clearTerrain(windowHeight);

    attron(COLOR_PAIR(1));

    float playerReal = player.position.x - player.currentOffset;
    int terrainStart = (int)(playerReal / TER_SCALE);
    int numPoints = (windowWidth / TER_SCALE) + 2;
    int terrainEnd = terrainStart + numPoints;

    for (int i = terrainStart; i < terrainEnd; i++) {
        float pAdj = player.currentOffset - player.position.x;
        int startX = pAdj + (i * TER_SCALE);
        int startY = heightMap[(mapWidth + i) % mapWidth];
        int endY = heightMap[(mapWidth + i + 1) % mapWidth];
        int groundY = windowHeight - TER_GND_LEVEL;

        drawLine(startX, startY, endY, groundY);
    }

    attroff(COLOR_PAIR(1));
}

// Remove previous terrain by clearing all terrain rows
void clearTerrain(int windowHeight) {
    int startY = windowHeight - TER_GND_LEVEL - MAX_TER_H;
    for (int y = startY; y <= windowHeight; y++) {
        move(y, 0);
        clrtoeol();
    }
}

// Procedure for choosing how to connect points of terrain
void drawLine(int startX, int startY, int endY, int groundY) {
    int yDiff = abs(endY - startY);
    
    char slopeChar = '/';
    char flatChar = '_';
    int dir = 1; //1 = up, -1 = down
    int drawY;

    // If "going down"
    if (endY < startY) {
        slopeChar = '\\';
        dir = -1;
    }

    if (yDiff <= TER_SCALE) {
        /*
        * for 0 -> yDiff
        *   draw slopeChar at correct coords
        * 
        * for the rest (up to terrain scale)
        *   draw flatChar at correct coords
        * */
        for (int i = 0; i < yDiff; i++) {
            drawY = groundY - (startY + (dir * i)) + (dir < 0);
            if (startX + i >= 0)
                mvaddch(drawY, startX + i, slopeChar);
        }

        for (int i = yDiff; i < TER_SCALE; i++) {
            drawY = groundY - endY;
            if (startX + i >= 0)
                mvaddch(drawY, startX + i, flatChar);
        }
    } else {
        /*
        * for the first two characters
        *   draw top and bottom slopes
        *   fill in with verticals
        * 
        * for the rest
        *   draw flatChar
        * */
        for (int i = 0; i < yDiff; i++) {
            drawY = groundY - (startY + (dir * i)) + (dir < 0);
            if (i == 0 || i == yDiff - 1) {
                mvaddch(drawY, startX + (i != 0), slopeChar);
            } else {
                mvaddch(drawY, startX + (dir > 0), '|');
            }
        }
        
        for (int i = 2; i < TER_SCALE; i++) {
            drawY = groundY - endY;
            mvaddch(drawY, startX + i, flatChar);
        }
    }
}

// Randomly position stars in the background
void generateStars() {
    int starScatter = 35;     // maximum scatter
    starScatter /= 2;         // minimum scatter
    
    int horizonLevel = (windowHeight - TER_GND_LEVEL) - MAX_TER_H;
    int yRange = horizonLevel - HUD_HEIGHT;
    maxStarCount = (yRange * terrainWidth) / starScatter;
    int currentStarCount = 0;
    stars = (star*)malloc(sizeof(star) * maxStarCount);
    int starPos = starScatter;
    for (int y = HUD_HEIGHT; y < horizonLevel; y++) {
        for (int x = 0; x < terrainWidth; x++) {
            if (starPos == 0) {
                if (currentStarCount < maxStarCount) {
                    stars[currentStarCount].position.x = x;
                    stars[currentStarCount].position.y = y;
                    stars[currentStarCount].zDepth = rand() % 5 + 3;
                    currentStarCount ++;
                }
                starPos = rand() % starScatter + starScatter;
            }
            starPos--;
        }
    }
}

// Draws stars at updated position onscreen
void updateStars() {
    int horizonLevel = (windowHeight - TER_GND_LEVEL) - MAX_TER_H;
    for (int i = 0; i < horizonLevel; i++) {
        move(i, 0);
        clrtoeol();
    }

    for (int i = 0; i < maxStarCount; i++) {
        float deltaPX = player.position.x - player.lastPosition.x;
        stars[i].position.x -= deltaPX / stars[i].zDepth;
        if (stars[i].position.x < 0) {
            stars[i].position.x += terrainWidth;
        } else if (stars[i].position.x > terrainWidth) {
            stars[i].position.x -= terrainWidth;
        }
        float floatX = player.currentOffset + stars[i].position.x;
        int drawX = (int)floatX % (int)terrainWidth;
        mvaddch(stars[i].position.y, drawX, '.');
    }
}