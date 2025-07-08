#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "screen.h"
#include "gameData.h"
#include "snake.h"
#include "object.h"

void drawBorder(Tigr* screen){
    TPixel borderColor = tigrRGB(255, 20, 20);

    // Top
    tigrLine(screen, 0, 0, WINDOW_WIDTH, 0, borderColor);
    // Bottom
    tigrLine(screen, 0, WINDOW_HIGHT - 1, WINDOW_WIDTH, WINDOW_HIGHT - 1, borderColor);
    // Left
    tigrLine(screen, 0, 0, 0, WINDOW_HIGHT, borderColor);
    // Right
    tigrLine(screen, WINDOW_WIDTH - 1, 0, WINDOW_WIDTH - 1, WINDOW_HIGHT, borderColor);
}

void drawGrid(Tigr* screen){
    for(int x = 0; x < WINDOW_WIDTH; x += CELL_SIZE) tigrLine(screen, x, 0, x, WINDOW_HIGHT, tigrRGB(30,30,30));
    for(int y = 0; y < WINDOW_HIGHT; y += CELL_SIZE) tigrLine(screen, 0, y, WINDOW_WIDTH, y, tigrRGB(30,30,30));
}

void drawObject(Tigr* screen, Object* object, TPixel objColor) {
    if(!object->eaten) {
        tigrFill(screen, object->objPosition.x*CELL_SIZE, object->objPosition.y*CELL_SIZE, CELL_SIZE-1, CELL_SIZE-1, objColor);
    }
}

void drawSnake(Tigr*screen, Snake* snake){
    if(snake->alive){
        for(int i = 0; i < snake->length; i++) {
            TPixel color = (i==0) ? tigrRGB(0,255,0) : snake->color;
            tigrFill(screen, snake->body[i].x*CELL_SIZE, snake->body[i].y*CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
        }
    }
}

void drawScoreBoard(Tigr* screen, Snake* snake, int x, int y) {
    char scoreText[32] ;
    snprintf(scoreText, sizeof(scoreText), "Score : %d", (snake->score>=0) ? snake->score : 0);
    tigrPrint(screen, tfont, x, y, tigrRGB(255,255,255), scoreText);
}

void drawGame(Tigr* screen, Game* game) {
    drawGrid(screen);
    drawBorder(screen);

    drawObject(screen, &game->food, tigrRGB(255,50,50));
    if(&game->boom != NULL) drawObject(screen, &game->boom, tigrRGB(50,50,50));
    if(&game->specialFood != NULL) drawObject(screen, &game->specialFood, tigrRGB(100,100,250));

    if(&game->snake1.alive) drawSnake(screen, &game->snake1);
    if(&game->snake2 && game->multiplayer && &game->snake2.alive) drawSnake(screen, &game->snake2);

    drawScoreBoard(screen, &game->snake1, 10, 10);
    if(&game->snake2 && game->multiplayer && &game->snake2.alive) drawScoreBoard(screen, &game->snake2, GRID_WIDTH-60, 10);
}

// below are all the main functions

void menuState(Tigr* screen, GameState* gameState) {
    tigrClear(screen, tigrRGB(0,0,0));

    Tigr* menuPicture = tigrLoadImage("asset/SnakeMenu.png");
    
    int x = (screen->w - menuPicture->w) / 2;
    int y = (screen->h - menuPicture->h) / 2;

    tigrBlit(screen, menuPicture, x, y, 0, 0, menuPicture->w, menuPicture->h);

    if(tigrKeyDown(screen, '1')) *gameState = SINGLE_PLAYER;
    else if(tigrKeyDown(screen, '2')) *gameState = MULTI_PLAYER;
    else if(tigrKeyDown(screen, TK_ESCAPE)) {
        tigrFree(screen);
        exit(0);
    } 

    tigrFree(menuPicture);
}

void singlePlayer(Game* game, Tigr* screen) {
    // for spawning boom and food at a random time during the game
    game->boomSpawnTimer -= game->deltaTime;
    game->specialFoodspawnTimer -= game->deltaTime;

    // for counting time per frame to set how fast the snake can move per frame
    game->timer += game->deltaTime;

    // counting how long since the obejct is spanwed 
    if(!game->boom.eaten) game->boom.spawnTime += game->deltaTime;
    if(!game->specialFood.eaten) game->specialFood.spawnTime += game->deltaTime;

    // delete the object for respawning after the snake did't eat the object (the object won't be drawn if eaten is true)
    if(!game->boom.eaten && game->boom.spawnTime >= game->boom.lifeTime) game->boom.eaten = true;
    if(!game->specialFood.eaten && game->specialFood.spawnTime >= game->specialFood.lifeTime) game->specialFood.eaten = true;

    // the snake can move if the timer = delay (the snake speed)
    if(game->timer >= game->snake1.delay) {
        game->timer = 0;

        tigrClear(screen, tigrRGB(0,0,0)); // clear <-----
        
        game->multiplayer = false;

        placeObject(game);

        move(screen, &game->snake1, NULL);
        snakeProperty(&game->snake1);
        checkCollition(&game->snake1, NULL, &game->multiplayer);

        if(!game->snake1.alive) game->gameState = GAME_OVER;

        eatFood(&game->food, &game->snake1);
        eatBoom(game, &game->boom, &game->snake1);
        eatSpecialFood(&game->specialFood, &game->snake1);

        specialEffectCountDown(game, &game->snake1);

        drawGame(screen, game);
    }

}