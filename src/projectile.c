#include "player.h"
#include "projectile.h"
#include "terrain.h"
#include "screen.h"
#include "alien.h"
#include "main.h"

#include <ncurses.h>
#include <string.h>

// Updates all active projectiles
void updateProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (player.projectiles[i].active != 0) {
            int speed = player.projectiles[i].speed;
            int direction = player.projectiles[i].dir;
            int deltaX = speed * direction;

            player.projectiles[i].xPos += deltaX * (1.0 / 60.0);

            int originX = player.projectiles[i].origin.x;
            int xPos = originX + player.projectiles[i].xPos;
            int drawX = getScreenX(xPos);

            if (!isOnScreen(drawX)) {
                removeProjectile(i);
            } else {
                drawProjectile(&(player.projectiles[i]));
            }
        }

        int colliding = projectileCollision(&(player.projectiles[i]));
        if (colliding != -1) {
            killAlien(colliding);
            removeProjectile(i);
        }
    }
}

// Draws the given projectile at the correct position
void drawProjectile(projectile *p) {
    int xPos = p->origin.x + p->xPos;
    int drawX = getScreenX(xPos);

    mvaddch(p->origin.y, drawX, '-' | A_BOLD);
}

// Checks whether the projectile is colliding with an alien
int projectileCollision(projectile *p) {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].alive && aliens[i].onscreen) {
            vector pPos = {p->origin.x + p->xPos, p->origin.y};

            if (isColliding(aliens[i].pos, pPos, 1.4))
                return aliens[i].id;
        }
    }
    return -1;
}

// Overwrites an index of the projectiles array
void removeProjectile(int index) {
    projectile p = {0, {0, 0}, 0, 0, 0, index};
    player.projectiles[index] = p;
}

// Initialises a new projectile
void addProjectile(int index) {
    float x = player.position.x + player.facing;
    float y = player.position.y;
    int dir = player.facing;
    projectile p = {1, {x, y}, 0, 70, dir, index};
    player.projectiles[index] = p;
}

// Gets the first index available to add a new projectile to
// Returns the index, or -1 if the array is already full
int firstAvailableIndex() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (player.projectiles[i].active == 0)
            return i;
    }

    return -1;
}