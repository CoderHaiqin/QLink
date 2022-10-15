#pragma once

#define TEST 1

//basic constants
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define PERIOD 20       //update per 50 ms

//constants of gameState
#define GAME_LOADING 0
#define GAME_ONGOING 1
#define GAME_PAUSE 2

#define SINGAL_MODE 0
#define DOUBLE_MODE 1
#define LOAD_DATA 2

#define GAMEOVER_TIMEUP 4
#define GAMEOVER_NOSOLUTION 5

//constant of direction
#define STOP 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define LEFT_UP 5
#define LEFT_DOWN 6
#define RIGHT_DOWN 7
#define RIGHT_UP 8

//constant of visible objects
#define NUM_OF_IMAGES 11
#define NUM_OF_BLOCK_IMAGES 4

#define BLOCK_RED 0
#define BLOCK_BLUE 1
#define BLOCK_ORANGE 2
#define BLOCK_GREEN 3
#define BLOCK_SHUFFLE 4
#define BLOCK_TIME 5
#define BLOCK_HINT 6
#define NON_BLOCK -1
#define TEMP_BLOCK -2

//constant for timing
#define HINT_TIME 10.0f
#define LINE_TIME 3.0f
#define ITEM_GENERATE_CD 10.0f

//size
#define BLOCK_SIZE 50
#define GRID_SIZE_X 10
#define GRID_SIZE_Y 10

//player
#define PLAYER_1 7
#define SELECTION_1 8
#define PLAYER_2 9
#define SELECTION_2 10
#define PLAYER_SIZE 20
#define SPEED 4
#define BLOCK_SCORE 100

//signal type
#define ERASERED 0
#define SHUFFLED 1
