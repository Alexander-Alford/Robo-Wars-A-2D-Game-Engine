#ifndef GRID_H
#define GRID_H

#include "global.h"

typedef struct
{
	uint8_t tCode; //Texture code.
	uint8_t cCode; //Collision code.
	uint8_t bProp;
	SDL_RendererFlip flip;	
} Tile;

SDL_Texture *TileTexture;
Tile* LEVEL_TILES;
uint32_t TOTAL_TILES;


#endif