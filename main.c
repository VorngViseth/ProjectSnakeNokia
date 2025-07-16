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

int main() {

    Tigr* screen = tigrWindow(WINDOW_WIDTH, WINDOW_HIGHT, "PROJECT SNAKE NOKIA", TIGR_FIXED);
    Game game;

    init_audio(&game.audio);
    game.bmg_play = false;
    
    initGame(screen, &game);

    //default snake color
    game.snake1.color = tigrRGB(255, 0, 0);
    game.snake2.color = tigrRGB(0, 128, 128);
    
    while(!tigrClosed(screen)){
        game.deltaTime = tigrTime();

        if(!game.bmg_play){
            game.bmg_play = true;
            play_bgm(&game.audio, "asset/SnakeNokiaSongTrack.mp3");
        }

        switch (game.gameState){
            case MENU :
                menuState(screen, &game.gameState);
            break;
            case SINGLE_PLAYER :
                singlePlayer(&game, screen);
            break;
            case MULTI_PLAYER :
                multiplayer(&game, screen);
            break;
            case CHOOS_COLOR :
                chooseColor(&game, screen);
            break;
            case GAME_OVER :
                gameOver(&game, screen);
            break;
        }

        tigrUpdate(screen);
    }

    cleanUpGame(&game, screen);
    return 0;
}