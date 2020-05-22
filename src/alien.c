#include "screen.h"
#include "alien.h"
#include "terrain.h"
#include "player.h"
#include "hud.h"
#include "person.h"

#include <ncurses.h>

// Globally accessible array
alien aliens[MAX_ALIENS];


// Initialises aliens with starting values
void initialiseAliens() {
    for (int i = 0; i < MAX_ALIENS; i++) {
        float x = rand() % terrainWidth;
        int yMax = windowHeight - TER_GND_LEVEL - 3;
        int yMin = HUD_HEIGHT + 2;
        float y = rand() % (yMax - yMin) + yMin;

        alien a = {
            i, 0,
            {x, y},
            {x, y},
            5, 1, 1,
            150,
            6, -1, -1
        };

        aliens[i] = a;
    }
}

// Updates each living alien each time the game loop runs
void updateAliens() {
    /* MOVEMENT
    * If not arrived            1
    *   Move towards target
    * Else
    *   Clamp position
    *   If holding              2
    *     Remove person
    *   Else if next to person  3
    *     Pick up person
    *   
    *   If holding              4
    *     Set target to top
    *   Else if person in range 5
    *     Set target to person
    *   Else
    *     Set random target
    */
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].alive) {
            // Has the alien arrived at its target
            float dist = modulus(aliens[i].pos, aliens[i].nextPos);
            aliens[i].arrived = dist < 1.5;

            if (!aliens[i].arrived) {               // 1
                moveAlien(&(aliens[i]));
            } else {
                // Clamp x position to bounds of the terrain
                int clampedX = aliens[i].pos.x;
                while (clampedX < 0)
                    clampedX += terrainWidth;
                clampedX %= terrainWidth;
                aliens[i].pos.x = clampedX;
                
                // Check whether next to person
                int targetID = aliens[i].targeting;
                int colliding = 0;
                if (targetID != -1) {
                    vector aPos = aliens[i].pos;
                    vector target = getPersonPosition(targetID);
                    colliding = isColliding(aPos, target, 2);
                    int caught = getPerson(targetID)->captor != -1;
                    int alive = getPerson(targetID)->alive;
                    colliding = colliding && !caught && alive;
                }

                // Check whether holding person and remove
                if (aliens[i].captive != -1) {      // 2
                    removePerson(aliens[i].captive);
                    aliens[i].captive = -1;
                // Check whether next to person and pick up
                } else if ((targetID != -1) && colliding) {    // 3
                    captureCitizen(&(aliens[i]), &(people[targetID]));
                }
                
                float x = aliens[i].pos.x;
                float y = aliens[i].pos.y;

                // Generate new target for alien to move towards

                int personID = checkForPeople(&(aliens[i]));
                if (aliens[i].captive != -1) {      // 4
                    y = HUD_HEIGHT + 1;
                } else if (personID != -1) {        // 5
                    vector tPos = getPersonPosition(personID);
                    aliens[i].targeting = personID;
                    x = tPos.x;
                    y = tPos.y;
                } else {
                    x += rand() % 31 - 15;
                    y += rand() % 9 - 4;
                }

                setTarget(&(aliens[i]), x, y);
            }

            drawAlien(&(aliens[i]));
        }
    }
}

// Set the target from x and y values given
void setTarget(alien *a, float x, float y) {
    if (y > windowHeight - TER_GND_LEVEL) {
        y = windowHeight - TER_GND_LEVEL;
    } else if (y < HUD_HEIGHT + 1) {
        y = HUD_HEIGHT + 1;
    }

    a->nextPos.x = x;
    a->nextPos.y = y;
}

// Return the position that a captive person should be drawn at
vector getCaptorPos(int id) {
    vector pos = {0,0};
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].id == id) {
            pos.x = aliens[i].pos.x;
            pos.y = aliens[i].pos.y + 1;
        } 
    }
    return pos;
}

// Moves the alien towards its target
void moveAlien (alien *a) {
    int xDiff = a->nextPos.x - a->pos.x;
    int yDiff = a->nextPos.y - a->pos.y;
    int xDir = 1, yDir = 1;
    if (xDiff)
    xDir = xDiff / abs(xDiff);
    if (yDiff)
    yDir = yDiff / abs(yDiff);

    float deltaX = a->moveSpeed * xDir * (1.0 / 60.0);
    float deltaY = a->moveSpeed * yDir * (1.0 / 60.0);
    vector deltaPos = {deltaX, deltaY};

    a->pos = addVector(a->pos, deltaPos);
}

// Returns ID of the closest, living person, if they are in range
int checkForPeople(alien *a) {
    float shortestDist = 10000000000;
    int closestPerson = -1;

    for (int i = 0; i < MAX_PEOPLE; i++) {
        float dist = modulus(a->pos, people[i].pos);
        int grounded = people[i].pos.y == windowHeight-TER_GND_LEVEL;
        int valid = people[i].alive && grounded;
        if (dist < shortestDist && valid) {
            shortestDist = dist;
            closestPerson = i;
        }
    }

    if (shortestDist < a->range) {
        return closestPerson;
    } else {
        return -1;
    }
}

// Sets necessary variables for abduction
void captureCitizen(alien *a, person *p) {
    a->captive = p->id;
    p->captor = a->id;
}

// Draws the alien at the correct location, if it is on screen
void drawAlien(alien *a) {
    int drawX = getScreenX(a->pos.x);
    a->onscreen = isOnScreen(drawX);
    if (a->onscreen) {
        attron(COLOR_PAIR(3));
        mvaddch(a->pos.y, drawX, 'M');
        attroff(COLOR_PAIR(3));
    }
}

// Kills the alien and releases any captive
void killAlien(int id) {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].id == id && aliens[i].alive) {
            aliens[i].alive = 0;
            addPoints(aliens[i].points);

            if (aliens[i].captive != -1) {
                releasePerson(aliens[i].captive);
            }

            return;
        }
    }
}

// Kills all visible aliens (smart bomb)
void killAliensOnScreen() {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].alive && aliens[i].onscreen) {
            killAlien(aliens[i].id);
        }
    }
}