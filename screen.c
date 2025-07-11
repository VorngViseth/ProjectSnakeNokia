#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "screen.h"
#include "gameData.h"
#include "snake.h"
#include "object.h"

void drawGameOver(Tigr* screen, Game* game) {
    char gameOverText[64];
    if (game->multiplayer) {
        if (!game->snake1.alive && !game->snake2.alive) {
            snprintf(gameOverText, sizeof(gameOverText), "DRAW! Scores: %d-%d", game->snake1.score, game->snake2.score);
        } else if (!game->snake1.alive) {
            snprintf(gameOverText, sizeof(gameOverText), "PLAYER 2 WINS! %d-%d", game->snake1.score, game->snake2.score);
        } else {
            snprintf(gameOverText, sizeof(gameOverText), "PLAYER 1 WINS! %d-%d", game->snake1.score, game->snake2.score);
        }
    } else {
        snprintf(gameOverText, sizeof(gameOverText), "GAME OVER! Score: %d", game->snake1.score);
    }
    
    int tw = tigrTextWidth(tfont, gameOverText);
    tigrPrint(screen, tfont, (WINDOW_WIDTH-tw)/2, WINDOW_HIGHT/2 - 30, 
             tigrRGB(255, 50, 50), gameOverText);
    
    const char* restartText = "Press SPACE to return to menu";
    tw = tigrTextWidth(tfont, restartText);
    tigrPrint(screen, tfont, (WINDOW_WIDTH-tw)/2, WINDOW_HIGHT/2 + 10,
             tigrRGB(250, 50, 50), restartText);

    const char* exitText = "Press esc to exit the game";
    tw = tigrTextWidth(tfont, exitText);
    tigrPrint(screen, tfont, (WINDOW_WIDTH-tw)/2, WINDOW_HIGHT/2 + 60,
                tigrRGB(250,50,50), exitText);
}

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

