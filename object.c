#include "object.h"
#include "gameData.h"
#include "tigr.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

void initObject(Object* object, Game* game) {
    bool valid = false;
    while(!valid) {
        valid = true; 
        object->objPosition.x = rand() % GRID_WIDTH;
        object->objPosition.y = rand() % GRID_HIGHT;

        for(int i = 0; i < game->snake1.length; i++) {
            if(object->objPosition.x == game->snake1.body[i].x && object->objPosition.y == game->snake1.body[i].y) {
                valid = false;
                break;
            }
        }

        if(&game->snake2 != NULL && &game->snake2.alive) {
            for(int i = 0; i < game->snake2.length; i++) {
                if(object->objPosition.x == game->snake2.body[i].x && object->objPosition.y == game->snake2.body[i].y) {
                    valid = false;
                    break;
                }
            }
        }

        // prevent from overlaping to other objects

        if (valid && object != NULL && !object->eaten &&
            object->objPosition.x == game->food.objPosition.x &&
            object->objPosition.y == game->food.objPosition.y) valid = false; 

        if (valid && &game->boom != NULL && !game->boom.eaten &&
            object->objPosition.x == game->boom.objPosition.x &&
            object->objPosition.y == game->boom.objPosition.y) valid = false; 

        if (valid && &game->specialFood != NULL && !game->specialFood.eaten &&
            object->objPosition.x == game->specialFood.objPosition.x &&
            object->objPosition.y == game->specialFood.objPosition.y) valid = false;    
    }

    object->spawnTime = 0;
    object->lifeTime = rand()%10 + 5; // 5-14 sec
    object->eaten = false;
}

void placeObject(Game* game) {
    if(game->food.eaten) initObject(&game->food, game);

    if(game->boomSpawnTimer <= 0 && &game->boom.eaten) {
        initObject(&game->boom, game);
        game->boomSpawnTimer = rand()%20 + 10;
    }

    if(game->specialFoodspawnTimer <= 0 && &game->specialFood.eaten){
        initObject(&game->specialFood, game);
        game->specialFoodspawnTimer = rand()%20 + 10;
    }
}

void eatFood(Object* food, Snake* snake) {
    if(snake->body[0].x == food->objPosition.x && snake->body[0].y == food->objPosition.y){
        snake->length++ ;
        snake->score++ ;
        food->eaten = true;
    } 
}

void eatBoom(Game* game, Object* boom, Snake* snake) {
    if(snake->body[0].x == boom->objPosition.x && snake->body[0].y == boom->objPosition.y){
        snake->score -= 3;
        snake->length -= 3;
        boom->eaten = true;
        if(snake->length < 3 && snake->score < 0) {
            snake->alive = false;
            game->gameState = GAME_OVER;
        }
    } 
}

void eatSpecialFood(Object* specialFood, Snake* snake) {
    if(snake->body[0].x == specialFood->objPosition.x && snake->body[0].y == specialFood->objPosition.y){
        specialFood->eaten = true;
        snake->score++;
        snake->delay = 0.03f ;
        snake->specailEffectDuration = 5.0f;
    } 
}

void specialEffectCountDown(Game* game, Snake* snake) {
    if(snake->specailEffectDuration > 0.0f) {
        snake->specailEffectDuration -= game->deltaTime;
        if(snake->specailEffectDuration <= 0.0f) snake->delay = game->originalDelay;
    }
}