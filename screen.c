#include <stdlib.h>

#include "screen.h"
#include "gameData.h"

void menuState(Tigr* screen, GameState* gameState) {
    tigrClear(screen, tigrRGB(0,0,0));

    Tigr* menuPicture = tigrLoadImage("asset/SnakeMenu.png");
    
    int x = (screen->w - menuPicture->w) / 2;
    int y = (screen->h - menuPicture->h) / 2;

    tigrBlit(screen, menuPicture, x, y, 0, 0, menuPicture->w, menuPicture->h);

    if(tigrKeyDown(screen, '1')) *gameState = SINGLE_PLAYER;
    else if(tigrKeyDown(screen, '2')) *gameState = MULTI_PLAYER;
    else if(tigrKeyDown(screen, TK_ESCAPE)) {
        tigrFree(screen);
        exit(0);
    } 

    tigrFree(menuPicture);
}