void drawSnake(Tigr* screen, Snake* snake) {
    if (snake->alive) {
        for (int i = 0; i < snake->length; i++) {
            TPixel color;
            if (i == 0) {
                // Make head color a brighter version of the body color
                color.r = (snake->color.r + 100 > 255) ? 255 : snake->color.r + 100;
                color.g = (snake->color.g + 100 > 255) ? 255 : snake->color.g + 100;
                color.b = (snake->color.b + 100 > 255) ? 255 : snake->color.b + 100;
                color.a = 255;
            } else {
                color = snake->color;
            }
            tigrFill(screen, snake->body[i].x * CELL_SIZE, snake->body[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
        }
    }
}

void drawScoreBoard(Tigr* screen, Snake* snake, int x, int y) {
    char scoreText[32] ;
    snprintf(scoreText, sizeof(scoreText), "Score : %d", (snake->score>=0) ? snake->score : 0);
    tigrPrint(screen, tfont, x, y, snake->color, scoreText);
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
    if(&game->snake2 && game->multiplayer && &game->snake2.alive) drawScoreBoard(screen, &game->snake2, 37*CELL_SIZE, 10);
}

void initGame(Tigr* screen, Game* game ){
    game->gameState = MENU;

    game->dir1 = 0;
    game->dir2 = 0;

    game->snake1.timer = 0;
    game->snake2.timer = 0;
    game->multiplayer = false; 

    game->food.eaten = true;
    game->boom.eaten = true;
    game->specialFood.eaten = true;
    
    srand(time(NULL));  
    game->boomSpawnTimer = rand()%20 + 10; // 10 - 29 secs
    game->specialFoodspawnTimer = rand()%20 + 10; // 10-29 secs

    int centerX = GRID_WIDTH / 2;
    int centerY = GRID_HIGHT / 2;

    snakeInit(&game->snake1, game->color1, centerX - 5, centerY, &game->dir1);
    snakeInit(&game->snake2, game->color2, centerX + 5, centerY, &game->dir2);

    game->originalDelay = game->snake1.delay;
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
    else if(tigrKeyDown(screen, '3')) *gameState = CHOOS_COLOR;
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
    game->snake1.timer += game->deltaTime;

    // counting how long since the obejct is spanwed 
    if(!game->boom.eaten) game->boom.spawnTime += game->deltaTime;
    if(!game->specialFood.eaten) game->specialFood.spawnTime += game->deltaTime;

    // delete the object for respawning after the snake did't eat the object (the object won't be drawn if eaten is true)
    if(!game->boom.eaten && game->boom.spawnTime >= game->boom.lifeTime) game->boom.eaten = true;
    if(!game->specialFood.eaten && game->specialFood.spawnTime >= game->specialFood.lifeTime) game->specialFood.eaten = true;

    // the snake can move if the timer = delay (the snake speed)
    if(game->snake1.timer >= game->snake1.delay) {
        game->snake1.timer = 0;

        tigrClear(screen, tigrRGB(0,0,0)); // clear <-----
        
        game->multiplayer = false;

        placeObject(game);

        move(screen, &game->snake1, NULL);

        if(!game->snake1.alive) game->gameState = GAME_OVER;

        eatFood(&game->food, &game->snake1);
        eatBoom(game, &game->boom, &game->snake1);
        eatSpecialFood(&game->specialFood, &game->snake1);

        snakeProperty(&game->snake1);

        specialEffectCountDown(game, &game->snake1);

        checkCollition(&game->snake1, NULL, &game->multiplayer);

        drawGame(screen, game);
    }

}

void multiplayer(Game* game, Tigr* screen) {
    // for spawning boom and food at a random time during the game
    game->boomSpawnTimer -= game->deltaTime;
    game->specialFoodspawnTimer -= game->deltaTime;

    // for counting time per frame to set how fast the snake can move per frame
    game->snake1.timer += game->deltaTime;
    game->snake2.timer += game->deltaTime;

    // counting how long since the obejct is spanwed 
    if(!game->boom.eaten) game->boom.spawnTime += game->deltaTime;
    if(!game->specialFood.eaten) game->specialFood.spawnTime += game->deltaTime;

    // delete the object for respawning after the snake did't eat the object (the object won't be drawn if eaten is true)
    if(!game->boom.eaten && game->boom.spawnTime >= game->boom.lifeTime) game->boom.eaten = true;
    if(!game->specialFood.eaten && game->specialFood.spawnTime >= game->specialFood.lifeTime) game->specialFood.eaten = true;

    tigrClear(screen, tigrRGB(0,0,0)); // clear <-----
    game->multiplayer = true;
    // the snake can move if the timer = delay (the snake speed)
    if(game->snake1.timer >= game->snake1.delay ) {
        game->snake1.timer = 0;

        placeObject(game);

        eatFood(&game->food, &game->snake1);
        
        eatBoom(game, &game->boom, &game->snake1);
        
        eatSpecialFood(&game->specialFood, &game->snake1);

        snakeProperty(&game->snake1);
        
        specialEffectCountDown(game, &game->snake1);
        
    }
    if(game->snake2.timer >= game->snake2.delay) {
        
        game->snake2.timer = 0;

        placeObject(game);

        eatFood(&game->food, &game->snake2);
        
        eatBoom(game, &game->boom, &game->snake2);
        
        eatSpecialFood(&game->specialFood, &game->snake2);

        snakeProperty(&game->snake2);

        specialEffectCountDown(game, &game->snake2);

    }
    move(screen, &game->snake1, &game->snake2);
    checkCollition(&game->snake1, &game->snake2, &game->multiplayer);
    if(!game->snake1.alive || !game->snake2.alive) game->gameState = GAME_OVER;
    drawGame(screen, game);

}
void chooseColor(Game* game , Tigr* screen){
    tigrClear(screen, tigrRGB(0,0,0));

        
        tigrPrint(screen, tfont, 100, 130, tigrRGB(255, 0, 0),     "1 - Red");
        tigrPrint(screen, tfont, 100, 160, tigrRGB(0, 255, 0),     "2 - Green");
        tigrPrint(screen, tfont, 100, 190, tigrRGB(0, 0, 255),     "3 - Blue");
        tigrPrint(screen, tfont, 100, 220, tigrRGB(255, 255, 0),   "4 - Yellow");
        tigrPrint(screen, tfont, 100, 250, tigrRGB(128, 0, 128),   "5 - Purple");
        tigrPrint(screen, tfont, 100, 280, tigrRGB(255, 165, 0),   "6 - Orange");
        tigrPrint(screen, tfont, 100, 310, tigrRGB(255, 255, 255), "7 - White");
        tigrPrint(screen, tfont, 100, 340, tigrRGB(128, 128, 128), "8 - Gray");
        tigrPrint(screen, tfont, 100, 370, tigrRGB(0, 255, 255),   "9 - Cyan");
        tigrPrint(screen, tfont, 100, 400, tigrRGB(255, 105, 180), "0 - Pink");
        tigrUpdate(screen);

        if (tigrKeyHeld(screen, '1')) game->color1 = tigrRGB(255, 0, 0);        // Red
        else if (tigrKeyHeld(screen, '2')) game->color1 = tigrRGB(0, 255, 0);   // Green
        else if (tigrKeyHeld(screen, '3')) game->color1 = tigrRGB(0, 0, 255);   // Blue
        else if (tigrKeyHeld(screen, '4')) game->color1 = tigrRGB(255, 255, 0); // Yellow
        else if (tigrKeyHeld(screen, '5')) game->color1 = tigrRGB(128, 0, 128); // Purple
        else if (tigrKeyHeld(screen, '6')) game->color1 = tigrRGB(255, 165, 0); // Orange
        else if (tigrKeyHeld(screen, '7')) game->color1 = tigrRGB(255, 255, 255); // White
        else if (tigrKeyHeld(screen, '8')) game->color1 = tigrRGB(128, 128, 128);       // Gray
        else if (tigrKeyHeld(screen, '9')) game->color1 = tigrRGB(0, 255, 255);   // Cyan
        else if (tigrKeyHeld(screen, '0')) game->color1 = tigrRGB(255, 105, 180); // Pink
        else if (tigrKeyHeld(screen, TK_ESCAPE)) {
            initGame(screen, game);
            game->gameState = MENU;
        }
}

void gameOver(Game*game, Tigr* screen){
    tigrClear(screen, tigrRGB(0,0,0));

    drawGameOver(screen, game);
    
    if(tigrKeyDown(screen, TK_SPACE)){
        game->gameState = MENU;
        initGame(screen, game);
    }
    else if(tigrKeyDown(screen, TK_ESCAPE)) {
        tigrFree(screen);
        exit(0);
    } 
}