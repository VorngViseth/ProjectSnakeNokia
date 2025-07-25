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
            snprintf(gameOverText, sizeof(gameOverText), " DRAW!\nScores:\n  %d-%d", game->snake1.score, game->snake2.score);
        } else if (!game->snake1.alive) {
            snprintf(gameOverText, sizeof(gameOverText), "PLAYER2\n WINS!\n  %d-%d", game->snake1.score, game->snake2.score);
        } else {
            snprintf(gameOverText, sizeof(gameOverText), "PLAYER1\n WINS!\n  %d-%d", game->snake1.score, game->snake2.score);
        }
    } else {
        snprintf(gameOverText, sizeof(gameOverText), "GAME OVER!\n Score: %d", game->snake1.score);
    }

    float scale = 10.0f;
    
    int tw = tigrTextWidth(tfont, gameOverText);
    drawScaledText(screen, tfont, (WINDOW_WIDTH-(tw*(scale)))/2, 230, tigrRGB(100, 100, 100), scale, gameOverText);

    const char* restartText = "Press SPACE to return to menu";
    tw = tigrTextWidth(tfont, restartText);
    drawScaledText(screen, tfont, (WINDOW_WIDTH-(tw*(scale-7.5)))/2, 720, tigrRGB(100, 100, 100), scale-7.5, restartText);

    const char* exitText = "Press ESC to exit the game";
    tw = tigrTextWidth(tfont, exitText);
    drawScaledText(screen, tfont, (WINDOW_WIDTH-(tw*(scale-8)))/2, 790, tigrRGB(100, 100, 100), scale-8, exitText);
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

    if(game->snake1.alive) drawSnake(screen, &game->snake1);
    if(&game->snake2 && game->multiplayer && game->snake2.alive) drawSnake(screen, &game->snake2);

    drawScoreBoard(screen, &game->snake1, 10, 10);
    if(&game->snake2 && game->multiplayer && game->snake2.alive) drawScoreBoard(screen, &game->snake2, 37*CELL_SIZE, 10);
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

    game->bmg_boom_play = false;
    game->bmg_food_play = false;
    game->bmg_specialFood_play = false;
    game->bmg_gameOver_play = false;

    init_audio(&game->foodAudio);
    init_audio(&game->boomAudio);
    init_audio(&game->specialFoodAudio);
    init_audio(&game->gameOverAudio);

    snakeInit(&game->snake1, game->snake1.color, centerX - 5, centerY, &game->dir1);
    snakeInit(&game->snake2, game->snake2.color, centerX + 5, centerY, &game->dir2);

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

        eatFood(&game->food, &game->snake1, game);
        eatBoom(game, &game->boom, &game->snake1);
        eatSpecialFood(&game->specialFood, &game->snake1, game);

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

        eatFood(&game->food, &game->snake1, game);
        
        eatBoom(game, &game->boom, &game->snake1);
        
        eatSpecialFood(&game->specialFood, &game->snake1, game);

        snakeProperty(&game->snake1);
        
        specialEffectCountDown(game, &game->snake1);
        
    }
    if(game->snake2.timer >= game->snake2.delay) {
        
        game->snake2.timer = 0;

        placeObject(game);

        eatFood(&game->food, &game->snake2, game);
        
        eatBoom(game, &game->boom, &game->snake2);
        
        eatSpecialFood(&game->specialFood, &game->snake2, game);

        snakeProperty(&game->snake2);

        specialEffectCountDown(game, &game->snake2);

    }
    move(screen, &game->snake1, &game->snake2);
    checkCollition(&game->snake1, &game->snake2, &game->multiplayer);
    if(!game->snake1.alive || !game->snake2.alive) game->gameState = GAME_OVER;
    drawGame(screen, game);

}

