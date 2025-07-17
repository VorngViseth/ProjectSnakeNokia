the snake body is following the one before it and the whole body is following the head 
on linux :
gcc main.c audio.c snake.c screen.c tigr.c object.c -o ./bin/snake -lm -lpthread -ldl -lGL -lX11 && ./bin/snake

on window :
gcc main.c audio.c snake.c screen.c tigr.c object.c -o ./bin/snake -lwinmm -lgdi32 -lopengl32
then run :
./bin/snake 

user input : W = 0, S = 2, A = 3, D = 1;    0 up, 2 down, 3 left, 4 right

snake got speed up for 5 sec from eating the special fruit

subtrack 3 lengths and scores for each time snake eat a bomb

single player : check collission between self and wall
multiplayer : check collision between other snake and wall, not self collision

MULTIPLAYER : check collision between wall and snakes only, if snake1 head hit snake2 body snake2 will win even if snake1 have more score, but if both of the snakes head hit eachother the game will decide the winner on point
