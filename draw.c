#include "draw.h"
#include "gba.h"
#include "stdio.h"


void fullDrawAppState(AppState *state) {
    drawAppState(state);
}

void undrawAppState(AppState *state) {
    drawRectDMA(state -> foodx, state -> foody, 4, 4, BLACK);
    drawRectDMA(10, 10, 14, 14, BLACK);
}

// This function will be used to draw things that might have moved in a frame.
// draw the snake, the food, the score.
void drawAppState(AppState *state) {
    char pnts[3];
    sprintf(pnts, "%d", state -> points);
    drawString(10, 10, pnts , WHITE);

    drawRectDMA(state -> foodx, state -> foody, 4, 4, 0x381f);

    drawRectDMA(state -> snakex, state -> snakey, 4, 4, 0x3fe0);
    drawRectDMA(state -> deletex, state -> deletey, 4, 4, BLACK);
    
}
