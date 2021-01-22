/*
Header file for all global variables/objects that will be used by several object files.
//Only use extern on variables that need to be declared for reference in other files and are defined in a single file.
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


//A simple structure to describe rectangles. Incredibly useful in almost every file. Exactly the same as SDL_Rect so it became a #define for convenience.
#define Box SDL_Rect




//Expanded form of the box structure. Also very useful, though takes up more space.
typedef struct
{
	Box box;
	int x_vel;
	int y_vel;
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



//Defined in Grid.c
extern int BASE_TILE_SIZE;
extern int BASE_QUADRANT_SIZE;

extern unsigned int LEVEL_SIZE_V;
extern unsigned int LEVEL_SIZE_H;

//These macros make it easier to call the total number of tiles in a row/column on a map.
#define VERTICAL_LEVEL_TILES (LEVEL_SIZE_V*BASE_QUADRANT_SIZE)
#define HORIZONTAL_LEVEL_TILES (LEVEL_SIZE_H*BASE_QUADRANT_SIZE)
#define TOTAL_TILES (VERTICAL_LEVEL_TILES*HORIZONTAL_LEVEL_TILES)

#endif