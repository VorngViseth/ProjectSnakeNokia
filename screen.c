#include "screen.h"
#include "gameData.h"

void drawMenu(Tigr* screen){
    tigrClear(screen, tigrRGB(0,0,0));

    int textWidth = tigrTextWidth(tfont, "SNAKE NOKIA");
    tigrPrint(screen, tfont, (WINDOW_WIDTH-textWidth)/2, WINDOW_HIGHT/3, tigrRGB(0,255,0), "SNAKE NOKIA");
    
}

void menuState(Tigr* screen, GameState* gameState) {
    drawMenu(screen);
}