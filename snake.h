#ifndef SNAKE_H
#define SNAKE_H

#include "tigr.h"
#include "gameData.h"

// user input mmove
void move(Tigr* screen, Snake* snake1, Snake* snake2);

// initialize snake on the grid
void snakeInit(Snake* snake, TPixel color, int x, int y, int* dir);

void snakeProperty(Snake* snake);

void checkCollition(Snake* snake1, Snake* snake2, bool* multiplayer);

#endif