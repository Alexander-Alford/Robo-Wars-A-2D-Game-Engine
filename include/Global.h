/*
Header file for all global variables/objects/macros that will be used by several object files.
Smaller source files will have their definitions placed here instead of in their own header file.
*/

#ifndef GLOBAL_H
#define GLOBAL_H

//Will be used in almost every source file.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"



//Bit masks along with some bitwise macros. 
#define BIT0 0x1 
#define BIT1 0x2
#define BIT2 0x4
#define BIT3 0x8
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define BIT_GET(var, mask, type) ((type) (var & mask))
#define BIT_FLIP(var, mask, type) ((type) (var ^= mask))
#define BIT_CLEAR(var, mask, type) ((type) (var &= ~mask))
#define BIT_SET(var, mask, type) ((type) (var |= mask))

#define Box SDL_Rect

typedef struct
{
	Box box;
	int16_t x_v;
	int16_t y_v;
} Vbox;



int Tick(int *holder, int tick, int cap, char reset, int resetVal);
void FrameControl(); 

//Defined in main.c
extern char coreloop_f;

//Defined in startup.c
void Start_Window();
void Terminate();

//Defined in gfx.c
extern uint16_t WINDOW_WIDTH;
extern uint16_t WINDOW_HEIGHT;
extern SDL_Rect SCREEN;
extern float TARGET_FPS;

uint8_t isOnScreen(SDL_Rect* test);
void CoreGraphic();

//Defined in sfx.c
void CoreSound();

//Defined in cnr.c
uint8_t boxTestColl(Box* A, Box* B);

//Defined in object.c
void CoreLogic();

//Defined in input.c
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

uint16_t KEY_ARR_FINAL[TOTAL_KEYS];

void CoreInput();

//Defined in startup.c
void StartProgram();
void TerminateProgram();

//Defined in grid.c
extern uint8_t TILE_SIZE;
extern uint8_t QUADRANT_SIZE;
extern int LEVEL_SIZE_V;
extern int LEVEL_SIZE_H;

//These macros make it easier to call the total number of tiles in a row/column on a map.
#define VERTICAL_LEVEL_TILES (LEVEL_SIZE_V*QUADRANT_SIZE)
#define HORIZONTAL_LEVEL_TILES (LEVEL_SIZE_H*QUADRANT_SIZE)


#endif