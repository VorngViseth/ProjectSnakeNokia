#ifndef OBJECT_H
#define OBJECT_H

#include "tigr.h"
#include "gameData.h"

void placeObject(Game* game);

void eatFood(Object* food, Snake* snake, Game* game);

void eatBoom(Game* game, Object* boom, Snake* snake);

void eatSpecialFood(Object* specialFood, Snake* snake, Game* game);

void specialEffectCountDown(Game* game, Snake* snake);

#endif