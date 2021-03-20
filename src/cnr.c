/*
This file houses all code related to collision detection and resolution methods. 
*/

#include "global.h"




inline void vboxMove(Vbox* self) 
{
self->box.x += self->x_v;
self->box.y += self->y_v;	
}
inline void vboxAddVel(Vbox* self, int16_t x_a, int16_t y_a) 
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
char vboxCTbox(Vbox* V, Box* B)
{
//Velocity Overlap Difference Horizontal and Vertical.	
int16_t VOD_H = 0;
int16_t VOD_V = 0;

//Resolve flags.
char vf = '0';
char hf = '0';


	if (V->x_v > 0)
	{
	VOD_H = (V->x_v - ((V->box.x + V->box.w) - B->x));
	hf = 'l';
	}	
	else if (V->x_v < 0)
	{
	VOD_H = (abs(V->x_v) - ((B->x + B->w) - V->box.x));
	hf = 'r';
	}

	if (V->y_v > 0)
	{
	VOD_V = (V->y_v - ((V->box.y + V->box.h) - B->y));
	vf = 'u';
	}	
	else if (V->y_v < 0)
	{
	VOD_V = (abs(V->y_v) - ((B->y + B->h) - V->box.y));
	vf = 'd';
	}


if (V->x_v == 0) return vf;	
if (V->y_v == 0) return hf;	


	if (VOD_H > VOD_V)
	{
	return hf;
	}
	else if (VOD_H < VOD_V)
	{
	return vf;
	}	
	else
	{
	return 'c';	
	}	
}


//Resolution types. 
inline void boxResLeft(Box* A, Box* B)
{
A->x = (B->x - A->w);	
}
inline void boxResRight(Box* A, Box* B)
{
A->x = (B->x + B->w);	
}
inline void boxResUp(Box* A, Box* B)
{
A->y = (B->y - A->h);	
}
inline void boxResDown(Box* A, Box* B)
{
A->y = (B->y + B->h);	
}


//AABB solver. Detects and resolves collision for vbox and box.
char vboxSolveBox(Vbox* V, Box* B) 
{
	char ret = '0';	

	if( BIT_GET(boxTestColl(&V->box, B), BIT0, uint8_t) )
	{	
		ret = vboxCTbox(V,B);

		switch (ret)
		{
			case 'r':
				boxResRight(&V->box, B);
				break;
			case 'l':
				boxResLeft(&V->box, B);
				break;
			case 'u':
				boxResUp(&V->box, B);
				break;
			case 'd':
				boxResDown(&V->box, B);
				break;		
		}
	}	
	return ret;
}


//Switch function used to resolve every kind of tile collision and return how it resolved.
char CollisionCodeSwitch(uint16_t collsionCode, Vbox* V, Box Tile)
{
	switch(collsionCode)
	{	
	case 0: //Air tile. Do no collision resolution. This will probably never be used so that the Check_Possible_Tile_Collisions function can be more optimized.
	puts("Master Collision Control air tile call.");
	break;
	case 1: //Solid square block.
		return vboxSolveBox(V, &Tile);				
	case 2: //Vertical half block left.
		Tile.w = BASE_TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 3: //Vertical half block right.
		Tile.x += BASE_TILE_SIZE/2;
		Tile.w = BASE_TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 4: //Horizontal half block top.
		Tile.h = BASE_TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 9: //Horizontal half block bottom.
		Tile.y += BASE_TILE_SIZE/2;
		Tile.h = BASE_TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 10: //Top left quarter block.
		Tile.w = BASE_TILE_SIZE/2;
		Tile.h = BASE_TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 11: //Top right quarter block.
		Tile.x += BASE_TILE_SIZE/2;
		Tile.w = BASE_TILE_SIZE/2;
		Tile.h = BASE_TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 12: //Bottom left quarter block.
		Tile.y += BASE_TILE_SIZE/2;
		Tile.w = BASE_TILE_SIZE/2;
		Tile.h = BASE_TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	case 13: //Bottom right quarter block.
		Tile.x += BASE_TILE_SIZE/2;
		Tile.y += BASE_TILE_SIZE/2;
		Tile.w = BASE_TILE_SIZE/2;
		Tile.h = BASE_TILE_SIZE/2;
		return vboxSolveBox(V, &Tile);
	default:
		return '0';
	}	
}

