# Recreation of Defender in C

## Compiling and Running the Game

### Simplest Method
1. Compile and run the project using the `make run` command from the `src` directory
    - This compiles the source code into object files, then links them and automatically runs the output file

### Compiling and Running Separately
1. Compile the project using the `make` command from the `src` directory
2. Run the resulting executable with `make run` or `./main` from the same place

### Alternate Method
1. Run `gcc -c *.c -Iinclude` from the `src` directory
2. Run `gcc -o main *.o -lncurses -lm`, also from the `src` directory
3. Run the resulting executable with `./main` from the same place

NB: The game will adapt to any initial terminal size, but that size should then be kept the same for the duration of the game

## Controls
### Movement
W/S - up/down

A/D - left/right

### Combat
Z - Shoot

X - Use smart bomb

## To Do
- [x] Player
    - [x] Movement
        - [x] Left and right with acceleration and deceleration
        - [x] Up and down with no delay
        - [ ] *Detecting and appropriately handling multiple simultaneous inputs*
        - [x] Left and right looping ~~(obsolete once level scrolls)~~
        - [x] Lerp to left or right side of screen depending on direction
        - [x] Update "camera" position
    - [x] Firing
        - [x] Create an instance of a projectile
        - [x] Keep track of position and draw at the relevant location
        - [x] Remove ~~after a length of time not hitting anything~~when offscreen
        - [x] Check for collisions
    - [x] Smart bomb
        - [x] Finds enemies on screen
        - [x] Kill all
    - [x] Can be killed
- [x] Aliens
    - [x] Move
    - [x] Detect and move to citizens and player when in range
    - [x] Can be destroyed by player projectiles
    - [x] Kill player on contact
- [x] Citizens
    - [x] *Wander at ground level*
    - [x] Can be abducted
    - [x] Will fall when alien shot
- [x] Terrain
    - [x] Generate a heightmap with appropriate values
    - [x] Display the terrain using the values from the heightmap
    - [x] Left and right scrolling
        - [x] ~~Keep track of camera position~~ Access "camera" position
        - [x] Draw relevant section(s) of terrain
- [x] UI
    - [ ] *Radar view*
    - [x] Score
        - [x] Updated when necessary
            - [x] Alien killed
            - [ ] Citizen caught
            - [ ] Citizen dropped off
        - [x] Displayed on-screen
    - [x] Number of smart bombs remaining
    - [x] Number of lives remaining
- [ ] *Misc*
    - [ ] *Hyperspace jump*
        - [ ] *Warp player to other point*
        - [ ] *Possibility to go wrong*