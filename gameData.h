#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "tigr.h"
#include "miniaudio.h"
#include "audio.h"
#include <stdbool.h>

#define CELL_SIZE 25
#define GRID_HIGHT 40
#define GRID_WIDTH 40
#define WINDOW_HIGHT (CELL_SIZE*GRID_WIDTH)
#define WINDOW_WIDTH (CELL_SIZE*GRID_WIDTH)

typedef enum {
    MENU,
    SINGLE_PLAYER,
    MULTI_PLAYER,
    CHOOS_COLOR,
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
    float specailEffectDuration;
    float timer;
} Snake;

typedef struct {
    Position objPosition;
    bool eaten;
    float spawnTime;
    float lifeTime;
} Object;

typedef struct {
    Snake snake1, snake2;
    Object food, boom, specialFood;
    GameState gameState;
    int dir1, dir2;
    AudioSystem audio, foodAudio, boomAudio, specialFoodAudio, gameOverAudio;
    bool bmg_play, bmg_food_play, bmg_boom_play, bmg_specialFood_play, bmg_gameOver_play;
    
    float deltaTime;
    bool multiplayer;
    float boomSpawnTimer;
    float specialFoodspawnTimer;
    float originalDelay;
} Game;

#endif