//Checks and solves collisions with map tiles. Needs additional checking for collision after resolution on diagonal movement.
uint8_t TileCollisionSolver(Vbox* V)
{
/*	
0 = Right resolution.
1 = Left resolution.
2 = Top resolution.
3 = Bottom resolution.
4 = Corner resolution.
*/
uint8_t ret = 0x0;


Box testTile = {0,0,BASE_TILE_SIZE,BASE_TILE_SIZE};




	if(V->x_v != 0 || V->y_v != 0) //If vbox is moving.
	{	
		//Vbox's position and size normalized to the grid. 
		//Bottom_y and right_x have their position + dimension decremented for a tricky reason relating to having 0 x and y positions.
		int left_x_norm = self->box.x/BASE_TILE_SIZE;
		int right_x_norm = (self->box.x + self->box.w - 1)/BASE_TILE_SIZE;
		int top_y_norm = self->box.y/BASE_TILE_SIZE;
		int bottom_y_norm = (self->box.y + self->box.h - 1)/BASE_TILE_SIZE;
	
		int16_t width_norm = right_x_norm - left_x_norm + 1;
		int16_t height_norm = bottom_y_norm - top_y_norm + 1;		
		//Buffer for ret.
		//GBFS check = {0,0,0,0,0,0,0,0};		

		unsigned int final_tile;
		signed char h_increment;
		signed int v_increment;

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
			for(unsigned register int I = 0; I < total_tiles; I++)
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
						if(p_PRIMARY_TILE_ARRAY[current_tile].CollisionCode != 0)
						{	
						testtile.x = (current_tile%HORIZONTAL_LEVEL_TILES)*BASE_TILE_SIZE;
						testtile.y = (current_tile/HORIZONTAL_LEVEL_TILES)*BASE_TILE_SIZE;	
						check = CollisionCodeSwitch(p_PRIMARY_TILE_ARRAY[current_tile].CollisionCode, self, &test_tile);
						}
						
					//printf("Tile %d at %d,%d.\n", current_tile, test_tile.x, test_tile.y);
					
					//ret.a |= check.a; //Bitwise OR adds any 1 collision flags to the return GBFD.
					//ret.b |= check.b;
					//ret.c |= check.c;
					//ret.d |= check.d;
					//ret.e |= check.e;
					//ret.f |= check.f;
					}	
			}
			
			
				if(0) //Corner case. Auto resolves vertically. More complex solution possible, but not necessary. 
				{
					if(self->y_vel > 0)
					{
					Res_Top(&self->box, &test_tile);	
					}
					else
					{
					Res_Bottom(&self->box, &test_tile);		
					}
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
					if(p_PRIMARY_TILE_ARRAY[current_tile].CollisionCode != 0)
					{	
					testtile.x = (current_tile%HORIZONTAL_LEVEL_TILES)*BASE_TILE_SIZE;
					testtile.y = (current_tile/HORIZONTAL_LEVEL_TILES)*BASE_TILE_SIZE;	
					//check = CollisionCodeSwitch(p_PRIMARY_TILE_ARRAY[current_tile].CollisionCode, self, &test_tile);
					}
						
				
					
				//ret.a |= check.a; 
				//ret.b |= check.b;
				//ret.c |= check.c;
				//ret.d |= check.d;
				//ret.e |= check.e;
				//ret.f |= check.f;
				}			
			}
			//printf("Tiles = %d - %d = %d.\n", right_x_norm, left_x_norm ,width_norm);
			
		}
	
	
	return ret;
	}		
}




