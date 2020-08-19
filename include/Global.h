//Header file for all global variables/objects that will be used by several object files.
//Only use extern on variables that need to be declared for reference in other files and are defined in a single file.
//By stating the type and name of a variable, it is technically defined. This is not true of functions, storage classes, or pointers.


#ifndef G_CONSTANTS_H
#define G_CONSTANTS_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
//This structure is a Generic Boolean Flag Structure that can hold eight boolean flags in a single byte. 
typedef struct 
{
unsigned char a : 1;
unsigned char b : 1;
unsigned char c : 1;
unsigned char d : 1;
unsigned char e : 1;
unsigned char f : 1;
unsigned char g : 1;
unsigned char h : 1;	
} GBFS;

//A simple structure to describe rectangles. Incredibly useful in almost every file. Exactly the same as SDL_Rect so it became a typedef for convenience.
#define Box SDL_Rect

//Expanded form of the box structure. Also very useful, though takes up more space.
typedef struct
{
	Box box;
	int x_vel;
	int y_vel;
} VBox;

//Dynamic pointer structure. Very useful.
typedef struct
{
unsigned int size;
unsigned int non_null;	
void** Array;		
} DPS; 

//Defined in Global.c.
GBFS Assign_GBFS(unsigned int* buf);
int First_Null_In_DPS_Array(DPS* pnt);
DPS* Create_DPS(unsigned int num);
void DPS_shuffle(DPS* p_target);
void DPS_reallocate(DPS* p_target, unsigned int new_num);
void Destroy_DPS(DPS* p_target);
int DPS_Add_Member(DPS* target, void* addition, unsigned int possible_expansion);
int DPS_Remove_Member(DPS* holder, unsigned int index_tbr);
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

//Defined in Object.c


#endif