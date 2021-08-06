#include "logic.h"

void initializeAppState(AppState* appState) {

    appState -> snakex = 20;
    appState -> snakey = 20;
    appState -> direction = Right;
    appState -> length = 15;

    appState -> pathCounter = 0;
    appState -> stopDelete = 0;
    appState -> deletex = appState -> snakex - appState -> length - 1;
    appState -> deletey = 20;
    appState -> path = malloc((MAX_SIZE / 16) * sizeof(int));
    appState -> path[0] = 0xFFFFFFFF;

    appState -> foodx = 100;
    appState -> foody = 100;

    appState -> gameOver = 0;
    appState -> points = 0;
}


// This function processes your current app state and returns the new (i.e. next)
// state of application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {

    AppState nextAppState = *currentAppState;

    int curDirection = currentAppState -> direction;

    if (KEY_JUST_PRESSED(BUTTON_DOWN, keysPressedNow, keysPressedBefore) && (curDirection != Up)) {
        nextAppState.direction = Down;
    } else if (KEY_JUST_PRESSED(BUTTON_UP, keysPressedNow, keysPressedBefore) && (curDirection != Down)) {
        nextAppState.direction = Up;
    } else if (KEY_JUST_PRESSED(BUTTON_LEFT, keysPressedNow, keysPressedBefore) && (curDirection != Right)) {
        nextAppState.direction = Left;
    } else if (KEY_JUST_PRESSED(BUTTON_RIGHT, keysPressedNow, keysPressedBefore) && (curDirection != Left)) {
        nextAppState.direction = Right;
    }

    int i = (nextAppState.length + nextAppState.pathCounter) % MAX_SIZE;
    switch (curDirection) {
        case Up:
            nextAppState.path[i / 16] = setDirectionArray(nextAppState.path, Up, i);
            break;
        case Down:
            nextAppState.path[i / 16] = setDirectionArray(nextAppState.path, Down, i);
            break;
        case Left:
            nextAppState.path[i / 16] = setDirectionArray(nextAppState.path, Left, i);
            break;
        case Right:
            nextAppState.path[i / 16] = setDirectionArray(nextAppState.path, Right, i);
            break;
    }

    switch (nextAppState.direction) {
        case Up:
            nextAppState.snakey = currentAppState -> snakey - 1;
            break;
        case Down:
            nextAppState.snakey = currentAppState -> snakey + 1;
            break;
        case Left:
            nextAppState.snakex = currentAppState -> snakex - 1;
            break;
        case Right:
            nextAppState.snakex = currentAppState -> snakex + 1;
            break;
    }


    if ((currentAppState -> snakex < currentAppState -> foodx + 4 && currentAppState -> snakex > currentAppState -> foodx - 4)
        && (currentAppState -> snakey < currentAppState -> foody + 4 && currentAppState -> snakey > currentAppState -> foody - 4)) {
        nextAppState.points += 1;
        nextAppState.foodx = randint(10, WIDTH - 10);
        nextAppState.foody = randint(8, HEIGHT - 8);
        
        nextAppState.length += GROWTH;
        nextAppState.pathCounter -= GROWTH - 1;
        if (nextAppState.pathCounter < 0) {
            nextAppState.pathCounter = MAX_SIZE + nextAppState.pathCounter;
        }
        nextAppState.stopDelete = GROWTH - 1;
    } else if (!(currentAppState -> stopDelete)) {
        //switch (nextAppState.path[0]) 
        switch (getDirectionArray(nextAppState.path, nextAppState.pathCounter)) {
            case Up:
                nextAppState.deletey = currentAppState -> deletey - 1;
                break;
            case Down:
                nextAppState.deletey = currentAppState -> deletey + 1;
                break;
            case Left:
                nextAppState.deletex = currentAppState -> deletex - 1;
                break;
            case Right:
                nextAppState.deletex = currentAppState -> deletex + 1;
                break;
        }
        nextAppState.pathCounter++;
        nextAppState.pathCounter = nextAppState.pathCounter % MAX_SIZE;
    } else {
        nextAppState.stopDelete--;
        nextAppState.pathCounter++;
    }



    if ((nextAppState.snakex < 0 || nextAppState.snakex > WIDTH - 4 || nextAppState.length > MAX_PATH_INDEX)
        || (nextAppState.snakey < 0 || nextAppState.snakey > HEIGHT - 4)) {
            nextAppState.gameOver = 1;
    }
    switch (nextAppState.direction) {
        case Up:
            if (*(VIDEOBUFFER + OFFSET(nextAppState.snakey, nextAppState.snakex, WIDTH)) == 0x3fe0) {
               nextAppState.gameOver = 1; 
            }
            break;
        case Down:
            if (*(VIDEOBUFFER + OFFSET(nextAppState.snakey + 4, nextAppState.snakex, WIDTH)) == 0x3fe0) {
               nextAppState.gameOver = 1; 
            }
            break;
        case Left:
            if (*(VIDEOBUFFER + OFFSET(nextAppState.snakey, nextAppState.snakex, WIDTH)) == 0x3fe0) {
               nextAppState.gameOver = 1; 
            }
            break;
        case Right:
            if (*(VIDEOBUFFER + OFFSET(nextAppState.snakey, nextAppState.snakex + 4, WIDTH)) == 0x3fe0) {
               nextAppState.gameOver = 1; 
            }
            break;
    }


    return nextAppState;
}
