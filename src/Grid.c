/*
The purpose of this file is to host all code related to level loading, tiling, and interactivity.
*/

#include "global.h"
#include "gfx.h"


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
	uint8_t tCode; //Texture code.
	uint8_t cCode; //Collision code.
	uint8_t bProp;
	SDL_RendererFlip flip;	
} Tile;


uint8_t TILE_SIZE = 16;
uint8_t QUADRANT_SIZE = 16;

uint32_t LEVEL_ID = 0;
int LEVEL_SIZE_H = 0;
int LEVEL_SIZE_V = 0;
uint32_t TOTAL_TILES = 0;

SDL_Texture *TileTexture = NULL;
int TILES_TEXTURE_W = 0;
int TILES_TEXTURE_H = 0;

//Level tile array.
Tile* LEVEL_TILES = NULL;
//FILE pointer that will allow the map data to be read.
FILE* LEVEL_FILE = NULL;






uint_fast8_t AllocTiles()
{
	LEVEL_TILES = malloc(TOTAL_TILES * sizeof(Tile));
	
		if (!LEVEL_TILES)
		{
		puts("Error allocating memory for tiles.");
		return 1;
		}	
		else
		{	
			//Assigns all of the allocated tiles default values.
			for(uint_fast32_t i = 0; i < TOTAL_TILES; i++ )
			{
			LEVEL_TILES[i].bProp = 'N';
			LEVEL_TILES[i].flip = SDL_FLIP_NONE;
			LEVEL_TILES[i].tCode = 0;
			LEVEL_TILES[i].cCode = 0;
			}

		printf("%d tiles have been allocated.\n", (TOTAL_TILES));
		return 0;
		}
}

//Assigns the tiles' tesxture and rectangle array.
void AssignTileTextureArray(const char* PATH)
{
	if(TileTexture)
	{
	puts("Warning! Attempt to allocate tiles' texture when texture is already present. Attempting deallocation of previous texture.");
	SDL_DestroyTexture(TileTexture);
	TileTexture = NULL;
	}	
	
	TileTexture = Load_Texture(PATH);		

	if(!TileTexture)
	{
	puts("Tile surface source could not be allocated.");	
	return;
	}

	//Gets texture width and height and then divides it into tiles.
	SDL_QueryTexture(TileTexture, NULL, NULL, &TILES_TEXTURE_H, &TILES_TEXTURE_W);
	TILES_TEXTURE_H /= TILE_SIZE;
	TILES_TEXTURE_W /= TILE_SIZE;
	
	if ( TILES_TEXTURE_H < 1 || TILES_TEXTURE_W < 1)
	{
	puts("Tile texture source is too small. Tile textures cannot be assigned.");
	return;
	}
	
	printf("	Tile textures successfully created with %d Tile textures from \"%s\" texture.\n", TILES_TEXTURE_H*TILES_TEXTURE_W, PATH);	
}


