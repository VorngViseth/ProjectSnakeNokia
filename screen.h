#ifndef SCREEN_H
#define SCREEN_H

#include <time.h>
#include "tigr.h"
#include "gameData.h"

void menuState(Tigr* screen, GameState* gameState);

// void singlePlayer(Game* game, GameState* gameState, Tigr* screen, Snake* snake1, Snake* snake2, Object* food, Object* boom, Object* specialFood, bool* multiplayer);
void singlePlayer(Game* game, Tigr* screen);
// void singlePlayer(Game* game, GameState* gameState, Tigr* screen, Snake* snake1, Snake* snake2, Object* food, Object* boom, Object* specialFood, bool* multiplayer);
void multiplayer(Game* game , Tigr* screen);

void chooseColor(Game* game, Tigr* screen);

void gameOver(Game* game, Tigr* screen);

void initGame(Tigr* screen, Game* game);

void cleanUpGame(Game* game, Tigr* screen);

#endif