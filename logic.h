#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"
#include "stdlib.h"

#define Up 0
#define Down 1
#define Left 2
#define Right 3
#define testDirection(integer, direction) (((integer) & 0x00000003) == (direction))
#define getDirectionArray(pointer, index) (((pointer)[(index) / 16] >> (((index) % 16) * 2)) & 3)
#define setDirectionArray(pointer, direction, index) (((pointer)[(index) / 16] & ~(3 << (((index) % 16) * 2))) | ((direction) << (((index) % 16) * 2)))

#define GROWTH 20
#define MAX_SIZE 800 // multiple of 16
#define MAX_PATH_INDEX (MAX_SIZE - 1)

typedef enum {
    U, R, D, L
} Direction;

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
    
    int snakex;
    int snakey;
    int direction;
    int length;

    int foodx;
    int foody;

    int points;
    int deletex;
    int deletey;

    int stopDelete;
    int pathCounter;
    int *path;

} AppState;


// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);


#endif
