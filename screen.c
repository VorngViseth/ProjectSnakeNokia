#include <stdlib.h>

#include "screen.h"
#include "gameData.h"
#include "snake.h"

void drawGrid(Tigr* screen){
    for(int x = 0; x < WINDOW_WIDTH; x++) tigrLine(screen, x, 0, x, WINDOW_HIGHT, tigrRGB(30,30,30));
    for(int y = 0; y < WINDOW_HIGHT; y++) tigrLine(screen, 0, y, WINDOW_HIGHT, y, tigrRGB(30,30,30));
}

void drawFood(Tigr* screen, Object* food) {
    if(!food->eaten) {
        tigrFill(screen, food->objPosition.x*CELL_SIZE, food->objPosition.y*CELL_SIZE, CELL_SIZE, CELL_SIZE, tigrRGB(230,10,0));
    }
}

void drawSnake(Tigr*screen, Snake* snake);

void drawGame(Snake* snake1, Snake* snake2, Tigr* screen, Object* food, bool multiplayer) {
    drawGrid(screen);
    drawFood(screen, food);

}

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

void singlePlayer(Tigr* screen, Snake* snake1, Snake* snake2) {
    tigrClear(screen, tigrRGB(0,0,0));

}