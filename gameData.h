#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "tigr.h"
#include "miniaudio.h"
#include <stdbool.h>

#define CELL_SIZE 20
#define GRID_HIGHT 30
#define GRID_WIDTH 40
#define WINDOW_HIGHT (GRID_HIGHT*GRID_WIDTH)
#define WINDOW_WIDTH (GRID_HIGHT*GRID_WIDTH)

typedef enum {
    MENU,
    SINGLE_PLAYER,
    MULTI_PLAYER,
    GAME_OVER
} GameState;

typedef struct {
    int x; 
    int y;
} Position;

typedef struct {
    Position body[GRID_HIGHT* GRID_WIDTH];
    int length;
    int direction;
    int newDirection;
    TPixel color;
    int score;
    bool alive;
    bool moved;
    float delay;
} Snake;

typedef struct {
    Position objPosition;
    bool eaten;
} Object;

typedef struct{
    Snake snake1, snake2;
    Object food, boom, specialFood;
    GameState gameState;
    int dir1, dir2;
    AudioSystem audio;
    bool bmg_play;
    float timer;
    bool multiplayer;
    bool print;
} Game;

#endif