#include <stdlib.h>

#include "screen.h"
#include "gameData.h"

void drawMenu(Tigr* screen){
    tigrClear(screen, tigrRGB(0,0,0));

    int textWidth = tigrTextWidth(tfont, "SNAKE NOKIA");
    tigrPrint(screen, tfont, (WINDOW_WIDTH-textWidth)/2, WINDOW_HIGHT/3, tigrRGB(0,255,0), "SNAKE NOKIA");

    textWidth = tigrTextWidth(tfont, "1. Single Player");
    tigrPrint(screen, tfont, (WINDOW_WIDTH-textWidth)/2, WINDOW_HIGHT/2, tigrRGB(0,200,100), "1. Single Player");

    textWidth = tigrTextWidth(tfont, "2. Multi Player");
    tigrPrint(screen, tfont, (WINDOW_WIDTH-textWidth)/2, WINDOW_HIGHT/2+40, tigrRGB(0,200,100), "2. Multi Player");

    textWidth = tigrTextWidth(tfont, "click esc to exit the game");
    tigrPrint(screen, tfont, (WINDOW_WIDTH-textWidth)/2, WINDOW_HIGHT/2+80, tigrRGB(200,200,100), "click esc to exit the game");
}

void menuState(Tigr* screen, GameState* gameState) {
    drawMenu(screen);

    if(tigrKeyDown(screen, '1')) *gameState = SINGLE_PLAYER;
    else if(tigrKeyDown(screen, '2')) *gameState = MULTI_PLAYER;
    else if(tigrKeyDown(screen, TK_ESCAPE)) {
        tigrFree(screen);
        exit(0);
    } 
}