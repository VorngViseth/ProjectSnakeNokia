#include "object.h"
#include "gameData.h"
#include "tigr.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

void initObject(Object* object, Snake* snake1, Snake* snake2) {
    bool valid = false;
    while(!valid) {
        valid = true; 
        object->objPosition.x = rand() % GRID_WIDTH;
        object->objPosition.y = rand() % GRID_HIGHT;

        for(int i = 0; i < snake1->length; i++) {
            if(object->objPosition.x == snake1->body[i].x && object->objPosition.y == snake1->body[i].y) {
                valid = false;
                break;
            }
        }

        if(snake2 != NULL && snake2->alive) {
            for(int i = 0; i < snake2->length; i++) {
                if(object->objPosition.x == snake2->body[i].x && object->objPosition.y == snake2->body[i].y) {
                    valid = false;
                    break;
                }
            }
        }
    }

    object->eaten = false;
}

void eatFood(Object* food, Snake* snake) {
    if(snake->body[0].x == food->objPosition.x && snake->body[0].y == food->objPosition.y){
        snake->length++ ;
        snake->score++ ;
        food->eaten = true;
    } 
}

void eatBoom(Object* boom, Snake* snake) {
    if(snake->body[0].x == boom->objPosition.x && snake->body[0].y == boom->objPosition.y){
        snake->score-- ;
        boom->eaten = true;
    } 
}

void eatSpecialFood(Object* specialFood, Snake* snake) {
    if(snake->body[0].x == specialFood->objPosition.x && snake->body[0].y == specialFood->objPosition.y){
        snake->delay = 0.05f ;
        specialFood->eaten = true;
    } 
}