void chooseColor(Game* game , Tigr* screen){
    tigrClear(screen, tigrRGB(0, 0, 0));

    Tigr* menuPicture = tigrLoadImage("asset/background.png");
    
    int x = (screen->w - menuPicture->w) / 2;
    int y = (screen->h - menuPicture->h) / 2;

    tigrBlit(screen, menuPicture, x, y, 0, 0, menuPicture->w, menuPicture->h);

    float scale = 5.0f; // Adjust this value to make text bigger or smaller

    drawScaledText(screen, tfont, 150, 80, game->snake1.color,            scale, "Player1");
    drawScaledText(screen, tfont, 100, 180, tigrRGB(255, 0, 0),     scale, "1 - Red");
    drawScaledText(screen, tfont, 100, 240, tigrRGB(0, 255, 0),     scale, "2 - Green");
    drawScaledText(screen, tfont, 100, 300, tigrRGB(0, 0, 255),     scale, "3 - Blue");
    drawScaledText(screen, tfont, 100, 360, tigrRGB(255, 255, 0),   scale, "4 - Yellow");
    drawScaledText(screen, tfont, 100, 420, tigrRGB(128, 0, 128),   scale, "5 - Purple");
    drawScaledText(screen, tfont, 100, 480, tigrRGB(255, 165, 0),   scale, "6 - Orange");
    drawScaledText(screen, tfont, 100, 540, tigrRGB(255, 255, 255), scale, "7 - White");
    drawScaledText(screen, tfont, 100, 600, tigrRGB(128, 128, 128), scale, "8 - Gray");
    drawScaledText(screen, tfont, 100, 660, tigrRGB(0, 255, 255),   scale, "9 - Cyan");
    drawScaledText(screen, tfont, 100, 720, tigrRGB(255, 105, 180), scale, "0 - Pink");

    drawScaledText(screen, tfont, 600, 80, game->snake2.color,            scale, "Player2");
    drawScaledText(screen, tfont, 550, 180, tigrRGB(0, 128, 128),   scale, "Q - Teal");
    drawScaledText(screen, tfont, 550, 240, tigrRGB(173, 255, 47),  scale, "W - Lime");
    drawScaledText(screen, tfont, 550, 300, tigrRGB(128, 0, 0),     scale, "E - Maroon");
    drawScaledText(screen, tfont, 550, 360, tigrRGB(0, 0, 128),     scale, "R - Navy Blue");
    drawScaledText(screen, tfont, 550, 420, tigrRGB(255, 215, 0),   scale, "T - Gold");
    drawScaledText(screen, tfont, 550, 480, tigrRGB(135, 206, 235), scale, "Y - Sky Blue");
    drawScaledText(screen, tfont, 550, 540, tigrRGB(128, 128, 0),   scale, "U - Olive");
    drawScaledText(screen, tfont, 550, 600, tigrRGB(255, 127, 80),  scale, "I - Coral");
    drawScaledText(screen, tfont, 550, 660, tigrRGB(250, 128, 114), scale, "O - Salmon");
    drawScaledText(screen, tfont, 550, 720, tigrRGB(75, 0, 130),    scale, "P - Indigo");

    drawScaledText(screen, tfont, 300, 850, tigrRGB(255, 255, 255),    scale, "ESC - to exit");


    if (tigrKeyHeld(screen, '1')) game->snake1.color = tigrRGB(255, 0, 0);          // Red
    else if (tigrKeyHeld(screen, '2')) game->snake1.color = tigrRGB(0, 255, 0);     // Green
    else if (tigrKeyHeld(screen, '3')) game->snake1.color = tigrRGB(0, 0, 255);     // Blue
    else if (tigrKeyHeld(screen, '4')) game->snake1.color = tigrRGB(255, 255, 0);   // Yellow
    else if (tigrKeyHeld(screen, '5')) game->snake1.color = tigrRGB(128, 0, 128);   // Purple
    else if (tigrKeyHeld(screen, '6')) game->snake1.color = tigrRGB(255, 165, 0);   // Orange
    else if (tigrKeyHeld(screen, '7')) game->snake1.color = tigrRGB(255, 255, 255); // White
    else if (tigrKeyHeld(screen, '8')) game->snake1.color = tigrRGB(128, 128, 128); // Gray
    else if (tigrKeyHeld(screen, '9')) game->snake1.color = tigrRGB(0, 255, 255);   // Cyan
    else if (tigrKeyHeld(screen, '0')) game->snake1.color = tigrRGB(255, 105, 180); // Pink
    else if (tigrKeyHeld(screen, 'Q') || tigrKeyHeld(screen, 'q')) game->snake2.color = tigrRGB(0, 128, 128);       // Teal
    else if (tigrKeyHeld(screen, 'W') || tigrKeyHeld(screen, 'w')) game->snake2.color = tigrRGB(173, 255, 47);      // Lime
    else if (tigrKeyHeld(screen, 'E') || tigrKeyHeld(screen, 'e')) game->snake2.color = tigrRGB(128, 0, 0);         // Navy Blue
    else if (tigrKeyHeld(screen, 'R') || tigrKeyHeld(screen, 'r')) game->snake2.color = tigrRGB(0, 0, 128);         // Maroon
    else if (tigrKeyHeld(screen, 'T') || tigrKeyHeld(screen, 't')) game->snake2.color = tigrRGB(255, 215, 0);       // Gold
    else if (tigrKeyHeld(screen, 'Y') || tigrKeyHeld(screen, 'y')) game->snake2.color = tigrRGB(135, 206, 235);     // Sky Blue
    else if (tigrKeyHeld(screen, 'U') || tigrKeyHeld(screen, 'u')) game->snake2.color = tigrRGB(128, 128, 0);       // Olive
    else if (tigrKeyHeld(screen, 'I') || tigrKeyHeld(screen, 'i')) game->snake2.color = tigrRGB(255, 127, 80);      // Coral
    else if (tigrKeyHeld(screen, 'O') || tigrKeyHeld(screen, 'o')) game->snake2.color = tigrRGB(250, 128, 114);     // Salmon
    else if (tigrKeyHeld(screen, 'P') || tigrKeyHeld(screen, 'p')) game->snake2.color = tigrRGB(75, 0, 130);        // Indigo

    else if (tigrKeyHeld(screen, TK_ESCAPE)) {
        
        game->gameState = MENU;
    }
}

void gameOver(Game* game, Tigr* screen){
    tigrClear(screen, tigrRGB(0, 0, 0));

    drawGameOver(screen, game);

    if(!game->bmg_gameOver_play) {
        stop_bgm(&game->audio);
        shutdown_audio(&game->gameOverAudio);
        init_audio(&game->gameOverAudio);
        play_bgm(&game->gameOverAudio, "asset/snakeGameOver.mp3");
        game->bmg_gameOver_play = true;
    }

    if(tigrKeyDown(screen, TK_SPACE)){
        game->bmg_play = false;
        shutdown_audio(&game->gameOverAudio);
        shutdown_audio(&game->foodAudio);
        shutdown_audio(&game->boomAudio);
        shutdown_audio(&game->specialFoodAudio);
        game->gameState = MENU;
        initGame(screen, game);
    }
    else if(tigrKeyDown(screen, TK_ESCAPE)) {
        cleanUpGame(game, screen);
        exit(0);
    } 
}

void cleanUpGame(Game* game, Tigr* screen){
    shutdown_audio(&game->gameOverAudio);
    shutdown_audio(&game->audio);
    shutdown_audio(&game->foodAudio);
    shutdown_audio(&game->boomAudio);
    shutdown_audio(&game->specialFoodAudio);
    tigrFree(screen);
}