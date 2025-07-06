#include <stdio.h>
#include <time.h>
#include "tigr.h"
#include "miniaudio.h"
#include "audio.h"
#include "gameData.h"
#include "snake.h"

int main() {
    srand(time(NULL));
    Tigr* screen = tigrWindow(WINDOW_WIDTH, WINDOW_HIGHT, "PROJECT SNAKE NOKIA", 0);
    GameState gameState = MENU;
    Snake snake1, snake2;
    Object food, boom, specialFood;

    float timer = 0;
    const float delay = 0.1f;
    bool multiplayer = false; 

    snakeInit(&snake1, tigrRGB(0,200,0), WINDOW_WIDTH/2-5, WINDOW_HIGHT/2, 0);
    snakeInit(&snake2, tigrRGB(0,0,200), WINDOW_WIDTH/2+5, WINDOW_HIGHT/2, 0);
    
    while(!tigrClosed(screen)){
        float deltaTime = tigrTime();

        switch (gameState){
            case MENU :

            break;
            case SINGLE_PLAYER :

            break;
            case MULTI_PLAYER :

            break;
            case GAME_OVER :
            
            break;
        }
    }
    
    return 0;
}