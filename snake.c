#include "tigr.h"
#include  "snake.h"
#include "gameData.h"
#include <time.h>
#include <stdbool.h>

void wallCollision(Snake* snake) {
    if(snake->body[0].x < 0 || snake->body[0].x >= GRID_WIDTH ||
        snake->body[0].y < 0 || snake->body[0].y >= GRID_HIGHT) {
        snake->alive = false;
        return;
    }   
}

void bodyCollision(Snake* snake) {
    for(int i = 1; i < snake->length; i++) {
        if(snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y) {
            snake->alive = false; 
            return;
        }
    }
}

void snakeCollision(Snake* snake, Snake* otherSnake) {
    for(int i = 1; i < snake->length; i++) {
        if(snake->body[0].x == otherSnake->body[i].x && snake->body[0].y == otherSnake->body[i].y) {
            snake->alive = false;
            return;
        }
    }
}

void head2HeadCollision(Snake* snake1, Snake* snake2) {
    if(snake1->body[0].x == snake2->body[0].x && snake1->body[0].y == snake2->body[0].y) {
        snake1->alive = false;
        snake2->alive = false;
    }
}

void checkCollition(Snake* snake1, Snake* snake2, bool* multiplayer) {
    if(!snake1->moved) return;

    if(*multiplayer == false) {
        wallCollision(snake1);
        bodyCollision(snake1);        
    }else {
        head2HeadCollision(snake1, snake2);        
        if(snake1->alive) wallCollision(snake1);
        if(snake2->alive) wallCollision(snake2);
        if(snake1->alive && snake2->alive) {
            snakeCollision(snake1, snake2);
            snakeCollision(snake2, snake1);           
        }
    }
}

void move(Tigr* screen, Snake* snake1, Snake* snake2) {
    // up = 0, down = 2, left = 3, right = 1

    // move snake 1 : WASD  
    if(tigrKeyHeld(screen, 'W') && snake1->direction != 2) snake1->newDirection = 0;
    if(tigrKeyHeld(screen, 'D') && snake1->direction != 3) snake1->newDirection = 1; 
    if(tigrKeyHeld(screen, 'S') && snake1->direction != 0) snake1->newDirection = 2;
    if(tigrKeyHeld(screen, 'A') && snake1->direction != 1) snake1->newDirection = 3;

    if(snake2 != NULL) {
        // move snake 2 : arrow keys  
        if(tigrKeyHeld(screen, TK_UP) && snake2->direction != 2) snake2->newDirection = 0;
        if(tigrKeyHeld(screen, TK_RIGHT) && snake2->direction != 3) snake2->newDirection = 1;
        if(tigrKeyHeld(screen, TK_DOWN) && snake2->direction != 0) snake2->newDirection = 2;
        if(tigrKeyHeld(screen, TK_LEFT) && snake2->direction != 1) snake2->newDirection = 3;

    }
}

void snakeInit(Snake* snake, TPixel color, int x, int y, int* dir){
    snake->length = 3;
    snake->score = 0;
    snake->color = color;
    snake->alive = true;
    snake->moved = false;
    snake->direction = *dir;
    snake->newDirection = *dir;
    snake->delay = 0.1f;
    snake->specailEffectDuration = 0.0f;

    // snake face up and the body is extending down at the start
    for(int i = 0; i < snake->length; i++){
        snake->body[i].x = x;
        snake->body[i].y = y + i;   
    }
}

void snakeProperty(Snake* snake) {
    if(!snake->alive) return;

    snake->direction = snake->newDirection;

    for(int i = snake->length-1; i > 0; i--) snake->body[i] = snake->body[i-1];

    switch(snake->direction){
        case 0 : snake->body[0].y-- ; break;
        case 1 : snake->body[0].x++ ; break;
        case 2 : snake->body[0].y++ ; break;
        case 3 : snake->body[0].x-- ; break;
    }

    snake->moved = true;
}

