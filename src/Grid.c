/*
The purpose of this file is to host all code related to level loading, tiling, and interactivity.
*/

#include "global.h"



/*
This structure will encapsulate the tiles that will compose every map in the game.
The x and y coordinates as well as the ID will be taken from the base Object structure.
The texture for the tile that will be rendered will be pointed to in the TILE_TEXTURE_ARRAY.
A bound texture is unneeded because all grid tiles have global dimensions of BASE_TILE_SIZE (16x16).
Tile_Code_G will tell which member of TILE_TEXTURE_ARRAY the p_TileText will point to.
For tF_flip, N = no flip, V = vertical flip, H = horizontal flip, and B = flipped both ways.
*/
typedef struct
{
	unsigned int TileCode_G;
	unsigned int CollisionCode;
	char BlockPropety;
	SDL_RendererFlip tF_flip;	
} Tile;




uint8_t TILE_SIZE = 16;
uint8_t QUADRANT_SIZE = 16;
uint32_t LEVEL_ID = 0;
uint8_t LEVEL_SIZE_H = 0;
uint8_t LEVEL_SIZE_V = 0;
uint32_t TOTAL_TILES = 0;

unsigned int TOTAL_TILE_TEXTURES = 0;


SDL_Texture* TILES_TEXTURE = NULL;

SDL_Rect* TILE_TEXTURE_ARRAY;


Tile* LEVEL_TILES = NULL;
//FILE pointer that will allow the map data to be read.
FILE* LEVEL_FILE = NULL;






//Allocates default tile array. Will be merged with map loading function eventually.
void AllocTiles()
{

	//Allocate an array of tiles.
	p_PRIMARY_TILE_ARRAY = malloc(TOTAL_TILES * sizeof(Tile));
	
		//Checks if the allocation was successful.
		if (p_PRIMARY_TILE_ARRAY == NULL)
		{
		printf("Error allocating memory for tiles. \n");
		}
		
		else
		{	
			//Assigns all of the allocated tiles default values.
			for(register int i = 0; i < TOTAL_TILES; i++ )
			{
			p_PRIMARY_TILE_ARRAY[i].BlockPropety = 'N';
			p_PRIMARY_TILE_ARRAY[i].tF_flip = SDL_FLIP_NONE;
			p_PRIMARY_TILE_ARRAY[i].TileCode_G = DEBUG_TILE_INDEX;
			p_PRIMARY_TILE_ARRAY[i].CollisionCode = 0;
			}

		printf("%d tiles have been allocated.\n", (TOTAL_TILES));
		}
}



//Assigns the tiles' tesxture and rectangle array.
void Assign_Tile_Texture_Array(const char* PATH)
{
	unsigned register int I = 0;
	unsigned int V = 0;
	unsigned int H = 0;	
	
	if(TILES_TEXTURE != NULL)
	{
	printf("Warning! Attempt to allocate tiles' texture when texture is already present. Attempting deallocation of previous texture.\n");
	SDL_DestroyTexture(TILES_TEXTURE);
	TILES_TEXTURE = NULL;
	}	
	
	
	TILES_TEXTURE = Load_Texture(PATH);		


	if (TILES_TEXTURE == NULL)
	{
	printf("Tile surface source could not be allocated. \n");	
	return;
	}

	//Gets texture width and height and then divides it into tiles.
	SDL_QueryTexture(TILES_TEXTURE, NULL, NULL, &H, &V);
	V /= BASE_TILE_SIZE;
	H /= BASE_TILE_SIZE;
	
	//Allocates the number of sdl rectangles for the tile texture array.
	TILE_TEXTURE_ARRAY = malloc((V*H) * sizeof(SDL_Rect));
	
	if (TILE_TEXTURE_ARRAY == NULL)
	{
	printf("Failed to allocate memory for the Tile Texture Array. \n");	
	return;
	}
	
	if ( V < 1 || H < 1)
	{
	printf("Tile texture source is too small. Tile textures cannot be assigned. \n");
	free(TILE_TEXTURE_ARRAY);
	TILE_TEXTURE_ARRAY = NULL;
	return;
	}



	//Nestled for loops that iterate each index of a row and then continue onto the next column.	
	for(unsigned register int Y = 0; Y < V; Y++)
	{
		for(unsigned register int X = 0; X < H; X++)
		{
		TILE_TEXTURE_ARRAY[I].x = X*BASE_TILE_SIZE;
		TILE_TEXTURE_ARRAY[I].y = Y*BASE_TILE_SIZE;
		TILE_TEXTURE_ARRAY[I].w = BASE_TILE_SIZE;
		TILE_TEXTURE_ARRAY[I].h = BASE_TILE_SIZE;		
		I++;	
		}
	}
	

	TOTAL_TILE_TEXTURES = I;
	DEBUG_TILE_INDEX = I - 1;
	
	printf("	Tile textures successfully created with %d Tile textures from \"%s\" texture.\n", I, PATH);	
}

