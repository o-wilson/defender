#include <ncurses.h>

#include "person.h"
#include "terrain.h"
#include "screen.h"
#include "alien.h"

// Globally accessible array
person people[MAX_PEOPLE];

// Initialises people with default values
void initialisePeople() {
    for (int i = 0; i < MAX_PEOPLE; i++) {
        float x = rand() % terrainWidth;
        float y = windowHeight - TER_GND_LEVEL;
        int moveDelay = rand() % (MAX_MD - MIN_MD) + MIN_MD;
        person p = {
            i, 1,
            {x, y},
            {x, y},
            1, 5,
            MIN_MD, MAX_MD, moveDelay,
            '|', -1
        };
        people[i] = p;
    }
}

// Updates all living people each time the game loop runs
void updatePeople() {
    int groundLevel = windowHeight - TER_GND_LEVEL;

    for (int i = 0; i < MAX_PEOPLE; i++) {
        if (!people[i].alive) continue;

        if (people[i].captor != -1) {
            vector pos = getCaptorPos(people[i].captor);
            people[i].pos.x = pos.x;
            people[i].pos.y = pos.y;
            drawPerson(&(people[i]));
            continue;
        }

        if (people[i].pos.y < groundLevel) {
            people[i].pos.y += GRAV_FACTOR * (1.0 / 60.0);
        } else if (people[i].pos.y > groundLevel) {
            people[i].pos.y = groundLevel;
        }

        float dist = modulus(people[i].pos, people[i].nextPos);
        people[i].arrived = dist < 1;
        
        if (!people[i].arrived && people[i].pos.y == groundLevel) {
            int diff = people[i].nextPos.x - people[i].pos.x;
            if (diff) {
                int dir = diff / abs(diff);
                int deltaX = people[i].speed * dir;
                people[i].pos.x += deltaX * (1.0 / 60.0);
            }
        } else if (people[i].moveDelay <= 0) {
            people[i].moveDelay = rand() % (MAX_MD - MIN_MD) + MIN_MD;
            float xDiff = rand() % (20 - 5) + 5;
            float newDir = rand() % 2;
            xDiff -= (newDir == 0) * 2 * xDiff;
            
            int clampedX = people[i].pos.x;
            while (clampedX < 0)
                clampedX += terrainWidth;
            clampedX %= terrainWidth;

            people[i].pos.x = clampedX;
            people[i].nextPos.x = clampedX;
            people[i].nextPos.x += xDiff;
            people[i].nextPos.y = groundLevel;
        } else if (people[i].moveDelay > 0){
            people[i].moveDelay --;
        }

        

        drawPerson(&(people[i]));
    }
}

// Draws a person at the correct position,if on screen
void drawPerson(person *p) {
    attron(COLOR_PAIR(2));

    int drawX = getScreenX(p->pos.x);
    if (isOnScreen(drawX)) {
        mvaddch(p->pos.y, drawX, p->personChar | A_BOLD);
    }

    attroff(COLOR_PAIR(2));
}

// Releases a person from an alien
void releasePerson(int id) {
    for (int i = 0; i < MAX_PEOPLE; i++) {
        if (people[i].id == id) {
            people[i].captor = -1;
            return;
        }
    }
}

// Returns a pointer to a person from a supplied ID
person* getPerson(int id) {
    for (int i = 0; i < MAX_PEOPLE; i++) {
        if (people[i].id == id) {
            return &(people[i]);
        }
    }
}

// Removes a person by setting their alive flag to 0
void removePerson(int id) {
    for (int i = 0; i < MAX_PEOPLE; i++) {
        if (people[i].id == id) {
            people[i].captor = -1;
            people[i].alive = 0;
            return;
        }
    }
}

// Returns position of person given by supplied ID
vector getPersonPosition(int id) {
    for (int i = 0; i < MAX_PEOPLE; i++) {
        if (people[i].id == id) {
            vector v = {people[i].pos.x, people[i].pos.y};
            return v;
        }
    }
}