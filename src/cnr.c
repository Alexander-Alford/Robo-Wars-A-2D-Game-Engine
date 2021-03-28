/*
This file houses all code related to collision detection and resolution methods. 
*/

#include "global.h"
#include "grid.h"



void vboxMove(Vbox* self) 
{
self->box.x += self->x_v;
self->box.y += self->y_v;	
}
static inline void vboxAddVel(Vbox* self, int16_t x_a, int16_t y_a) 
{
self->x_v += x_a;
self->y_v += y_a;	
}


//AABB collision detection. Tests if shapes are overlapping.
uint8_t boxTestColl(Box* A, Box* B)
{	
/*
0 = collision
1 = left of box
2 = right of box
3 = above box
4 = below box	
*/
	uint8_t ret = 0x0;
	
	if ((A->x + A->w) <= B->x) //A left of B.
	{
	BIT_SET(ret, BIT1, uint8_t);	
	}
	else if (A->x >= (B->x + B->w)) //A right of B.
	{
	BIT_SET(ret, BIT2, uint8_t);
	}

	else if ((A->y + A->h) <= B->y) //A above B.
	{
	BIT_SET(ret, BIT3, uint8_t);
	}	
	else if (A->y >= (B->y + B->h)) //A below B.
	{
	BIT_SET(ret, BIT4, uint8_t);
	}

	else //A is overlapping with B.
	{
	BIT_SET(ret, BIT0, uint8_t);	
	}
	
	return ret;
}


//Collision types. These functions decide how the collision should be resolved.
uint8_t vboxCTbox(Vbox* V, Box* B)
{
//Velocity Overlap Difference Horizontal and Vertical.	
int16_t VOD_H = 0;
int16_t VOD_V = 0;

//Resolve flag.
uint8_t flag = 0x0;

	if (V->x_v > 0)//Resolve Left.
	{
	VOD_H = (V->x_v - ((V->box.x + V->box.w) - B->x));
	BIT_SET(flag, BIT0, uint8_t);
	}	
	else if (V->x_v < 0)//Resolve Right.
	{
	VOD_H = (abs(V->x_v) - ((B->x + B->w) - V->box.x));
	BIT_SET(flag, BIT1, uint8_t);
	}

	if (V->y_v > 0)//Resolve Up.
	{
	VOD_V = (V->y_v - ((V->box.y + V->box.h) - B->y));
	BIT_SET(flag, BIT2, uint8_t);
	}	
	else if (V->y_v < 0)//Resolve Down.
	{
	VOD_V = (abs(V->y_v) - ((B->y + B->h) - V->box.y));
	BIT_SET(flag, BIT3, uint8_t);
	}


if (V->x_v == 0 || V->y_v == 0) return flag;	
	


	if (VOD_H > VOD_V)
	{
	BIT_CLEAR(flag, (BIT2 | BIT3), uint8_t);
	}
	else if (VOD_H < VOD_V)
	{
	BIT_CLEAR(flag, (BIT0 | BIT1), uint8_t);
	}	
	else
	{
	return BIT4;	
	}

return flag;	
}


//Resolution types. 
static inline void boxResLeft(Box* A, Box* B)
{
A->x = (B->x - A->w);	
}
static inline void boxResRight(Box* A, Box* B)
{
A->x = (B->x + B->w);	
}
static inline void boxResUp(Box* A, Box* B)
{
A->y = (B->y - A->h);	
}
static inline void boxResDown(Box* A, Box* B)
{
A->y = (B->y + B->h);	
}


//AABB solver. Detects and resolves collision for vbox and box.
uint8_t vboxSolveBox(Vbox* V, Box* B) 
{
	uint8_t ret = 0x0;	

	if( BIT_GET(boxTestColl(&V->box, B), BIT0, uint8_t) )
	{	
		ret = vboxCTbox(V,B);

		switch (ret)
		{
			case BIT0:
				boxResLeft(&V->box, B);
				break;			
			case BIT1:
				boxResRight(&V->box, B);
				break;
			case BIT2:
				boxResUp(&V->box, B);
				break;
			case BIT3:
				boxResDown(&V->box, B);
				break;		
		}
	}	
	return ret;
}


//Switch function used to resolve every kind of tile collision and return how it resolved.
uint8_t CollisionCodeSwitch(uint16_t collsionCode, Vbox* V, Box Tile)
{
	switch(collsionCode)
	{	
	case 0: //Air tile. Do no collision resolution. This will probably never be used so that the Check_Possible_Tile_Collisions function can be more optimized.
	puts("Master Collision Control air tile call.");
	break;
	case 1: //Solid square block.
		return vboxSolveBox(V, &Tile);				
	case 2: //Vertical half block left.
		Tile.w = TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 3: //Vertical half block right.
		Tile.x += TILE_SIZE/2;
		Tile.w = TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 4: //Horizontal half block top.
		Tile.h = TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 9: //Horizontal half block bottom.
		Tile.y += TILE_SIZE/2;
		Tile.h = TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 10: //Top left quarter block.
		Tile.w = TILE_SIZE/2;
		Tile.h = TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 11: //Top right quarter block.
		Tile.x += TILE_SIZE/2;
		Tile.w = TILE_SIZE/2;
		Tile.h = TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 12: //Bottom left quarter block.
		Tile.y += TILE_SIZE/2;
		Tile.w = TILE_SIZE/2;
		Tile.h = TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 13: //Bottom right quarter block.
		Tile.x += TILE_SIZE/2;
		Tile.y += TILE_SIZE/2;
		Tile.w = TILE_SIZE/2;
		Tile.h = TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	default:
		return 0x0;
	}	
}

