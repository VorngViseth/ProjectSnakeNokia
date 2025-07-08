#ifndef OBJECT_H
#define OBJECT_H

#include "tigr.h"
#include "gameData.h"

void placeObject(Game* game);

void eatFood(Object* food, Snake* snake);

void eatBoom(Game* game, Object* boom, Snake* snake);

void eatSpecialFood(Object* specialFood, Snake* snake);

void specialEffectCountDown(Game* game, Snake* snake);

#endif