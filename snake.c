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

void snakeProperty(Snake* snake) {
    if(!snake->alive) return;

    snake->direction = snake->newDirection;
    snake->moved = true;

    for(int i = snake->length-1; i > 0; i++) snake->body[i] = snake->body[i+1];

    switch(snake->direction){
        case 0 : snake->body[0].y-- ;
        case 1 : snake->body[0].x++ ;
        
    }
}