//Checks and solves collisions with map tiles. Needs additional checking for collision after resolution on diagonal movement.
uint8_t TileCollisionSolver(Vbox* V)
{
/*	
0 = Left resolution.
1 = Right resolution.
2 = Top resolution.
3 = Bottom resolution.
4 = Corner resolution.
*/
uint8_t ret = 0x0;

Box testTile = {0,0,TILE_SIZE,TILE_SIZE};

	if(V->x_v != 0 || V->y_v != 0) //If vbox is moving.
	{	
		//Vbox's position and size normalized to the grid. 
		//Bottom_y and right_x have their position + dimension decremented for a tricky reason relating to having 0 x and y positions.
		int left_x_norm = V->box.x/TILE_SIZE;
		int right_x_norm = (V->box.x + V->box.w - 1)/TILE_SIZE;
		int top_y_norm = V->box.y/TILE_SIZE;
		int bottom_y_norm = (V->box.y + V->box.h - 1)/TILE_SIZE;
	
		int16_t width_norm = right_x_norm - left_x_norm + 1;
		int16_t height_norm = bottom_y_norm - top_y_norm + 1;			

		unsigned int final_tile;
		int8_t h_increment;
		int16_t v_increment;

		int current_tile = 0;

		if(V->x_v > 0)
		{
		final_tile = right_x_norm;
		h_increment = 1;
		}
		else
		{
		final_tile = left_x_norm;	
		h_increment = -1;
		}
			
		if(V->y_v > 0)
		{
		final_tile += bottom_y_norm*HORIZONTAL_LEVEL_TILES;
		v_increment = HORIZONTAL_LEVEL_TILES;
		}
		else
		{
		final_tile += top_y_norm*HORIZONTAL_LEVEL_TILES;
		v_increment = -HORIZONTAL_LEVEL_TILES;			
		}
		
			if(V->x_v != 0 && V->y_v != 0) //If two velocities exist.
			{	

	
			char flip_flag = 'X';
		
			//Technically 1 more tile than there actually exists, but this is just used as a loop cap so it's fine.
			int total_tiles = width_norm + height_norm - 1;
		
			int x_scanned = 0;
			int y_scanned = 0;
	
				//Scans every tile except for the corner tile, the only one that can resolve as a corner.
				//May try same function with x row and y column scanned separately.
				for(uint_fast32_t I = 0; I < total_tiles; I++)
				{
				
					if(flip_flag == 'X') //Scan x tile.
					{
					x_scanned++;					
					current_tile = final_tile + (x_scanned - width_norm)*h_increment;	
					flip_flag = (y_scanned < (height_norm - 1)) ? 'Y' : 'X'; //If there are still more y tiles to scan.
					}	
					else //Scan y tile.
					{
					y_scanned++;					
					current_tile = final_tile + (y_scanned - height_norm)*v_increment;
					flip_flag = (x_scanned < (width_norm - 1)) ? 'X' : 'Y'; //If there are still more x tiles to scan.
					}	
				
						if(current_tile > -1 && current_tile < TOTAL_TILES)
						{
							if(LEVEL_TILES[current_tile].cCode != 0)
							{	
							testTile.x = (current_tile%HORIZONTAL_LEVEL_TILES)*TILE_SIZE;
							testTile.y = (current_tile/HORIZONTAL_LEVEL_TILES)*TILE_SIZE;	
							ret |= CollisionCodeSwitch(LEVEL_TILES[current_tile].cCode, V, testTile);
							}
						}	
				}
			
			
				if( BIT_GET(ret, BIT4, uint8_t) ) //Corner case. Auto resolves vertically. More complex solution possible, but not necessary. 
				{	
					(V->y_v > 0) ? boxResUp(&V->box, &testTile) : boxResDown(&V->box, &testTile);
				}			
			}
		
		else //If only one velocity exists.
		{
		
		unsigned int axis = (V->x_v != 0) ? height_norm : width_norm;	
		int axis_increment = (V->x_v != 0) ? v_increment : h_increment;	
		
			for(unsigned register int I = 1; I <= axis; I++)
			{
			current_tile = final_tile + (I - axis)*axis_increment;

				if(current_tile > -1 && current_tile < TOTAL_TILES)
				{
					if(LEVEL_TILES[current_tile].cCode != 0)
					{	
					testTile.x = (current_tile%HORIZONTAL_LEVEL_TILES)*TILE_SIZE;
					testTile.y = (current_tile/HORIZONTAL_LEVEL_TILES)*TILE_SIZE;	
					ret |= CollisionCodeSwitch(LEVEL_TILES[current_tile].cCode, V, testTile);
					}
				}			
			}
			//printf("Tiles = %d - %d = %d.\n", right_x_norm, left_x_norm ,width_norm);
			
		}
	
	
	return ret;
	}		
}




