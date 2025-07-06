the snake body is following the one before it and the whole body is following the head 

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
SINGLE_PLAYER :
TigrClear(screen);
user input : W = 0, S = 2, A = 3, D = 1;    0 up, 2 down, 3 left, 4 right
snakeProperty
check collision
eatFood
drawGame