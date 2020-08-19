#include <Global.h>
#include <Object.h>
#include <stdlib.h>
#include <Graphics.h>
#include <Grid.h>
#include <Input.h>













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

void Print_O_Array(DPS* O_arr)
{
	printf("____________OBJECT_ARRAY_DISECTION__________\n");
		for(register int N = 0, S = 0; N < O_arr->non_null && S < O_arr->size; S++)
		{
			if(O_arr->Array[S])
			{
			printf("ID: %d\n", ((Object*)O_arr->Array[S])->ID);
			printf("Index: %d | Actual index: %d\n", ((Object*)O_arr->Array[S])->index, S);
			printf("GBFS: %d HB: %d CB: %d TB: %d IV: %d\n", ((Object*)O_arr->Array[S])->size[0], ((Object*)O_arr->Array[S])->size[1], ((Object*)O_arr->Array[S])->size[2], ((Object*)O_arr->Array[S])->size[3], ((Object*)O_arr->Array[S])->size[4]);
			printf("Texture: ");	
				if( ((Object*)O_arr->Array[S])->p_T )
				{
				printf("%s \n", ((T_Bind*) ((Object*) O_arr->Array[S])->p_T)->Name );		
				}
				else
				{
				printf("NULL \n");	
				}
			printf("Function: ");
				if( ((Object*)O_arr->Array[S])->func )
				{
				printf("NON-NULL\n");		
				}
				else
				{
				printf("NULL \n");	
				}				
			printf("Instances:\n");
			
					printf("	Size: %d\n", ((Object*)O_arr->Array[S])->dps_ins->size);
					printf("	Non-null: %d\n", ((Object*)O_arr->Array[S])->dps_ins->non_null);
					
					for(register int NU = 0, SI = 0; NU < ((Object*)O_arr->Array[S])->dps_ins->non_null && SI < ((Object*)O_arr->Array[S])->dps_ins->size; SI++ )
					{
						
						
						if( ((Object*)O_arr->Array[S])->dps_ins->Array )
						{
						printf("	|%d|:\n", SI);
						printf("	Position: x = %d, y = %d\n", ((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->cb[0].vb.box.x, ((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->cb[0].vb.box.y );
						printf("	Special: %d %d %d %d %d %d %d %d %d %d\n", 
						((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->special[0],
						((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->special[1],
						((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->special[2],
						((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->special[3],
						((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->special[4],
						((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->special[5],
						((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->special[6],
						((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->special[7],
						((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->special[8],
						((idat*) ((Object*)O_arr->Array[S])->dps_ins->Array[SI])->special[9]	
						);
					
						NU++;
						}
					}
			
			N++;
			}
		}
	
	
		printf("________________END_DISECTION_______________\n");
}


void Dump_Data_To_Console()
{
printf("_____________GAME DATA DUMP_____________ \n");
printf("window size --------> %dx%d (pixels)\n", WINDOW_HEIGHT, WINDOW_WIDTH);
printf("screen size --------> %dx%d (pixels)\n", SCREEN.h, SCREEN.w);
printf("screen offsets -----> %d,%d (x,y)\n", SCREEN.x, SCREEN.y);
printf("target fps/delay --> %d / %.4f (seconds)\n", TARGET_FPS, (1.0000/TARGET_FPS));
printf("recognized keys ----> %d (including unknown)\n", TOTAL_KEYS);
printf("total textures -----> %d \n", TEXTURE_COUNT);
printf("total objects ------> %d = %d\n", OBJECT_COUNT, OBJECT_P_ARRAY->non_null);
printf("object array size --> %d\n", OBJECT_P_ARRAY->size);
Print_O_Array(OBJECT_P_ARRAY);
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



/*
Box Give_Box_of_Tile

int Box_Distance_From_Tiles(Box* B, char direction)
{
	unsigned int total_tiles;	
	unsigned int tile_inc;
	
	if(direction == 'U' || direction == 'D') //Scan tiles above or below box.
	{
	tile_inc = 1;
	
	}
	else //Scam tiles left or right of box.
	{
		
	}
}

*/

