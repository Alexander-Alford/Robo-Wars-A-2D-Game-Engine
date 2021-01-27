/*
Header file for all global variables/objects/macros that will be used by several object files.
*/

#ifndef GLOBAL_H
#define GLOBAL_H

//Will be used in almost every source file.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


//This define cleans up the code a bit.
#define Box SDL_Rect


//Box with velocity components. Very useful.
typedef struct
{
	Box box;
	int16_t x_v;
	int16_t y_v;
} Vbox;



int Tick(int *tick_holder, int tick_num, int tick_cap, char cap_reset, int holder_reset_val);
void FrameControl(); 

//Defined in main.c
extern char F_Core_Loop;
extern int GAME_SPEED;


//Defined in Graphics.c
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern float TARGET_FPS;

//Defined in Input.c
enum KEY_STATES
{
STANDBY,
PRESSED,
HELD,
RELEASED
};
enum KEYS
{
UNKNOWN_KEY,
UP,
DOWN,
LEFT,
RIGHT,
A,
B,
C,
D,
E,
F,
G,
H,
I,
J,
K,
L,
M,
N,
O,
P,
Q,
R,
S,
T,
U,
V,
W,
X,
Y,
Z,
SPACE,
TOTAL_KEYS
};

uint8_t KEY_ARR_FINAL[TOTAL_KEYS];

void CoreInput();

//Defined in Grid.c
extern int BASE_TILE_SIZE;
extern int BASE_QUADRANT_SIZE;

extern unsigned int LEVEL_SIZE_V;
extern unsigned int LEVEL_SIZE_H;

//These macros make it easier to call the total number of tiles in a row/column on a map.
#define VERTICAL_LEVEL_TILES (LEVEL_SIZE_V*BASE_QUADRANT_SIZE)
#define HORIZONTAL_LEVEL_TILES (LEVEL_SIZE_H*BASE_QUADRANT_SIZE)


#endif