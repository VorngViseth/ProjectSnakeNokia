#include "tigr.h"
#include  "snake.h"
#include "gameData.h"

void snakeInit(Snake* snake, TPixel color, int* x, int* y, int* dir){
    snake->length = 3;
    snake->score = 0;
    snake->color = color;
    snake->alive = true;
    snake->moved = false;
    snake->direction = dir;
    snake->newDirection = dir;

    // snake face up and the body is extending down at the start
    for(int i = 0; i < snake->length; i++){
        snake->body[i].x = x;
        snake->body[i].y = y + i;
    }
}