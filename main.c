#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "tigr.h"
#include "miniaudio.h"
#include "audio.h"
#include "gameData.h"
#include "snake.h"
#include "screen.h"

int main() {
    srand(time(NULL));
    Tigr* screen = tigrWindow(WINDOW_WIDTH, WINDOW_HIGHT, "PROJECT SNAKE NOKIA", 0);
    GameState gameState = MENU;
    Snake snake1, snake2;
    Object food, boom, specialFood;
    int dir1 = snake1.direction;
    int dir2 = snake2.direction;

    AudioSystem audio;
    init_audio(&audio);
    bool bmg_play = false;

    float timer = 0;
    const float delay = 0.1f;
    bool multiplayer = false; 

    snakeInit(&snake1, tigrRGB(0,200,0), WINDOW_WIDTH/2-5, WINDOW_HIGHT/2, &dir1);
    snakeInit(&snake2, tigrRGB(0,0,200), WINDOW_WIDTH/2+5, WINDOW_HIGHT/2, &dir2);
    
    while(!tigrClosed(screen)){
        float deltaTime = tigrTime();

        if(!bmg_play) {
            play_bgm(&audio, "asset/SnakeNokiaSongTrack.mp3");
            bmg_play = true;
        }

        switch (gameState){
            case MENU :
                menuState(screen, &gameState);
            break;
            case SINGLE_PLAYER :

            break;
            case MULTI_PLAYER :

            break;
            case GAME_OVER :
            
            break;
        }

        tigrUpdate(screen);
    }

    return 0;
}