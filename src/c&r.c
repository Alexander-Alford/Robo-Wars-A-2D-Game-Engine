/*
This file houses all code related to collision detection and resolution methods. 
*/

#include <Global.h>
#include <Input.h>
#include <Grid.h>
#include <Object.h>







/*
Investigate bound slope against perfectly diagonal boxes.

Simple box resolution
scan tile till a vertical and horizontal resolution are called for
resolve higher velocity axis first

Char Resolution Flags
L = resolved left
R = resolved right
U = resolved up
D = resolved down
C = resolved corner
S = resolved slope
*/



void MoveVbox(Vbox* sself) //Inline?
{
self->box.x += self->x_v;
self->box.y += self->y_v;	
}

void AddVel(Vbox* self, int16_t x_a, int16_t y_a) //Inline?
{
self->x_v += x_a;
self->y_v += y_a;	
}




//Collision detecting functions for various geometrical scenarios. 
GBFS Det_Box_Collision(Box* A, Box* B)
{	
/*
0 = false, 1 = true
a = Collision?
b = left of box?
c = right of box?
d = above box?
e = below box?
f = 
g = 	
*/
GBFS ret = {0,0,0,0,0,0,0,0};
	
	if ((A->x + A->w) <= B->x) //A left of B.
	{
	ret.b = 1;	
	}
	
	else if (A->x >= (B->x + B->w)) //A right of B.
	{
	ret.c = 1;	
	}


	else if ((A->y + A->h) <= B->y) //A above B.
	{
	ret.d = 1;
	}	

	else if (A->y >= (B->y + B->h)) //A below B.
	{
	ret.e = 1;	
	}

	else //A is overlapping with B.
	{
	ret.a = 1;	
	}
	
	return ret;
}

char Det_Point_Slope_Collision(int x, int y, Slope* Z)
{
	//This funstion takes the equation of a line, y = m(x+c) + b, and compares it to an x and y point.
	//From this comparison we can determine whether the point is on the line, above it, or below it.
	//Positive value = under, negative value = above, 0 = on line.
	//This if else serves to allow for simple integer division rather than needlessly using a double or float.
	int compare = (y - Z->y) - ( (Z->F_div == 'T') ? ((x - Z->x)/Z->m ) : ((x - Z->x)*Z->m ) );
	
	if(Z->F_corner == 00 || Z->F_corner == 01)
	{
		return (compare > 0) ? 'T' : 'F';
	}
	else
	{
		return (compare < 0) ? 'T' : 'F';
	}

}

char Det_Box_Point_Collision(int x, int y, Box* A)
{
if (x < A->x || x > (A->x + A->w))	//Point outside horizontal bounds.
{
return 'F';
}
if (y < A->y || y > (A->y + A->h)) //Point outside vertical bounds.
{
return 'F';	
}

return 'T';			
}	
	





//Collision types. These functions decide how the collision should be resolved.
char Col_Rect(VBox* V, Box* B)
{
//Velocity Overlap Difference Horizontal and Vertical.	
short int VOD_H = 0;
short int VOD_V = 0;

//Dirtection flags. R/L, U/D.
char hflag = 'N';
char vflag = 'N';

	if (V->x_vel > 0)
	{
	VOD_H = (V->x_vel - ((V->box.x + V->box.w) - B->x));
	hflag = 'L';
	}	
	else if (V->x_vel < 0)
	{
	VOD_H = (abs(V->x_vel) - ((B->x + B->w) - V->box.x));
	hflag = 'R';
	}

	if (V->y_vel > 0)
	{
	VOD_V = (V->y_vel - ((V->box.y + V->box.h) - B->y));
	vflag = 'U';
	}	
	else if (V->y_vel < 0)
	{
	VOD_V = (abs(V->y_vel) - ((B->y + B->h) - V->box.y));
	vflag = 'D';
	}

if (V->x_vel == 0)
{
return vflag;	
}
else if (V->y_vel == 0)
{
return hflag;	
}



	if (VOD_H > VOD_V)
	{
	return hflag;
	}
	else if (VOD_H < VOD_V)
	{
	return vflag;
	}	
	else
	{
	return 'C';	
	}	
}





//Resolution types. 
void Res_Left(Box* A, Box* B)
{
A->x = (B->x - A->w);	
}
void Res_Right(Box* A, Box* B)
{
A->x = (B->x + B->w);	
}
void Res_Top(Box* A, Box* B)
{
A->y = (B->y - A->h);	
}
void Res_Bottom(Box* A, Box* B)
{
A->y = (B->y + B->h);	
}
void Res_Slope_Vertical(Box* A, Slope* Z)
{
//If slopes resolve up, subtract height from vertical position.	
int y_off = (Z->F_corner == 00 || Z->F_corner == 01) ? A->h : 0;
//Slope division branching.
A->y = Z->y - y_off + (Z->F_div == 'F') ? ( ((A->x + A->w)/2) - Z->x)*Z->m : ( ((A->x + A->w)/2) - Z->x)/Z->m;			
}
void Res_Slope_Horizontal(Box* A, Slope* Z)
{
A->x = (Z->F_div == 'F') ? ( (Z->x + (A->y - Z->y)/Z->m)*2 ) : ( (Z->x + (A->y - Z->y)*Z->m)*2 ) - A->w;		
}







