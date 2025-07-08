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

    init_audio(&game->audio);
    snakeInit(&game->snake1, tigrRGB(0,200,0), WINDOW_WIDTH/2-5, WINDOW_HIGHT/2, &game->dir1);
    snakeInit(&game->snake2, tigrRGB(0,0,200), WINDOW_WIDTH/2+5, WINDOW_HIGHT/2, &game->dir2);
}

int main() {

    srand(time(NULL));

    Tigr* screen = tigrWindow(WINDOW_WIDTH, WINDOW_HIGHT, "PROJECT SNAKE NOKIA", 0);
    Game game;

    initGame(screen, &game);
    
    while(!tigrClosed(screen)){
        float deltaTime = tigrTime();

        if(!game.bmg_play) {
            play_bgm(&game.audio, "asset/SnakeNokiaSongTrack.mp3");
            game.bmg_play = true;
        }

        switch (game.gameState){
            case MENU :
                game.print = false;
                menuState(screen, &game.gameState);
            break;
            case SINGLE_PLAYER :
                if(!game.print){
                    printf("You are in single player\n");
                    game.print = true;
                }

            break;
            case MULTI_PLAYER :
                if(!game.print){
                    printf("You are in Multi player\n");
                    game.print = true;
                }
            break;
            case GAME_OVER :
            
            break;
        }

        tigrUpdate(screen);
    }

    return 0;
}