//Simple destructor for tiles' tecture.
void Free_Tile_Texture()
{
	
	SDL_DestroyTexture(TILES_TEXTURE);
	TILES_TEXTURE = NULL;
	
	free(TILE_TEXTURE_ARRAY);
	TILE_TEXTURE_ARRAY = NULL;

	printf("Tilesheet texture and array have been destroyed. \n");
}

/*
This function will take a text file with level data in it and read it. First, two hexadecimal values are read and stored as the LEVEL_SIZE
dimensions as vertical by horizontal quadrants. Next, a loop will read the next BASE_QUADRANT_SIZExLEVEL_SIZE_V number of values and store them 
in the first horizontal of Tile pointers. This will then be repeated for the following horizontals BASE_QUADRANT_SIZExLEVEL_SIZE_V number of times. 

After this, the objects in the room will be loaded and have their instances generated with the specified replacement values.

!This function should only be called after Allocate_Tile_Array and Assign_Tile_Texture_Array!
*/
void ReadMapData(const char* PATH, DPS* o_dps, unsigned int level_id)
{
	printf("	Reading \"%s\" map data...\n", PATH);
	
	//Opens a stream to the selected map file for reading and checks if the operation succeeded.
	p_LEVEL_FILE = fopen(PATH, "r");
		
		if (p_LEVEL_FILE == NULL)
		{
		printf("Failed to open map file. \n");
		goto EXIT;
		}

	char arrow_check = 'A';
	unsigned int level_check = 0;

		//Find the correct level data.
		while(fscanf(p_LEVEL_FILE, "%c", &arrow_check) != EOF)
		{
			
			if(arrow_check == '>')
			{

				fscanf(p_LEVEL_FILE, "%u", &level_check);
					
				if(level_check == level_id)
				{
				printf("	Loading level %u...\n", level_check);
				break;	
				}
			}
		}
	
		if(arrow_check != '>')
		{
		printf("Error! Could not find level %d.\n", level_id);	
		goto EXIT;	
		}
	
	//Scans and sets the vertical, and horizontal map size.
	fscanf(p_LEVEL_FILE, "%X %X", &LEVEL_SIZE_V, &LEVEL_SIZE_H);

	printf("	size of %xx%x...\n", LEVEL_SIZE_V, LEVEL_SIZE_H);

		//Check for valid map size.
		if (LEVEL_SIZE_V < 1 || LEVEL_SIZE_H < 1)
		{
		printf("Map size too small! No tiles will be allocated or rendered. \n");	
		goto EXIT;
		}
		
	//Losds level tile and background textures.	
	char bg_tile_music_buff[100] = {0};	

	fscanf(p_LEVEL_FILE, "%100s", &bg_tile_music_buff);
	Assign_Tile_Texture_Array(bg_tile_music_buff);	

	fscanf(p_LEVEL_FILE, "%100s", &bg_tile_music_buff);	
	Background = Assign_Texture(NULL, bg_tile_music_buff);
	
	fscanf(p_LEVEL_FILE, "%100s", &bg_tile_music_buff);	
	Load_Music(bg_tile_music_buff);	
	
		
	//Allocates the room tiles.
	Allocate_Tile_Array();
		
		if (p_PRIMARY_TILE_ARRAY == NULL) 
		{
		printf("Map reading aborted due to unallocated tile array! \n");
		goto EXIT;
		}
		
		
	unsigned register int I = 0;
	char flip_buffer = 'O';	
		
		//Assigns tile graphical code and flip flag.
		while (I < TOTAL_TILES && fscanf(p_LEVEL_FILE, "%d%c", &p_PRIMARY_TILE_ARRAY[I].TileCode_G, &flip_buffer) != EOF )
		{								
			switch (flip_buffer)
			{
			case 'H':
			p_PRIMARY_TILE_ARRAY[I].tF_flip = SDL_FLIP_HORIZONTAL;
			flip_buffer = 'O';
			break;
			case 'V':
			p_PRIMARY_TILE_ARRAY[I].tF_flip = SDL_FLIP_VERTICAL;
			flip_buffer = 'O';
			break;
			case 'B':	
			p_PRIMARY_TILE_ARRAY[I].tF_flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
			flip_buffer = 'O';
			break;			
			}
			
			//Assigns the debug tile if tilecode is too large.
			if (p_PRIMARY_TILE_ARRAY[I].TileCode_G > DEBUG_TILE_INDEX)
			{
			p_PRIMARY_TILE_ARRAY[I].TileCode_G = DEBUG_TILE_INDEX;
			}	
				
			I++;		
		}		
				
		if (I < TOTAL_TILES)
		{
		printf("\nError! Early end of map file stream! Extra tiles default to failsafe. \n");	
		}			
				
	I = 0;
		
		//Assigns tile collision code.
		while (I < TOTAL_TILES && fscanf(p_LEVEL_FILE, "%d", &p_PRIMARY_TILE_ARRAY[I].CollisionCode) != EOF)
		{
		I++;
		}

		if (I < TOTAL_TILES)
		{
		printf("\nError! Early end of map file stream! Extra tiles default to failsafe. \n");	
		}
		
	
		
	unsigned int num_of_objects = 0;
	unsigned int object_ID = 0;

	unsigned int instances = 0;	
	int x_y[2] = {0,0};
	Object* obj_address = NULL;
	
	long int special_buff[10] = {0};
		
	fscanf(p_LEVEL_FILE, "%d", &num_of_objects);	
	printf("	Loading %d level objects...\n", num_of_objects);
	
		for(register int I = 0; I < num_of_objects; I++) //Loops for every object type.
		{
			fscanf(p_LEVEL_FILE, "%d", &object_ID);
			
			obj_address = Load_Object("Assets/o_data", OBJECT_P_ARRAY, object_ID);					
			
			fscanf(p_LEVEL_FILE, "%d", &instances);

				for(unsigned register int I = 0; I < instances; I++) //Loops for instance.
				{					
					fscanf(p_LEVEL_FILE, "%d %d", &x_y[0], &x_y[1]);
					
					for(unsigned register int S = 0; S < 10; S++)
					{
						fscanf(p_LEVEL_FILE, "%d", &special_buff[S]);										
					}													
					
				Create_Object_Instance(obj_address, x_y, special_buff);	//Creates the instance.			
				}
							
			printf("|%d|->|%d|\n", object_ID, instances);
		}
					
			
	EXIT:

	if(p_LEVEL_FILE != NULL)
	{
	printf("Map loading complete. fclose return = %d\n",fclose(p_LEVEL_FILE));
	}
	p_LEVEL_FILE = NULL;	
}





