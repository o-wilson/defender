#include <ncurses.h>

#include "screen.h"
#include "player.h"
#include "terrain.h"
#include "main.h"

// Screen wipe animation
void wipe() {
    int wipeWidth = 5;
    int numWipers = (windowWidth / wipeWidth) + 1;

    for (int y = -15; y < windowHeight + numWipers; y++) {
        int numCorr = rand() % 5 + 2 * ((y>0) * y);
        for (int i = 0; i < numCorr; i++) {
            int drawY = rand() % windowHeight;
            
            int clearedX = ((y - drawY) * wipeWidth) * (drawY <= y);
            int minX = windowWidth - clearedX;
            int drawX = rand();
            if (minX) {
                drawX %= minX;
            }
            drawX += clearedX;
            
            int color = COLOR_PAIR(rand() % 5 + 1);
            mvaddch(drawY, drawX, ACS_CKBOARD | color);
        }
        for (int x = 0; x < numWipers; x++) {
            attron(COLOR_PAIR(6));
            for (int i = 0; i < wipeWidth; i++) {
                mvaddch(y - x, x * wipeWidth + i, ' ');
            }
            attroff(COLOR_PAIR(6));
            attron(COLOR_PAIR(5));
            for (int i = 0; i < wipeWidth; i++) {
                mvaddch(y - x - 1, x * wipeWidth + i, ' ');
            }
            attroff(COLOR_PAIR(5));
        }
        refresh();
        delay(40 + ((y < 0) * 100));
    }
}

// Draws supplied text in the center of the screen
void drawText(char* gameOverText) {
    int drawX = (windowWidth - strlen(gameOverText)) / 2;
    int drawY = windowHeight / 2 - 1;
    // mvprintw(drawY, drawX, "%s", gameOverText);
    attron(COLOR_PAIR(3));
    for (int i = 0; i < strlen(gameOverText); i++) {
        mvaddch(drawY, drawX + i, gameOverText[i]);
        if (i != strlen(gameOverText) - 1)
            mvaddch(drawY, drawX + i + 1, '_' | A_BLINK);
        refresh();
        int ms = 300 + (gameOverText[i] == ' ' * 400);
        delay(ms);
    }

    char score[] = "SCORE: 0000";
    int scoreLength = strlen(score);

    int points = player.score;
    int index = 1;
    while (points > 0) {
        int digit = points % 10;
        score[scoreLength - index] = digit + 48; // Convert to ASCII
        points /= 10;
        index++;
    }

    int scoreDrawX = (windowWidth - strlen(score)) / 2;
    for (int i = 0; i < strlen(score); i++) {
        mvaddch(drawY + 1, scoreDrawX + i, score[i]);
        mvaddch(drawY + 1, scoreDrawX + i + 1, '_' | A_BLINK);
        refresh();
        int ms = 300 + (score[i] == ' ' * 400);
        delay(ms);
    }
    attroff(COLOR_PAIR(3));

    delay(2000);

    for (int i = 0; i <= strlen(gameOverText); i++) {
        mvaddch(drawY, drawX + i, ' ');
        refresh();
        int ms = 200 + (gameOverText[i] == ' ' * 400);
        delay(ms);
    }

    for (int i = 0; i <= strlen(score); i++) {
        mvaddch(drawY + 1, scoreDrawX + i, ' ');
        refresh();
        int ms = 200 + (score[i] == ' ' * 400);
        delay(ms);
    }
}

// Called on game loss
void gameOver() {
    wipe();

    delay(1200);

    char* gameOverText = "GAME OVER";
    drawText(gameOverText);

    delay(1000);
}

// Called on game win
void winGame() {
    wipe();

    delay(1200);

    char* gameOverText = "CONGRATULATIONS!";
    drawText(gameOverText);

    delay(1000);
}

// Called when player loses a life
// Decrements life count and respawns player
void loseLife() {
    if (player.lifeCount <= 0) {
        player.lifeCount--;
        gameState = 0;
        return;
    }

    

    delay(1000);

    // Remove any buffered keypresses so that player is stationary
    int ch;
    while(ch = getch() != ERR);

    player.lifeCount--;
    player.velocity.x = 0;
    player.velocity.y = 0;
    player.acceleration.x = 0;
    player.acceleration.y = 0;

    do {
        player.position.x = rand() % terrainWidth;
    } while(checkPlayerCollision());
}