//Reads formatted map data from specified file and loads it for a level.
void ReadMapData(const char* PATH, unsigned int level_id)
{
	printf("	Reading \"%s\" map data...\n", PATH);
	
	FILE *pFile = fopen(PATH, "r");
		
		if(!pFile)
		{
		puts("Failed to open map file.");
		goto EXIT;
		}

	char arrowCheck = 'A';
	uint_fast16_t levelCheck = 0;

		//Find the correct level data.
		while(fscanf(pFile, "%c", &arrowCheck) != EOF)
		{
			if(arrowCheck == '>')
			{
				fscanf(pFile, "%u", &levelCheck);
					
				if(levelCheck == level_id)
				{
				printf("Loading level %u ", levelCheck);
				break;	
				}
			}
		}
	
		if(arrowCheck != '>')
		{
		printf("Error! Could not find level %d.\n", level_id);	
		goto EXIT;	
		}
	
	char *TT, *BG, *MU = NULL;
	TT = malloc(100*sizeof(char));
	BG = malloc(100*sizeof(char));
	MU = malloc(100*sizeof(char));

	fscanf(pFile, "%X %X %s %s %s", &LEVEL_SIZE_V, &LEVEL_SIZE_H, &TT, &BG, &MU);

		if (LEVEL_SIZE_V < 1 || LEVEL_SIZE_H < 1)
		{
		puts("Map size too small! No tiles will be allocated or rendered.");	
		goto EXIT;
		}
		else if(!TT || !BG || !MU)
		{
		puts("Map resources could not be loaded.");
		goto EXIT;
		}

	printf(" size of %xx%x, TT=\"%s\", BG=\"%s\", MU=\"%s\".\n", LEVEL_SIZE_V, LEVEL_SIZE_H, TT, BG, MU);

	TOTAL_TILES = HORIZONTAL_LEVEL_TILES*VERTICAL_LEVEL_TILES;

	AssignTileTextureArray(TT);	
	//Background = Assign_Texture(NULL, bg_tile_music_buff);
	//Load_Music(bg_tile_music_buff);	
		
		if (AllocTiles()) 
		{
		puts("Map reading aborted due to unallocated tile array!");
		goto EXIT;
		}
		
		
	uint_fast32_t I = 0;
	char flip_buffer = 'O';	
		
		//Assigns tile texture code and flip flag.
		while (I < TOTAL_TILES && fscanf(pFile, "%d%c", &LEVEL_TILES[I].tCode, &flip_buffer) != EOF )
		{								
			switch (flip_buffer)
			{
			case 'H':
			LEVEL_TILES[I].flip = SDL_FLIP_HORIZONTAL;
			flip_buffer = 'O';
			break;
			case 'V':
			LEVEL_TILES[I].flip = SDL_FLIP_VERTICAL;
			flip_buffer = 'O';
			break;
			case 'B':	
			LEVEL_TILES[I].flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
			flip_buffer = 'O';
			break;			
			}
			
			//Assigns the debug tile if tilecode is too large.
			if (LEVEL_TILES[I].tCode >= TILES_TEXTURE_W*TILES_TEXTURE_H)
			{
			LEVEL_TILES[I].tCode = TILES_TEXTURE_W*TILES_TEXTURE_H - 1;
			}	
				
			I++;		
		}		
				
		if (I < TOTAL_TILES)
		{
		printf("\nError! Early end of map file stream! Extra tiles default to failsafe. \n");	
		}			
				
	I = 0;
		
		//Assigns tile collision code.
		while (I < TOTAL_TILES && fscanf(pFile, "%d", &LEVEL_TILES[I].cCode) != EOF)
		{
		I++;
		}

		if (I < TOTAL_TILES)
		{
		printf("\nError! Early end of map file stream! Extra tiles default to failsafe. \n");	
		}
						
			
	EXIT:
	free(TT);
	free(BG);
	free(MU);

	if(pFile)
	{
	printf("Map loading complete. fclose return = %d\n", fclose(pFile));
	}
}



//Function that will render the grid tiles each frame. Includes occlusion culling. Move to gfx.c.
void Render_Tiles()
{
unsigned int V_Begin = SCREEN.y/TILE_SIZE;
unsigned int H_Begin = SCREEN.x/TILE_SIZE;

unsigned int V_End = V_Begin + SCREEN.h/TILE_SIZE + 1;
unsigned int H_End = H_Begin + SCREEN.w/TILE_SIZE + 1;

SDL_Rect textureRect, renderRect = {.x=0, .y=0, .w=TILE_SIZE, .h=TILE_SIZE};

	if(V_End > VERTICAL_LEVEL_TILES)
	{
	V_End = VERTICAL_LEVEL_TILES;
	}
	
	if(H_End > HORIZONTAL_LEVEL_TILES)
	{
	H_End = HORIZONTAL_LEVEL_TILES;	
	}	


	if (LEVEL_TILES)
	{
		for (uint_fast32_t V = V_Begin; V < V_End; V++)	
		{	
			for (uint_fast32_t H = H_Begin; H < H_End; H++)
			{
			unsigned int tile = (V*HORIZONTAL_LEVEL_TILES + H);
			
			renderRect.x = H*TILE_SIZE - SCREEN.x;
			renderRect.y = V*TILE_SIZE - SCREEN.y;
			textureRect.x = (LEVEL_TILES[tile].tCode%TILES_TEXTURE_W)*TILE_SIZE;
			textureRect.y = (LEVEL_TILES[tile].tCode/TILES_TEXTURE_W)*TILE_SIZE;

			SDL_RenderCopyEx(RENDERER, TileTexture, &textureRect, &renderRect, 0.0, NULL, LEVEL_TILES[(tile)].flip );
			}
		}
	}
}