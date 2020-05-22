#include <ncurses.h>
#include <string.h>
#include <math.h>

#include "screen.h"
#include "player.h"
#include "hud.h"
#include "terrain.h"

// Draws main frame for HUD
void drawHUD() {
    for (int i = 0; i < HUD_HEIGHT; i++) {
        move(i, 0);
        clrtoeol;
    }

    attron(COLOR_PAIR(3));

    float sideProportion = (1 - HUD_CENTER_PROP) / 2;
    int sideDivider = windowWidth * sideProportion;

    for (int i = 0; i < windowWidth; i++) {
        mvaddch(HUD_HEIGHT, i, ACS_HLINE);
    }

    for (int i = 0; i <= HUD_HEIGHT; i++) {
        if (i != HUD_HEIGHT) {
            mvaddch(i, sideDivider, ACS_VLINE);
            mvaddch(i, windowWidth - sideDivider, ACS_VLINE);
        } else {
            mvaddch(i, sideDivider, ACS_BTEE);
            mvaddch(i, windowWidth - sideDivider, ACS_BTEE);
        }
    }

    attroff(COLOR_PAIR(3));

    drawScore(sideDivider);
    drawLives(sideDivider);
    drawBombs(sideDivider);
}

// Adds points to score
void addPoints(int pointsToAdd) {
    player.score += pointsToAdd;
}

// Draws score in correct section
void drawScore(int sectionWidth) {
    if (player.score > 99999) {
        player.score = 99999;
    }

    attron(COLOR_PAIR(1));

    char score[] = "SCORE: 0000";
    int scoreLength = strlen(score);
    int startX = (sectionWidth - scoreLength) / 2;

    int points = player.score;
    int index = 1;
    while (points > 0) {
        int digit = points % 10;
        score[scoreLength - index] = digit + 48; // Convert to ASCII
        points /= 10;
        index++;
    }

    mvprintw(HUD_HEIGHT / 2, startX, "%s", score);

    attroff(COLOR_PAIR(1));
}

// Draws lives in correct section
void drawLives(int sectionWidth) {
    char lives[] = "LIVES:";
    int startX = windowWidth - (sectionWidth - 1);
    int startLives = 7;
    attron(COLOR_PAIR(1));
    mvprintw(HUD_HEIGHT / 2, startX, "%s", lives);
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2));
    startX += startLives;
    for (int i = 0; i < player.lifeCount; i++) {
        startX += 2 * (i > 0);
        int character = player.characterRight | A_BOLD;
        mvaddch(HUD_HEIGHT / 2, startX, character);
    }
    attroff(COLOR_PAIR(2));
}

// Draws bombs in correct section
void drawBombs(int sectionWidth) {
    char bombs[] = "BOMBS:";
    int startX = windowWidth - (sectionWidth - 1);
    int startBombs = 7;
    attron(COLOR_PAIR(1));
    mvprintw((HUD_HEIGHT / 2) + 1, startX, "%s", bombs);
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(7));
    startX += startBombs;
    for (int i = 0; i < player.smartBombs; i++) {
        int character = 48 + ((i % 2) * 31);
        mvaddch((HUD_HEIGHT / 2) + 1, startX + i, character);
    }
    attroff(COLOR_PAIR(7));
}