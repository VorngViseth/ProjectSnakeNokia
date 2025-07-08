#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tigr.h"
#include "miniaudio.h"
#include "audio.h"
#include "gameData.h"
#include "snake.h"
#include "screen.h"

void initGame(Tigr* screen, Game* game){
    game->gameState = MENU;

    game->dir1 = 0;
    game->dir2 = 0;

    game->bmg_play = false;
    game->timer = 0;
    game->multiplayer = false; 
    game->print = false;

    game->food.eaten = true;
    game->boom.eaten = true;
    game->specialFood.eaten = true;
    
    srand(time(NULL));  
    game->boomSpawnTimer = rand()%20 + 10; // 10 - 29 secs
    game->specialFoodspawnTimer = rand()%20 + 10; // 10-29 secs

    init_audio(&game->audio);

    int centerX = GRID_WIDTH / 2;
    int centerY = GRID_HIGHT / 2;

    snakeInit(&game->snake1, tigrRGB(0,200,0), centerX - 5, centerY, &game->dir1);
    snakeInit(&game->snake2, tigrRGB(0,0,200), centerX + 5, centerY, &game->dir2);

    game->originalDelay = game->snake1.delay;
}

void changeBGMIfNeeded(Game* game) {
    static GameState lastState = -1;

    if (game->gameState != lastState) {
        stop_bgm(&game->audio);  // Stop current music

        const char* track = (game->gameState == GAME_OVER)
            ? "asset/snakeGameOver.mp3"
            : "asset/SnakeNokiaSongTrack.mp3";

        play_bgm(&game->audio, track);
        lastState = game->gameState;
    }
}

int main() {

    Tigr* screen = tigrWindow(WINDOW_WIDTH, WINDOW_HIGHT, "PROJECT SNAKE NOKIA", TIGR_FIXED);
    Game game;

    initGame(screen, &game);
    
    while(!tigrClosed(screen)){
        game.deltaTime = tigrTime();

        changeBGMIfNeeded(&game);

        switch (game.gameState){
            case MENU :
                game.print = false;
                menuState(screen, &game.gameState);
            break;
            case SINGLE_PLAYER :
                // singlePlayer(&game, &game.gameState ,screen, &game.snake1, NULL, &game.food, &game.boom, &game.specialFood,&game.multiplayer);                
                singlePlayer(&game, screen);
            break;
            case MULTI_PLAYER :
                if(!game.print){
                    printf("gamestate : %d\n", game.gameState);
                    printf("You are in Multi player\n");
                    game.print = true;
                }
            break;
            case GAME_OVER :
                gameOver(&game, screen);
            break;
        }

        tigrUpdate(screen);
    }

    shutdown_audio(&game.audio);
    return 0;
}