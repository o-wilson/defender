#include "player.h"
#include "screen.h"
#include "terrain.h"
#include "hud.h"
#include "alien.h"

#include <ncurses.h>

// Initialises the player with default starting values
void initialisePlayer(int posX, int posY) {
    player.position.x = posX;
    player.position.y = posY;

    player.lastPosition.x = posX;
    player.lastPosition.y = posY;

    player.velocity.x = 0;
    player.velocity.y = 0;

    player.acceleration.x = 0;
    player.acceleration.y = 0;

    player.direction = 1;
    player.facing = 1;
    player.lerpSpeed = 4;

    player.thrust = 0.2;
    player.maxSpeed = 1;
    player.verticalSpeed = 1; 

    player.characterRight = '>';
    player.characterLeft = '<';
    player.thrustChar = '-';

    player.baseOffset = windowWidth / 4;
    player.currentOffset = player.baseOffset;
    player.targetOffset = player.currentOffset;

    player.lifeCount = 3;
    player.smartBombs = 5;

    player.shootDelay = 10;
    player.currentShootTimer = 0;

    for (int i = 0; i < MAX_PROJECTILES; i++) {
        removeProjectile(i);
    }

    player.score = 0;
}

// Update the player position and velocity
void updatePlayer() {
    setPlayerLast();

    player.velocity = addVector(player.velocity, player.acceleration);
    player.position = addVector(player.position, player.velocity);
    player.currentShootTimer--;

    //Lock vertical movement to screen
    if (player.position.y < HUD_HEIGHT + 1) {
        player.position.y = HUD_HEIGHT + 1;
    } else if (player.position.y > (windowHeight - TER_GND_LEVEL)) {
        player.position.y = (windowHeight - TER_GND_LEVEL);
    }

    //Update player offset
    float frameLerp = player.lerpSpeed * 1.0/60.0;
    float dist = player.targetOffset - player.currentOffset;
    player.currentOffset += dist * frameLerp;

    //Loop
    if (player.position.x >= terrainWidth) {
        player.position.x -= terrainWidth;
    } else if (player.position.x < 0) {
        player.position.x += terrainWidth;
    }

    int colliding = checkPlayerCollision();
    if (colliding) {
        loseLife();
    }
}

// Set the previous position of the player character
void setPlayerLast() {
    player.lastPosition.x = player.position.x;
    player.lastPosition.y = player.position.y;
}

// Move the player based on which key is pressed
// Returns 1 for horizontal movement, 0 for vertical
int movePlayer(char keyPressed) {
    if (keyPressed == 'D') {
        if (player.velocity.x < player.maxSpeed) {
            player.acceleration.x = player.thrust;
        } else {
            player.velocity.x = player.maxSpeed;
            player.acceleration.x = 0;
        }
        player.facing = 1;
        player.targetOffset = player.baseOffset;
    } else if (keyPressed == 'A') {
        if (player.velocity.x > -player.maxSpeed) {
            player.acceleration.x = -player.thrust;
        } else {
            player.velocity.x = -player.maxSpeed;
            player.acceleration.x = 0;
        }
        player.facing = -1;
        player.targetOffset = windowWidth - player.baseOffset;
    }

    // Vertical movement
    if (keyPressed == 'W') {
        player.velocity.y = -player.verticalSpeed;
    } else if (keyPressed == 'S') {
        player.velocity.y = player.verticalSpeed;
    } else {
        player.velocity.y = 0;
    }

    return (keyPressed == 'A' || keyPressed == 'D');
}

// Gradually decrease the velocity of the player until it is stopped
void deceleratePlayer() {
    if (fabs(player.velocity.x) < 0.2) {
        player.velocity.x = 0;
    }

    int tooFar = player.velocity.x * player.direction < 0;
    if (player.velocity.x && !tooFar) {
        float accScalar = 0.1 * -getSign(player.velocity.x);
        player.acceleration.x = player.thrust * accScalar;
    } else {
        player.velocity.x = 0;
        player.acceleration.x = 0;
    }
}

// Draw the player and thrust characters at the correct positions
void drawPlayer(int accelerating) {
    if (accelerating) {
        attron(COLOR_PAIR(4));
        int thrustX = player.currentOffset - player.facing;
        mvaddch(player.position.y, thrustX, player.thrustChar);
        attroff(COLOR_PAIR(4));
    }

    char c;
    if (player.facing == 1) {
        c = player.characterRight;
    } else if (player.facing == -1) {
        c = player.characterLeft;
    }
    attron(COLOR_PAIR(2));
    mvaddch(player.position.y, player.currentOffset, c | A_BOLD);
    attroff(COLOR_PAIR(2));
}

// Update the current direction of travel
void setPlayerDirection() {
    if (player.velocity.x) {
        player.direction = getSign(player.velocity.x);
    }
}

// Checks whether the player is hitting an alien
int checkPlayerCollision() {
    for (int i = 0; i < MAX_ALIENS; i++) {
        if (aliens[i].alive && aliens[i].onscreen) {
            if (isColliding(player.position, aliens[i].pos, 0.9)) {
                return 1;
            }
        }
    }
    return 0;
}

// Creates a projectile at the first available index
void shoot() {
    int index = firstAvailableIndex();
    if (index != -1)
        addProjectile(index);
}

// Uses a smart bomb
void smartBomb() {
    if (player.smartBombs <= 0)
        return;
    
    player.smartBombs --;
    killAliensOnScreen();
}

// Return +/- 1 based on sign of x
int getSign(float x) {
    return (int)(x / fabs(x));
}