#include "gba.h"
#include "logic.h"
#include "draw.h"
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>


int main(void) {
    // Manipulate REG_DISPCNT to set Mode 3.
    REG_DISPCNT = MODE3 | BG2_ENABLE;
    GBAState state = START;

    // store the "previous" and "current" states.
    AppState currentAppState, nextAppState;

    // store the current and previous values of the button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    int animationCount = 0;
    while (1) {
        // Load the current state of the buttons
        currentButtons = BUTTONS;

        switch(state) {
            case START:
                // Wait for VBlank
                waitForVBlank();

                fillScreenDMA(BLACK);
                drawString(69, 95, "Press A to Play", WHITE);
                state = START_NODRAW;
                break;
            case START_NODRAW:
                currentButtons = BUTTONS;

                const u16 *home[3];
                home[0] = snekW;
                home[1] = snekP;
                home[2] = snekG;
                waitForVBlank();
                drawImageDMA(63, 30, 109, 49, home[animationCount/100]);
                if (animationCount < 300) {
                    animationCount++;
                } else {
                    animationCount = 0;
                }

                if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
                    state = APP_INIT;
                }
                previousButtons = BUTTONS;
                break;
            case APP_INIT:
                // Initialize the app. Switch to the APP state.
                fillScreenDMA(BLACK);
                initializeAppState(&currentAppState);

                // Draw the initial state of the app
                fullDrawAppState(&currentAppState);

                state = APP;
                break;
            case APP:
                // Process the app for one frame, store the next state
                currentButtons = BUTTONS;
                nextAppState = processAppState(&currentAppState, previousButtons, currentButtons);

                // Wait for VBlank before we do any drawing.
                waitForVBlank();

                // Undraw the previous state
                undrawAppState(&currentAppState);

                // Draw the current state
                drawAppState(&nextAppState);

                // Now set the current state as the next state for the next iter.
                currentAppState = nextAppState;

                // Check if the app is exiting. If it is, then go to the exit state.
                if (nextAppState.gameOver) {
                    state = APP_EXIT;
                }
                if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                    state = START;
                    free(currentAppState.path);
                }
                previousButtons = BUTTONS;
                break;
            case APP_EXIT:
                // Wait for VBlank
                waitForVBlank();

                //Draw the gameover screen
                drawFullScreenImageDMA(gameOver);
                drawString(69, 140, "Press A to Restart", WHITE);

                state = APP_EXIT_NODRAW;
                previousButtons = BUTTONS;
                break;
            case APP_EXIT_NODRAW:
                // Check for a button press here to go back to the start screen
                currentButtons = BUTTONS;
                if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
                    free(currentAppState.path);
                    state = START;
                } else if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                    free(currentAppState.path);
                    state = START;
                }
                previousButtons = BUTTONS;
                break;
        }

        // Store the current state of the buttons
        previousButtons = currentButtons;
    }

    return 0;
}