GBFS Solver_Rect(VBox* V, Box* B, GBFS C) 
{
GBFS ret = {0,0,0,0,0,0,0,0};	
//C.a = 0 = implicit knowledge that box is already colliding.	
	if(C.a == 0 || Det_Box_Collision(&V->box, B).a == 1)
	{	
		switch (Col_Rect(V,B))
		{
			case 'R':
				Res_Right(&V->box, B);
				ret.a = 1;
				break;
			case 'L':
				Res_Left(&V->box, B);
				ret.b = 1;
				break;			
			case 'U':
				Res_Top(&V->box, B);
				ret.c = 1;
				break;
			case 'D':
				Res_Bottom(&V->box, B);
				ret.d = 1;
				break;
			case 'C':
				ret.e = 1;			
				break;			
		}
	return ret;
	}	
}

GBFS Solver_Slope(Box* A, Box* B, Slope* Z, GBFS C) 
{
GBFS ret = {0,0,0,0,0,0,0,0};
	
int y_point = ((Z->F_corner == 00 || Z->F_corner == 01) ? (A->y + A->h) : A->y);
int x_point = (A->x + A->w)/2;

	if(Det_Box_Point_Collision(x_point, y_point, B) == 'T' && Det_Point_Slope_Collision(x_point, y_point, Z) == 'T')
	{
		if(C.b == 1) //Vertical resolution
		{
		Res_Slope_Vertical(A, Z);	
		}	
		else 
		{
		Res_Slope_Horizontal(A, Z);	
		}
	ret.f = 1;
	}
return ret;		
}





//Switch fimction used to resolve every kind of tile collision and return how it resolved.
GBFS Collision_Code_Master_Switch(int F_CC, VBox* p_TBR, Box* Tile)
{

Slope slo = {'F',0,1,Tile->x,Tile->y};
Box box = *Tile;
/*
0/1 = false/true
a = Assume rect is already colliding? Useful for solid blocks. It appears this is going to be completely abandoned due to bugs.
b = horizontal slope resolution/vertical slope resolution?
*/
GBFS solve_flags = {1,1,0,0,0,0,0,0};

	
switch(F_CC)
{	
case 0: //Air tile. Do no collision resolution. This will probably never be used so that the Check_Possible_Tile_Collisions function can be more optimized.
printf("Master Collision Control air tile call. \n");
break;

case 1: //Solid square block.
	return Solver_Rect(p_TBR, &box, solve_flags);			
break;	

case 2: //Slope, bottom left, 45.
	return Solver_Slope(&p_TBR->box, &box, &slo, solve_flags);			
break;

case 3: //Slope, bottom righy, 45.
		slo.m = -1;
		slo.F_corner = 01;
		slo.x += BASE_TILE_SIZE;
	return Solver_Slope(&p_TBR->box, &box, &slo, solve_flags);	
break;

case 4: //Slope. top left, 45.
		slo.m = -1;
		slo.F_corner = 02;
		slo.x += BASE_TILE_SIZE;
	return Solver_Slope(&p_TBR->box, &box, &slo, solve_flags);	
break;

case 5: //Slope, top right, 45.
		slo.m = 1;
		slo.F_corner = 03;
	return Solver_Slope(&p_TBR->box, &box, &slo, solve_flags);		
break;

case 6: //Vertical half block left.
	solve_flags.a = 1;
	box.w = BASE_TILE_SIZE/2;
	return Solver_Rect(p_TBR, &box, solve_flags);
break;

case 7: //Vertical half block right.
	solve_flags.a = 1;
	box.x += BASE_TILE_SIZE/2;
	box.w = BASE_TILE_SIZE/2;
	return Solver_Rect(p_TBR, &box, solve_flags);
break;

case 8: //Horizontal half block top.
	solve_flags.a = 1;
	box.h = BASE_TILE_SIZE/2;
	return Solver_Rect(p_TBR, &box, solve_flags);
break;

case 9: //Horizontal half block bottom.
	solve_flags.a = 1;
	box.y += BASE_TILE_SIZE/2;
	box.h = BASE_TILE_SIZE/2;
	return Solver_Rect(p_TBR, &box, solve_flags);
break;

case 10: //Top left quarter block.
	solve_flags.a = 1;
	box.w = BASE_TILE_SIZE/2;
	box.h = BASE_TILE_SIZE/2;
	return Solver_Rect(p_TBR, &box, solve_flags);
break;

case 11: //Top right quarter block.
	solve_flags.a = 1;
	box.x += BASE_TILE_SIZE/2;
	box.w = BASE_TILE_SIZE/2;
	box.h = BASE_TILE_SIZE/2;
	return Solver_Rect(p_TBR, &box, solve_flags);
break;

case 12: //Bottom left quarter block.
	solve_flags.a = 1;
	box.y += BASE_TILE_SIZE/2;
	box.w = BASE_TILE_SIZE/2;
	box.h = BASE_TILE_SIZE/2;
	return Solver_Rect(p_TBR, &box, solve_flags);
break;

case 13: //Bottom right quarter block.
	solve_flags.a = 1;
	box.x += BASE_TILE_SIZE/2;
	box.y += BASE_TILE_SIZE/2;
	box.w = BASE_TILE_SIZE/2;
	box.h = BASE_TILE_SIZE/2;
	return Solver_Rect(p_TBR, &box, solve_flags);
break;

default:
break;
}	
}

