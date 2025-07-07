the snake body is following the one before it and the whole body is following the head 

gcc main.c audio.c snake.c screen.c tigr.c -o ../bin/snake -lm -lpthread -ldl -lGL -lX11 && ../bin/snake

snake have 3 lives
snake die from eatng 3 booms
snake got speed up for 5 sec from eating the special fruit

single player : check collission between self and wall
multiplayer : check collision between other snake and wall, not self collision

game struct :
init both snake 
draw grid 
(before the game start)

in main
MENU : 
if click 1 Singleplayer -> GameState = SINGLE_PLAYER
if click 2 Multiplayer -> GameState = MULTI_PLAYER
if click esc exit the game
SINGLE_PLAYER : if 
TigrClear(screen);
user input : W = 0, S = 2, A = 3, D = 1;    0 up, 2 down, 3 left, 4 right
snakeProperty
check collision
eatFood
drawGame
MULTIPLAYER : check collision between wall and snakes only, if snake1 head hit snake2 body snake2 will win even if snake1 have more score, but if both of the snakes head hit eachother the game will decide the winner on point
