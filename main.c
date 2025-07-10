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

    initGame(screen, &game);
    
    while(!tigrClosed(screen)){
        game.deltaTime = tigrTime();


        switch (game.gameState){
            case MENU :
                game.print = false;
                menuState(screen, &game.gameState);
            break;
            case SINGLE_PLAYER :
                singlePlayer(&game, screen);
            break;
            case MULTI_PLAYER :
                multiplayer(&game, screen);
            break;
            case CHOOS_COLOR :

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