//Function that will render the grid tiles each frame. Tiles offscreen will be culled.
void Render_Tiles()
{
//This pre-loop code only allows tiles on screen to be rendered.
unsigned int V_Begin = SCREEN.y/BASE_TILE_SIZE;
unsigned int H_Begin = SCREEN.x/BASE_TILE_SIZE;

unsigned int V_End = V_Begin + SCREEN.h/BASE_TILE_SIZE + 1;
unsigned int H_End = H_Begin + SCREEN.w/BASE_TILE_SIZE + 1;

	if(V_End > VERTICAL_LEVEL_TILES)
	{
	V_End = VERTICAL_LEVEL_TILES;
	}
	
	if(H_End > HORIZONTAL_LEVEL_TILES)
	{
	H_End = HORIZONTAL_LEVEL_TILES;	
	}	


	if (p_PRIMARY_TILE_ARRAY != NULL)
	{

		for (register int V = V_Begin; V < V_End; V++)	
		{	
			for (register int H = H_Begin; H < H_End; H++)
			{
			unsigned int Tile_select = (V*HORIZONTAL_LEVEL_TILES + H);
			
			
			SDL_Rect Render_Rect = { H*BASE_TILE_SIZE - SCREEN.x, V*BASE_TILE_SIZE - SCREEN.y, BASE_TILE_SIZE, BASE_TILE_SIZE};
		
			SDL_RenderCopyEx(RENDERER, TILES_TEXTURE, &TILE_TEXTURE_ARRAY[p_PRIMARY_TILE_ARRAY[(Tile_select)].TileCode_G], &Render_Rect, 0.0, NULL, p_PRIMARY_TILE_ARRAY[(Tile_select)].tF_flip );
			}
		}
	}
}