//Checks and solves collisions with map tiles. Needs additional checking for collision after resolution on diagonal movement.
GBFS Tile_Collision_Physical(VBox* self)
{
/*	
a = Right resolution.
b = Left resolution.
c = Top resolution.
d = Bottom resolution.
e = Corner resolution.
f = slope resolution.
g = not yet used.
h = not yet used.
*/
GBFS ret = {0,0,0,0,0,0,0,0};


Box test_tile = {0,0,BASE_TILE_SIZE,BASE_TILE_SIZE};




	if(self->x_vel != 0 || self->y_vel != 0) //If vbox is moving.
	{	
	//Vbox's position and size normalized to the grid. 
	//Bottom_y and right_x have their position + dimension decremented for a tricky reason relating to having 0 x and y positions.
	int left_x_norm = self->box.x/BASE_TILE_SIZE;
	int right_x_norm = (self->box.x + self->box.w - 1)/BASE_TILE_SIZE;
	int top_y_norm = self->box.y/BASE_TILE_SIZE;
	int bottom_y_norm = (self->box.y + self->box.h - 1)/BASE_TILE_SIZE;
	
	int width_norm = right_x_norm - left_x_norm + 1;
	int height_norm = bottom_y_norm - top_y_norm + 1;		
	//Buffer for ret.
	GBFS check = {0,0,0,0,0,0,0,0};		

	unsigned int final_tile;
	signed char h_increment;
	signed int v_increment;

	int current_tile = 0;

		if(self->x_vel > 0)
		{
		final_tile = right_x_norm;
		h_increment = 1;
		}
		else
		{
		final_tile = left_x_norm;	
		h_increment = -1;
		}
			
		if(self->y_vel > 0)
		{
		final_tile += bottom_y_norm*HORIZONTAL_LEVEL_TILES;
		v_increment = HORIZONTAL_LEVEL_TILES;
		}
		else
		{
		final_tile += top_y_norm*HORIZONTAL_LEVEL_TILES;
		v_increment = -HORIZONTAL_LEVEL_TILES;			
		}
		
			if(self->x_vel != 0 && self->y_vel != 0) //If two velocities exist.
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
						test_tile.x = (current_tile%HORIZONTAL_LEVEL_TILES)*BASE_TILE_SIZE;
						test_tile.y = (current_tile/HORIZONTAL_LEVEL_TILES)*BASE_TILE_SIZE;	
						check = Collision_Code_Master_Switch(p_PRIMARY_TILE_ARRAY[current_tile].CollisionCode, self, &test_tile);
						}
						
					//printf("Tile %d at %d,%d.\n", current_tile, test_tile.x, test_tile.y);
					
					ret.a |= check.a; //Bitwise OR adds any 1 collision flags to the return GBFD.
					ret.b |= check.b;
					ret.c |= check.c;
					ret.d |= check.d;
					ret.e |= check.e;
					ret.f |= check.f;
					}	
			}
			//printf("%d tiles scanned. %d x and %d y.\n", total_tiles, x_scanned, y_scanned);
			
				if(ret.e == 1) //Corner case.
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
		
		unsigned int axis = (self->x_vel != 0) ? height_norm : width_norm;	
		int axis_increment = (self->x_vel != 0) ? v_increment : h_increment;	
		
			for(unsigned register int I = 1; I <= axis; I++)
			{
			current_tile = final_tile + (I - axis)*axis_increment;

				if(current_tile > -1 && current_tile < TOTAL_TILES)
				{
					if(p_PRIMARY_TILE_ARRAY[current_tile].CollisionCode != 0)
					{	
					test_tile.x = (current_tile%HORIZONTAL_LEVEL_TILES)*BASE_TILE_SIZE;
					test_tile.y = (current_tile/HORIZONTAL_LEVEL_TILES)*BASE_TILE_SIZE;	
					check = Collision_Code_Master_Switch(p_PRIMARY_TILE_ARRAY[current_tile].CollisionCode, self, &test_tile);
					}
						
				
					
				ret.a |= check.a; 
				ret.b |= check.b;
				ret.c |= check.c;
				ret.d |= check.d;
				ret.e |= check.e;
				ret.f |= check.f;
				}			
			}
			//printf("Tiles = %d - %d = %d.\n", right_x_norm, left_x_norm ,width_norm);
			
		}
	
	
	return ret;
	}		
}


void Resolve_Tile_Collisions(Object* obj, idat* ida)
{
	for(unsigned register int I = 0; I < obj->size[2]; I++)
		{
			if(ida->cb[I].flg.b == 1) //Move is move flag set.
			{
			ida->cb[I].col = Tile_Collision_Physical(&ida->cb[I].vb);		
			}
		}	
}



