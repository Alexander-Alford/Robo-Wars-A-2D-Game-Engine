//#include "global.h"










/*

void Print_Map(char type)
{
register int V = 0;		
register int H = 0;	

switch(type)
{
	case 'P':
	printf("LEVEL_PHYSICAL\n");
	break;
	case 'G':
	printf("LEVEL_GRAPHICAL\n");
	break;
}
			


	while(((V*BASE_QUADRANT_SIZE*LEVEL_SIZE_H) + H + 1) < TOTAL_TILES)
	{
		while(H  < (LEVEL_SIZE_H*BASE_QUADRANT_SIZE))
		{
			switch(type)
			{
			case 'P':
			printf("%X ", p_PRIMARY_TILE_ARRAY[(V*BASE_QUADRANT_SIZE*LEVEL_SIZE_H) + H].CollisionCode);
			break;
			case 'G':
			printf("%X ", p_PRIMARY_TILE_ARRAY[(V*BASE_QUADRANT_SIZE*LEVEL_SIZE_H) + H].TileCode_G);
			break;
			}	

		
			if((H + 1) % BASE_QUADRANT_SIZE == 0)
			{
			printf("| ");	
			}	
		
		H++;	
		}
	printf("\n");
	H = 0;
	
		if((V + 1) % BASE_QUADRANT_SIZE == 0)
		{
			for(register int I = 0; I < (BASE_QUADRANT_SIZE*LEVEL_SIZE_H) + LEVEL_SIZE_H; I++)
			{
			printf("- ");	
			}
		printf("\n");
		}
	V++;
	}	
	
	
}




void Dump_Data_To_Console()
{
printf("_____________GAME DATA DUMP_____________ \n");
printf("window size --------> %dx%d (pixels)\n", WINDOW_HEIGHT, WINDOW_WIDTH);
printf("screen size --------> %dx%d (pixels)\n", SCREEN.h, SCREEN.w);
printf("screen offsets -----> %d,%d (x,y)\n", SCREEN.x, SCREEN.y);
printf("target fps/delay ---> %f / %.4f (seconds)\n", TARGET_FPS, (1.0000/TARGET_FPS));
printf("rendered frames ----> %u \n", FRAMES_ELAPSED);
printf("average fps --------> %f \n", AVERAGE_FPS);
printf("recognized keys ----> %d (including unknown)\n", TOTAL_KEYS);
printf("total textures -----> %d \n", TEXTURE_COUNT);
printf("total objects ------> %d = %d\n", OBJECT_COUNT, OBJECT_P_ARRAY->non_null);
printf("object array size --> %d\n", OBJECT_P_ARRAY->size);
printf("tile size ----------> %d (%cpixels)\n", BASE_TILE_SIZE, 251);	
printf("quadrant size ------> %d (%ctiles)\n", BASE_QUADRANT_SIZE, 251);
printf("tile textures ------> %d \n", TOTAL_TILE_TEXTURES);
printf("debug tile index ---> %d \n", DEBUG_TILE_INDEX);
printf("level size ---------> %dx%d (quadrants)\n", LEVEL_SIZE_V, LEVEL_SIZE_H);
printf("total level tiles --> %d \n", TOTAL_TILES);
Print_Map('P');
Print_Map('G');
printf("________________END DUMP________________\n");
}


void Debug_VBox_Mover(VBox* p_self)
{
	static int sub_pix_x = 0;
	static int sub_pix_y = 0;
	static int sub_px_inc = 0;
	static int sub_py_inc = 0;
	static int hold_x = 100;
	static int hold_y = 100;
	
	p_self->x_vel = 0;
	p_self->y_vel = 0;
	
	if (F_KeyState[UP] == 1 || F_KeyState[UP] == 2)
	{
	sub_py_inc -= 10;	
	}
	
	if (F_KeyState[DOWN] == 1 || F_KeyState[DOWN] == 2)
	{
	sub_py_inc += 10;
	}


	if (F_KeyState[LEFT] == 1 || F_KeyState[LEFT] == 2)
	{
	sub_px_inc -= 10;
	}
	
	if (F_KeyState[RIGHT] == 1 || F_KeyState[RIGHT] == 2)
	{
	sub_px_inc += 10;
	}
	
	
	sub_pix_x += sub_px_inc;
	sub_pix_y += sub_py_inc;
	
		while(sub_pix_x > 100)
		{
		p_self->x_vel += 1;	
		sub_pix_x -= 100;
		}
		while(sub_pix_x < -100)
		{
		p_self->x_vel -= 1;	
		sub_pix_x += 100;
		}

		while(sub_pix_y < -100)
		{
		p_self->y_vel -= 1;	
		sub_pix_y += 100;
		}
		while(sub_pix_y > 100)
		{
		p_self->y_vel += 1;	
		sub_pix_y -= 100;
		}		
	
	if(p_self->x_vel > 10)
	{
	p_self->x_vel = 10;	
	}
	else if(p_self->x_vel < -10)
	{
	p_self->x_vel = -10;	
	}
	
	if(p_self->y_vel > 10)
	{
	p_self->y_vel = 10;	
	}
	else if(p_self->y_vel < -10)
	{
	p_self->y_vel = -10;	
	}	
	
	
	if(F_KeyState[X] == 1)
	{
	hold_x = p_self->box.x;
	hold_y = p_self->box.y;
	printf("xv %d yv %d\n", p_self->x_vel, p_self->y_vel);	
	}
	
	if(F_KeyState[S] == 1)
	{
	p_self->box.x = hold_x;
	p_self->box.y = hold_y;	
	}
	
	
	
}

void Debug_VBox_Mover_two(VBox* p_self, unsigned int vel)
{
	p_self->y_vel = 0;	
	p_self->x_vel = 0;
	
	if (F_KeyState[UP] == 1 || F_KeyState[UP] == 2)
	{
	p_self->y_vel = -vel;	
	}
	
	if (F_KeyState[DOWN] == 1 || F_KeyState[DOWN] == 2)
	{
	p_self->y_vel = vel;
	}


	if (F_KeyState[LEFT] == 1 || F_KeyState[LEFT] == 2)
	{
	p_self->x_vel = -vel;
	}
	
	if (F_KeyState[RIGHT] == 1 || F_KeyState[RIGHT] == 2)
	{
	p_self->x_vel = vel;
	}	
}




*/