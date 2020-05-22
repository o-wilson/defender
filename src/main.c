#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>

#include "main.h"
#include "player.h"
#include "terrain.h"
#include "alien.h"
#include "projectile.h"
#include "screen.h"
#include "hud.h"
#include "person.h"

void delay(int milliseconds);
void update();

int windowWidth, windowHeight;
int topOfWindow = 5;
int mapWidth, terrainWidth;

int gameState;

struct player player;

// Initialise the ncurses window
void start() {
    initscr();
    getmaxyx(stdscr, windowHeight, windowWidth);
    mapWidth = (windowWidth / 2) * TER_SCALE;

    // Initialise the colors to use
    start_color();                              /*~~COLORS~~*/
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);    // 1: Yellow
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);   // 2: Magenta
    init_pair(3, COLOR_GREEN, COLOR_BLACK);     // 3: Green
    init_pair(4, COLOR_CYAN, COLOR_BLACK);      // 4: Cyan
    init_pair(5, COLOR_WHITE, COLOR_BLACK);     // 5: White
    init_pair(6, COLOR_WHITE, COLOR_WHITE);     // 6: White on White
    init_pair(7, COLOR_RED, COLOR_BLACK);       // 7: Red

    noecho();
    keypad(stdscr, true);
    raw();
    curs_set(0);
    srand(time(0));

    terrainWidth = mapWidth * TER_SCALE;
}

int main() {
    start();

    generateStars();

    int heightMap[mapWidth];
    generateTerrain(heightMap, mapWidth);

    initialisePeople();
    initialiseAliens();
    initialisePlayer(0, windowHeight / 2);

    int accelerating;
    int ch;
    
    int aliensRemaining = MAX_ALIENS;
    int peopleRemaining = MAX_PEOPLE;

    char intro[15][49] =
    {
        " ____  _____ _____ _____ _      ____  _____ ____ ",
        "/  _ \\/  __//    //  __// \\  /|/  _ \\/  __//  __\\",
        "| | \\||  \\  |  __\\|  \\  | |\\ ||| | \\||  \\  |  \\/|",
        "| |_/||  /_ | |   |  /_ | | \\||| |_/||  /_ |    /",
        "\\____/\\____\\\\_/   \\____\\\\_/  \\|\\____/\\____\\\\_/\\_\\",
        "                                                 ",
        "                                                 ",
        "                 ***CONTROLS***                  ",
        "                A/D - Left/Right                 ",
        "                W/S - Up/Down                    ",
        "                Z   - Shoot                      ",
        "                X   - Smart Bomb                 ",
        "                Q   - Quit                       ",
        "                                                 ",
        "             Press any key to start.             "
    };

    int startX = (windowWidth - 49) / 2;
    int startY = (windowHeight - 15) / 2;
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 49; x++) {
            mvaddch(startY + y, startX + x, intro[y][x]);
        }
    }

    getch();

    gameState = 1;

    // Stops blocking from getch()
    nodelay(stdscr, true);

    // Loop while game is being played
    while (gameState == 1) {
        // Get current key pressed
        ch = getch();

        // Update current screen size
        getmaxyx(stdscr, windowHeight, windowWidth);
        
        // Check for victory
        aliensRemaining = 0;
        for (int i = 0; i < MAX_ALIENS; i++) {
            aliensRemaining += aliens[i].alive;
        }
        if (!aliensRemaining) {
            gameState = 2;
        }

        // Check for loss
        peopleRemaining = 0;
        for (int i = 0; i < MAX_PEOPLE; i++) {
            peopleRemaining += people[i].alive;
        }
        if (!peopleRemaining) {
            gameState = 0;
        }

        //Player movement
        player.velocity.y = 0;
        player.acceleration.y = 0;
        accelerating = 0;
        setPlayerDirection();

        // If a WASD key is pressed, move the ship appropriately
        ch = toupper(ch);
        if (ch == 'W' || ch == 'A' || ch == 'S' || ch == 'D') {
            accelerating = movePlayer(toupper(ch));
        }

        // if (ch == 'P') loseLife();

        if (ch == 'Q') gameState = 0;

        if (ch == 'Z') {
            if (player.currentShootTimer < 0) {
                shoot();
                player.currentShootTimer = player.shootDelay;
            }
        }

        if (ch == 'X') {
            smartBomb();
        }

        // If player is not moving the ship, then decelerate
        if (!accelerating) {
            deceleratePlayer();
        }



        // Update all elements
        updateStars();
        updateTerrain(heightMap, mapWidth);
        updatePlayer();
        updateProjectiles();
        updateAliens();
        updatePeople();



        drawPlayer(accelerating);
        


        /* DEBUG */

        // mvprintw(1, 0, "%d", player.direction);
        // mvprintw(2, 0, "%f,%f", a.pos.x, a.pos.y);
        // mvprintw(3, 0, "%f,%f", a.nextPos.x, a.nextPos.y);
        // mvprintw(4, 0, "%i", a.arrived);
        // mvprintw(5, 0, "%f", modulus(a.pos, a.nextPos));

        /* DEBUG */

        drawHUD(); // IMPORTANT: Last thing drawn to ensure it's always on top
        refresh();
        delay((int)((1.0f/60.0f)*1000));
    }

    delay(300);

    if (gameState == 0) {
        gameOver();
    } else if (gameState == 2) {
        winGame();
    }

    endwin();

    return 0;
}

//Creates a delay of the specified number of milliseconds
void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}